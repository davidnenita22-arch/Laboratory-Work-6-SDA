/*=============================================================================
 *  FILE:    file_utils.h
 *  PURPOSE: Helper functions to create / open / reopen / delete *.txt and
 *           *.bin working files.  The full path is always read from keyboard.
 *============================================================================*/

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
  #include <io.h>
  #define F_OK 0
  #define access _access
#else
  #include <unistd.h>
#endif

#define PATH_BUF 512

/* Read a file path from the keyboard */
static inline void read_path(char *buf, int bufsz, const char *prompt)
{
    printf("%s", prompt);
    if (!fgets(buf, bufsz, stdin)) { buf[0] = '\0'; return; }
    size_t len = strlen(buf);
    if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';
}

/* Create a new file (warn if exists, then overwrite) */
static inline FILE *file_create(const char *path)
{
    if (access(path, F_OK) == 0)
        printf("  [Warning: %s exists – will be overwritten]\n", path);
    FILE *fp = fopen(path, "w");
    if (!fp) perror("file_create: fopen");
    else printf("  [Created: %s]\n", path);
    return fp;
}

/* Open for appending */
static inline FILE *file_open_append(const char *path)
{
    FILE *fp = fopen(path, "a");
    if (!fp) perror("file_open_append: fopen");
    return fp;
}

/* Open for reading */
static inline FILE *file_open_read(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp) perror("file_open_read: fopen");
    return fp;
}

/* Delete a file */
static inline int file_delete(const char *path)
{
    if (remove(path) == 0) { printf("  [Deleted: %s]\n", path); return 1; }
    perror("file_delete: remove");
    return 0;
}

#endif /* FILE_UTILS_H */
