#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#define __MSVCRT_VERSION__ 0x0700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include "utils.c"

/**
 * Name....: metagen.c
 * Autor...: Xanadrel
 * Desc....: Generate meta file from a sorted but not unique list
 * License.: MIT
 */

static int cmp_cache (const void *p1, const void *p2)
{
    return strcmp (p1, p2);
}

int main (int argc, char *argv[])
{
    FILE *fpList, *fpMeta;
    int line_count;

    if (argc != 3)
    {
        fprintf (stderr, "usage: %s listfile metafile\n", argv[0]);

        return (-1);
    }

    char *listfile = argv[1];
    char *metafile = argv[2];

    if ((fpList = fopen (listfile, "rb")) == NULL)
    {
        fprintf (stderr, "%s: %s\n", listfile, strerror (errno));

        return (-1);
    }

    if ((fpMeta = fopen (metafile, "wb")) == NULL)
    {
        fprintf (stderr, "%s: %s\n", metafile, strerror (errno));

        fclose (fpList);

        return (-1);
    }

    char previous_line[BUFSIZ];
    char current_line[BUFSIZ];

    if (fgetl (fpList, BUFSIZ, previous_line) == -1) memset (previous_line, 0, BUFSIZ);

    int comp;
    line_count = 1;
    int line_number = 0;

    while (!feof (fpList))
    {
        if (fgetl (fpList, BUFSIZ, current_line) == -1) memset (previous_line, 0, BUFSIZ);
        comp = cmp_cache (previous_line, current_line);

        if (comp == 0)
        {
            line_count++;
            strcpy(previous_line, current_line);
        }
        else
        {
            if (line_count > 1)
            {
                fprintf(fpMeta, "%d %d\n", line_number, line_count); // print line number followed by count

                line_count = 1;
            }
            line_number++;
            strcpy(previous_line, current_line);
        }
    }

    fclose (fpList);
    fclose (fpMeta);

    return 0;
}
