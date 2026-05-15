#include "menu.h"  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Count interrogative sentences (ending '?') in a string */
static int count_interrogative(const char *str)
{
    int count = 0;
    for (int i = 0; str[i]; i++)
        if (str[i] == '?') count++;
    return count;
}

/* Return the length of the k-th interrogative sentence (1-based),
   or 0 if it doesn't exist.  Sentences are split on '.', '!', '?' */
static int kth_interrogative_length(const char *str, int k)
{
    int found = 0;
    int i = 0;
    int n = (int)strlen(str);
    int start = 0;

    while (i <= n) {
        char c = str[i];
        int is_end = (c == '.' || c == '!' || c == '?' || c == '\0');
        if (is_end) {
            if (c == '?') {
                found++;
                if (found == k) return i - start + 1;
            }
            int j = i + 1;
            while (j < n && (str[j] == ' ' || str[j] == '\t' ||
                             str[j] == '\n' || str[j] == '\r')) j++;
            start = j;
            i = j;
        } else { i++; }
    }
    return 0;
}

/* write input.txt and produce output.txt */
void lab6_write_input(void)
{
    printf("  Creating input.txt in current directory...\n");

    FILE *fin = fopen("input.txt", "w");
    if (!fin) { perror("  Cannot create input.txt"); return; }

    printf("  Enter your text string for input.txt:\n  > ");
    char buf[MAX_TEXT];
    if (!fgets(buf, MAX_TEXT, stdin)) { fclose(fin); return; }
    fputs(buf, fin);
    fclose(fin);
    printf("  [input.txt written]\n");

    /* Strip newline for analysis */
    size_t len = strlen(buf);
    if (len > 0 && buf[len-1] == '\n') buf[len-1] = '\0';

    /* Analysis */
    int  total = count_interrogative(buf);
    int  second_len = kth_interrogative_length(buf, 2);

    /* Screen output */
    printf("\n  ─ Analysis results ─\n");
    printf("  Number of interrogative sentences : %d\n", total);
    if (second_len > 0)
        printf("  Length of 2nd interrogative sent. : %d characters\n", second_len);
    else
        printf("  2nd interrogative sentence: not found\n");

    /* Write output.txt */
    FILE *fout = fopen("output.txt", "w");
    if (!fout) { perror("  Cannot create output.txt"); return; }
    fprintf(fout, "Interrogative sentences count : %d\n", total);
    fprintf(fout, "Length of 2nd interrogative : %d\n", second_len);
    fclose(fout);
    printf("  [output.txt written]\n");
    printf("  ────────────────────────────────────────────────────\n");
}
int main(void)
{
    run_menu();
    return 0;
}
