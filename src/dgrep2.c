#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE
#define __MSVCRT_VERSION__ 0x0700

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

#include "utils.c"

/**
 * Name....: dgrep2.c
 * Author..: Xanadrel
 * Desc....: Match start of line in sorted file
 * License.: MIT
 */

static int cmp_cache (const void *p1, const void *p2, const int len)
{
    return strncmp (p1, p2, len);
}


int main (int argc, char *argv[])
{
    FILE* fd1;

    struct stat64 infile_stat;

    if (argc != 3)
    {
        fprintf (stderr, "usage: %s infile search\n", argv[0]);

        return (-1);
    }

    char *infile        = argv[1];
    char *search_string = argv[2];

    if(stat64(infile,&infile_stat) < 0)
    {
        return 1;
    }

    if ((fd1 = fopen(infile, "r")) == NULL)
    {
        fprintf (stderr, "%s: %s\n", infile, strerror (errno));

        return (-1);
    }

    off64_t size_min = 0;
    off64_t size_mid = 0;
    off64_t size_max = infile_stat.st_size;
    off64_t read_len, memory_len = 0;

    off64_t extra_read = 0;

    char line_buf[BUFSIZ];
    char memory[BUFSIZ];
    int comp;
    int cmp_count = 0;
    int len = strlen(search_string);

    bool matched = false;
    bool no_stop_first_match = true;
  
    size_mid = (size_min + size_max) / 2;

    // Find a match
    while (!feof (fd1))
    {
        extra_read = 0;
        if ((read_len = fgetl_closest64 (fd1, BUFSIZ, line_buf, size_mid, &extra_read)) == -1) memset (line_buf, 0, BUFSIZ);
        comp = cmp_cache (line_buf, search_string, len);
        cmp_count++;


        if (comp == 0)
        {
            puts (line_buf);
            size_mid = size_mid + read_len + extra_read + 1;
            size_min = size_mid;
            matched = true;

            break;
        }
        else if (comp > 0 && !matched) // mid/2
        {
            if (read_len > 0 && memory_len == read_len && cmp_cache(line_buf, memory, read_len) == 0) break;
            size_max = size_mid + extra_read;
            size_mid = (size_min + size_max) / 2;           
        }
        else if (comp < 0 && !matched) // max/2
        {
            if (read_len > 0 && memory_len == read_len && cmp_cache(line_buf, memory, read_len) == 0) break;
            size_min = size_mid - extra_read;
            size_mid = (size_min + size_max) / 2;
        }
        else
        {
            printf("\nWHAT AM I DOING HERE ?\n");
            break;
        }
        memcpy(memory, line_buf, read_len);
        memory_len = read_len;
    }

    fclose (fd1);

    return 0;
}

