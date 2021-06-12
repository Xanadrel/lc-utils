#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#define __MSVCRT_VERSION__ 0x0700

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "utils.c"

/**
 * Name....: cmp_rli2.c
 * Author..: Xanadrel
 * Desc....: Get percentage of similar lines between two files
 * License.: MIT
 */

static int cmp_cache(const void *p1, const void *p2)
{
    return strcmp(p1, p2);
}

int main(int argc, char *argv[])
{
    FILE *fd1;
    FILE *fd2;

    if (argc != 3)
    {
        fprintf(stderr, "usage: %s infile removefile\n", argv[0]);

        return (-1);
    }

    char *infile = argv[1];
    char *removefile = argv[2];

    if ((fd1 = fopen(infile, "rb")) == NULL)
    {
        fprintf(stderr, "%s: %s\n", infile, strerror(errno));

        return (-1);
    }

    if ((fd2 = fopen(removefile, "rb")) == NULL)
    {
        fprintf(stderr, "%s: %s\n", removefile, strerror(errno));

        return (-1);
    }

    char line_buf1[BUFSIZ];
    char line_buf2[BUFSIZ];

    if (fgetl(fd1, BUFSIZ, line_buf1) == -1) memset(line_buf1, 0, BUFSIZ);
    if (fgetl(fd2, BUFSIZ, line_buf2) == -1) memset(line_buf2, 0, BUFSIZ);

    int comp = 1;
    uint similar = 0;
    uint total = 1;

    while (!feof(fd1) && !feof(fd2))
    {
        comp = cmp_cache(line_buf1, line_buf2);

        if (comp == 0)
        {
            if (fgetl(fd1, BUFSIZ, line_buf1) == -1) memset(line_buf1, 0, BUFSIZ);
            if (fgetl(fd2, BUFSIZ, line_buf2) == -1) memset(line_buf2, 0, BUFSIZ);

            similar++;
            total++;
        }
        else if (comp > 0)
        {
            if (fgetl(fd2, BUFSIZ, line_buf2) == -1) memset(line_buf2, 0, BUFSIZ);
        }
        else if (comp < 0)
        {
            if (fgetl(fd1, BUFSIZ, line_buf1) == -1) memset(line_buf1, 0, BUFSIZ);
            total++;
        }
    }

    if (!feof (fd1) && comp == 0) similar++;

    while (!feof (fd1))
    {
        if (fgetl (fd1, BUFSIZ, line_buf1) == -1)
        {
            memset (line_buf1, 0, BUFSIZ);
            continue;
        }
        total++;
    }

    fclose(fd1);
    fclose(fd2);

    printf("%s:%s %f%%\n", infile, removefile, (float)100 * (float)similar / (float)total);

    return 0;
}
