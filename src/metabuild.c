#define _GNU_SOURCE
#define _FILE_OFFSET_BITS 64
#define __MSVCRT_VERSION__ 0x0700

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "utils.c"

/**
 * Name....: metabuild.c
 * Autor...: Xanadrel
 * Desc....: Generate list/dict/count matching original list using sortu list + meta file
 * License.: MIT
 *
 * Requirements:
 *  - listfile must be sortu
 *  - foundfile must be sortu
 *  - metafile must match original sorted but not uniqued list
 */

#define BIGBUFSIZ (BUFSIZ * 4)

typedef struct {
    int line_number;
    int line_count;
} meta_t;

static int cmp_cache (const void *p1, const void *p2)
{
    return strcmp (p1, p2);
}

static int cmp_cache_len (const void *p1, const void *p2, const size_t len)
{
    return strncmp (p1, p2, len);
}

void read_meta(char *line, meta_t *meta)
{
    // atoi + strtok is a bit shitty, strtol, good
    char * pEnd;
    meta->line_number = strtol(line, &pEnd, 10);
    meta->line_count = strtol(pEnd, NULL, 10);
}

int main (int argc, char *argv[])
{
    // Files
    FILE *fdList, *fdMeta, *fdFound = NULL;

    if (argc < 4 || argc > 5 )
    {
        fprintf (stderr, "usage: %s [type] listfile metafile [foundfile]\n", argv[0]);
        // types: list, dict, count, left?

        return (-1);
    }

    char *str_type = argv[1];
    char *listfile = argv[2];
    char *metafile = argv[3];
    int type;

    if (strcmp(str_type,  "dict") == 0 ||
        strcmp(str_type, "count") == 0)
    {
        if (argc != 5)
        {
            fprintf (stderr, "Error: Need foundfile for type 'dict/count'\n");
            return (-1);
        }

        type = (strcmp(str_type,  "dict") == 0) ? 0 : 1;
        char *foundfile = argv[4];

        if ((fdFound = fopen (foundfile, "rb")) == NULL)
        {
            fprintf (stderr, "%s: %s\n", foundfile, strerror (errno));

            return (-1);
        }
    }
    else if (strcmp(str_type, "list") == 0)
    {
        if (argc != 4)
        {
            fprintf (stderr, "Error: Unneeded foundfile for type 'list'\n");
            return (-1);
        }

        type = 2;
    }
    else
    {
        fprintf (stderr, "Error: Unrecognized type\n");
        return (-1);
    }

    // Open list & meta
    if ((fdList = fopen (listfile, "rb")) == NULL)
    {
        fprintf (stderr, "%s: %s\n", listfile, strerror (errno));

        return (-1);
    }

    if ((fdMeta = fopen (metafile, "rb")) == NULL)
    {
        fprintf (stderr, "%s: %s\n", metafile, strerror (errno));

        fclose (fdList);

        return (-1);
    }

    char line_list[BUFSIZ];
    char line_found[BIGBUFSIZ];
    char line_meta[BUFSIZ];
    meta_t meta;

    int i, len, comp, lines_to_print;
    int line_number = -1;

    // read meta
    if (fgetl(fdMeta, BUFSIZ, line_meta) == -1) memset(line_meta, 0, BUFSIZ);
    read_meta(line_meta, &meta);

    // region Count found
    if (type == 1) { // count found

        int count_found = 0;

        // read found
        if (fgetl(fdFound, BIGBUFSIZ, line_found) == -1) memset(line_found, 0, BIGBUFSIZ);

        // read list
        if ((len = fgetl(fdList, BUFSIZ, line_list)) == -1) memset(line_list, 0, BUFSIZ);
        line_number++;

        while (!feof(fdFound) && !feof(fdList)) {

            comp = cmp_cache_len(line_list, line_found, (size_t) len);

            if (comp == 0) {
                // read meta to line_number
                if (!feof(fdMeta)) {
                    while (!feof(fdMeta) && !feof(fdList) && meta.line_number < line_number) {
                        if (fgetl(fdMeta, BUFSIZ, line_meta) == -1) memset(line_meta, 0, BUFSIZ);
                        read_meta(line_meta, &meta);
                    }
                    if (line_number == meta.line_number) {
                        count_found += meta.line_count;
                    } else {
                        count_found++;
                    }
                } else {
                    count_found++;
                }

                // read found
                if (fgetl(fdFound, BIGBUFSIZ, line_found) == -1) memset(line_found, 0, BIGBUFSIZ);

                // read list
                if ((len = fgetl(fdList, BUFSIZ, line_list)) == -1) memset(line_list, 0, BUFSIZ);
                line_number++;
            }
            else if (comp > 0)
            {
                // shouldn't go there if foundfile contents comes ONLY from list
                // read found
                if (fgetl(fdFound, BIGBUFSIZ, line_found) == -1) memset(line_found, 0, BIGBUFSIZ);
            }
            else if (comp < 0)
            {
                // read list
                if ((len = fgetl(fdList, BUFSIZ, line_list)) == -1) memset(line_list, 0, BUFSIZ);
                line_number++;
            }
        }
        
        fclose(fdFound);

        printf("%d\n", count_found);
    }
    // endregion Count found

    // region Dict generation
    if (type == 0) { // dict

        // read found
        if (fgetl(fdFound, BUFSIZ, line_found) == -1) memset(line_found, 0, BUFSIZ);
        line_number++;

        while (!feof(fdFound)) {

            // read list
            if ((len = fgetl(fdList, BUFSIZ, line_list)) == -1) memset(line_list, 0, BUFSIZ);

            // read meta to line_number
            if (!feof(fdMeta)) {
                while (!feof(fdMeta) && meta.line_number < line_number) {
                    if (fgetl(fdMeta, BUFSIZ, line_meta) == -1) memset(line_meta, 0, BUFSIZ);
                    read_meta(line_meta, &meta);
                }
                if (line_number == meta.line_number) {
                    lines_to_print = meta.line_count;
                } else {
                    lines_to_print = 1;
                }
            } else {
                lines_to_print = 1;
            }

            comp = cmp_cache_len(line_list, line_found, (size_t) len);

            if (comp == 0) {
                for (i = 0; i < lines_to_print; i++) {
                    puts(line_found + len + 1);
                }
                // read found
                if (fgetl(fdFound, BUFSIZ, line_found) == -1) memset(line_found, 0, BUFSIZ);
            }            
        }
        fclose(fdFound);
    }
    // endregion Dict generation

    // region List generation
    if (type == 2) { // list

        while (!feof(fdList)) {

            // read list
            if ((len = fgetl(fdList, BUFSIZ, line_list)) == -1)
            {
                memset(line_list, 0, BUFSIZ);
                break;
            }
            line_number++;

            // read meta to line_number
            if (!feof(fdMeta)) {
                while (!feof(fdMeta) && meta.line_number < line_number) {
                    if (fgetl(fdMeta, BUFSIZ, line_meta) == -1) memset(line_meta, 0, BUFSIZ);
                    read_meta(line_meta, &meta);
                }
                if (line_number == meta.line_number) {
                    lines_to_print = meta.line_count;
                } else {
                    lines_to_print = 1;
                }
            } else {
                lines_to_print = 1;
            }

            for (i = 0; i < lines_to_print; i++) {
                puts(line_list);
            }
        }
    }
    // endregion List generation

    // Close fd's
    fclose (fdList);
    fclose (fdMeta);

    return 0;
}
