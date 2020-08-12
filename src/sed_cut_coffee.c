#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#define __MSVCRT_VERSION__ 0x0700

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "utils.c"

/**
 * Name...: sed_cut_coffee.c
 * Author.: Xanadrel
 * Desc...: Prints lines removing leading count & spaces (output of uniq -c), replaces cut -bX-
 * License.: MIT
 */

int main (int argc, char *argv[])
{
  FILE *fd1;

  if (argc != 2)
  {
    fprintf (stderr, "usage: %s file\n", argv[0]);

    return (-1);
  }

  char *infile     = argv[1];

  if ((fd1 = fopen (infile, "rb")) == NULL)
  {
    fprintf (stderr, "%s: %s\n", infile, strerror (errno));

    return (-1);
  }


  int offset;
  char line_buf1[BUFSIZ];
  char* ptr_skip;
  offset = 0;

  if ((fgetl (fd1, BUFSIZ, line_buf1)) == -1) memset (line_buf1, 0, BUFSIZ);

  ptr_skip = line_buf1;
  while (*ptr_skip++ == ' ') { offset++; } // skips spaces at start
  while (*ptr_skip++ != ' ') { offset++; } // skips count
  offset += 2; // skips space and last count
  
  while (!feof (fd1))
  {
    puts(line_buf1 + offset); 
    
    if ((fgetl (fd1, BUFSIZ, line_buf1)) == -1) memset (line_buf1, 0, BUFSIZ);
  }

  fclose (fd1);

  return 0;
}
