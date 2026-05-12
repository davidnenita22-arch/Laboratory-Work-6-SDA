#ifndef CDT_H
#define CDT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TEXT   2048
#define MAX_PATH   512

typedef struct {
    char text[MAX_TEXT];   /* raw text string entered by the user  */
    int interrog_count;   /* number of interrogative sentences (end '?')  */
    int second_len;       /* length of the 2nd interrogative sentence */
    char source_file[MAX_PATH]; /* path of the file the record came from */
} TextRecord;

/* Utility: count interrogative sentences & find 2nd one's length */
static inline void analyse_text(TextRecord *r)
{
    if (!r) return;
    int interrog = 0;
    int second_len = -1;          /* -1  -> no 2nd interrogative found  */
    int i = 0;
    int n = (int)strlen(r->text);

    /* Walk through the text collecting sentence end positions */
    int sent_start = 0;

    while (i <= n) {
        char c = r->text[i];
        int  is_end = (c == '.' || c == '!' || c == '?' || c == '\0');

        if (is_end) {
            if (c == '?') {
                interrog++;
                if (interrog == 2) {
                    /* length = characters from sent_start to i  (inclusive '?') */
                    second_len = i - sent_start + 1;
                }
            }
            /* skip whitespace after sentence end to find next start */
            int j = i + 1;
            while (j < n && (r->text[j] == ' ' || r->text[j] == '\t' ||
                              r->text[j] == '\n' || r->text[j] == '\r'))
                j++;
            sent_start = j;
            i = j;
        } else {
            i++;
        }
    }

    r->interrog_count = interrog;
    r->second_len = (second_len >= 0) ? second_len : 0;
}

/* Read a TextRecord from keyboard + perform analysis */
static inline int read_record_keyboard(TextRecord *r, const char *src_hint)
{
    if (!r) return 0;
    printf("  Enter text (sentences end with . ! or ?): ");
    if (!fgets(r->text, MAX_TEXT, stdin)) return 0;

    /* strip trailing newline */
    size_t len = strlen(r->text);
    if (len > 0 && r->text[len-1] == '\n') r->text[len-1] = '\0';

    strncpy(r->source_file,
            src_hint ? src_hint : "keyboard",
            MAX_PATH - 1);
    r->source_file[MAX_PATH-1] = '\0';

    analyse_text(r);
    return 1;
}

/* Write a TextRecord to an already-open FILE* */
static inline void write_record_file(FILE *fp, const TextRecord *r)
{
    if (!fp || !r) return;
    fprintf(fp, "TEXT : %s\n", r->text);
    fprintf(fp, "INTERROG: %d\n", r->interrog_count);
    fprintf(fp, "2ND_LEN : %d\n", r->second_len);
    fprintf(fp, "SOURCE : %s\n", r->source_file);
    fprintf(fp, "---\n");
}

/* Print a TextRecord to stdout */
static inline void print_record(const TextRecord *r)
{
    if (!r) return;
    printf("  Text : %s\n", r->text);
    printf("  Interrogatives  : %d\n", r->interrog_count);
    printf("  2nd sent. length: %d\n", r->second_len);
    printf("  Source file : %s\n", r->source_file);
}

#endif
