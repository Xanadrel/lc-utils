#pragma GCC diagnostic ignored "-Wunused-result"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * Name....: utils.c
 * Autor...: atom/Xanadrel
 * Desc....: Various utility functions
 * License.: MIT
 */

typedef unsigned int uint;

off64_t super_chop (char *s, off64_t len)
{
    char *p = s + len - 1;

    while (len)
    {
        if (*p != '\n') break;
        *p-- = 0;
        len--;
    }

    while (len)
    {
        if (*p != '\r') break;
        *p-- = 0;
        len--;
    }

    return len;
}

int fgetl (FILE *stream, size_t sz, char *buf)
{
    if (feof (stream)) return -1;

    char *s = fgets (buf, sz, stream);

    if (s == NULL) return -1;

    size_t len = strlen (s);

    len = super_chop (s, len);

    return len;
}

off64_t fgetl_closest64 (FILE* fd, size_t buf_size, char *buf, off64_t offset, off64_t *extra_read)
{
    // seek to offset
    char tmp_buf[BUFSIZ];

    lseek64 (fileno(fd), offset, SEEK_SET);

    off64_t read_newline = 0;

    // search for newline
    if (read (fileno (fd), tmp_buf, buf_size) <= 0) return -1;

    while (tmp_buf[read_newline] != '\n')
    {
        read_newline++;
    }
    
    lseek64 (fileno (fd), offset + read_newline + 1, SEEK_SET);
    read (fileno (fd), tmp_buf, buf_size);
    
    off64_t len = 0;
    while (tmp_buf[len] != '\n' && tmp_buf[len] != '\r' && len < buf_size)
    {
        len++;
    }
    
    tmp_buf[len] = 0;
    memcpy (buf, tmp_buf, len +1);

    *extra_read = read_newline;

    return len;
}

off64_t fgetl_closest (FILE *stream, size_t buf_size, char *buf, off64_t offset, off64_t *extra_read)
{
    if (feof (stream)) return -1;

    // seek to offset
    fseek (stream, offset, SEEK_SET);

    off64_t read_newline = 0;

    // search for newline
    while (!feof (stream) && fgetc (stream) != '\n')
    {
        read_newline++;
    }

    // get the line
    char *s = fgets (buf, buf_size, stream);

    if (s == NULL) return -1;

    off64_t len = strlen (s);
    len = super_chop (s, len);

    *extra_read = read_newline;

    return len;
}
