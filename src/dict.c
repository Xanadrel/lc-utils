#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#define __MSVCRT_VERSION__ 0x0700

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "utils.c"

/**
 * name...: dict.c
 * author.: Xanadrel
 * desc...: prints the dictionary/wordlist (plain) using a <leftlist> (hash[:salt]), and a <foundfile> (hash[:salt]:plain)
 */

static int cmp_cache (const void *p1, const void *p2, const int len)
{
  return strncmp (p1, p2, len);
}

int main (int argc, char *argv[])
{
  FILE *fd1;
  FILE *fd2;

  if (argc != 3)
  {
    fprintf (stderr, "usage: %s <leftfile> <foundfile>\n", argv[0]);

    return (-1);
  }

  char *infile     = argv[1];
  char *removefile = argv[2];

  if ((fd1 = fopen (infile, "rb")) == NULL)
  {
    fprintf (stderr, "%s: %s\n", infile, strerror (errno));

    return (-1);
  }

  if ((fd2 = fopen (removefile, "rb")) == NULL)
  {
    fprintf (stderr, "%s: %s\n", removefile, strerror (errno));

    return (-1);
  }
  int len;
  char line_buf1[BUFSIZ];
  char line_buf2[BUFSIZ];

  len = 0;

  if ((len = fgetl (fd1, BUFSIZ, line_buf1)) == -1) memset (line_buf1, 0, BUFSIZ);
  if (fgetl (fd2, BUFSIZ, line_buf2) == -1) memset (line_buf2, 0, BUFSIZ);

  int comp = 1;

  while (!feof (fd1) && !feof (fd2))
  {
    comp = cmp_cache (line_buf1, line_buf2, len);

    if (comp == 0)
    {
      printf("%s\n", line_buf2 + len + 1);

      if ((len = fgetl (fd1, BUFSIZ, line_buf1)) == -1) memset (line_buf1, 0, BUFSIZ);
      if (fgetl (fd2, BUFSIZ, line_buf2) == -1) memset (line_buf2, 0, BUFSIZ);
    }
    else if (comp > 0)
    {
      if (fgetl (fd2, BUFSIZ, line_buf2) == -1) memset (line_buf2, 0, BUFSIZ);
    }
    else if (comp < 0)
    {
      if ((len = fgetl (fd1, BUFSIZ, line_buf1)) == -1) memset (line_buf1, 0, BUFSIZ);
    }
  }

  fclose (fd1);
  fclose (fd2);

  return 0;
}
