#ifndef STACK_H
#define STACK_H

#include "cdt.h"

typedef struct StackNode {
    TextRecord *data;        /* pointer to heap-allocated record */
    struct StackNode *next;
} StackNode;


typedef struct {
    StackNode *top;
    int size;
} Stack;


/* Allocate and initialise an empty stack */
static inline Stack *stack_create(void)
{
    Stack *s = (Stack *)malloc(sizeof(Stack));
    if (!s) { perror("stack_create: malloc"); return NULL; }
    s->top = NULL;
    s->size = 0;
    return s;
}

/* Free every node (and each node's TextRecord) then the stack itself */
static inline void stack_destroy(Stack *s)
{
    if (!s) return;
    StackNode *cur = s->top;
    while (cur) {
        StackNode *tmp = cur->next;
        free(cur->data);   /* free the TextRecord  */
        free(cur);         /* free the node        */
        cur = tmp;
    }
    free(s);
}

/* Push a *copy* of rec onto the stack */
static inline int stack_push(Stack *s, const TextRecord *rec)
{
    if (!s || !rec) return 0;

    TextRecord *copy = (TextRecord *)malloc(sizeof(TextRecord));
    if (!copy) { perror("stack_push: malloc TextRecord"); return 0; }
    *copy = *rec;                         /* shallow copy (all fields are by-value) */

    StackNode *node = (StackNode *)malloc(sizeof(StackNode));
    if (!node) { perror("stack_push: malloc StackNode"); free(copy); return 0; }
    node->data = copy;
    node->next = s->top;
    s->top = node;
    s->size++;
    return 1;
}

/* Pop and return pointer to data (caller must free) – NULL if empty */
static inline TextRecord *stack_pop(Stack *s)
{
    if (!s || !s->top) { printf("  [Stack is empty]\n"); return NULL; }
    StackNode  *node = s->top;
    TextRecord *data = node->data;
    s->top = node->next;
    free(node);
    s->size--;
    return data;   /* caller responsible for free() */
}

/* Peek at top without removing */
static inline TextRecord *stack_peek(const Stack *s)
{
    if (!s || !s->top) return NULL;
    return s->top->data;
}

/* Search by 1-based position from top; returns pointer or NULL */
static inline TextRecord *stack_search_by_pos(const Stack *s, int pos)
{
    if (!s || pos < 1 || pos > s->size) return NULL;
    StackNode *cur = s->top;
    for (int i = 1; i < pos && cur; i++) cur = cur->next;
    return cur ? cur->data : NULL;
}

/* Search by text value (first match); returns pointer or NULL */
static inline TextRecord *stack_search_by_value(const Stack *s, const char *txt)
{
    if (!s || !txt) return NULL;
    StackNode *cur = s->top;
    while (cur) {
        if (strcmp(cur->data->text, txt) == 0) return cur->data;
        cur = cur->next;
    }
    return NULL;
}

static inline int stack_delete_at(Stack *s, int pos)
{
    if (!s || pos < 1 || pos > s->size) {
        printf("[Invalid position]\n");
        return 0;
    }
    /* Special case: deleting top */
    if (pos == 1) {
        TextRecord *r = stack_pop(s);
        if (r) free(r);
        return 1;
    }
    StackNode *prev = s->top;
    for (int i = 1; i < pos - 1 && prev; i++) prev = prev->next;
    if (!prev || !prev->next) return 0;
    StackNode *target = prev->next;
    prev->next = target->next;
    free(target->data);
    free(target);
    s->size--;
    return 1;
}

static inline void stack_display(const Stack *s)
{
    if (!s || !s->top) { printf("  [Stack is empty]\n"); return; }
    printf("Stack (top → bottom), %d element(s):\n", s->size);
    printf("%-4s  %-45s  %5s  %5s\n",
           "#", "Text (first 45 chars)", "IQ", "2ndL");
    printf("%s\n", "----------------------------------------------------------------------");
    StackNode *cur = s->top;
    int pos = 1;
    while (cur) {
        char preview[46];
        strncpy(preview, cur->data->text, 45);
        preview[45] = '\0';
        printf("%-4d  %-45s  %5d  %5d\n",
               pos++, preview,
               cur->data->interrog_count,
               cur->data->second_len);
        cur = cur->next;
    }
}

static inline int stack_register(const Stack *s, const char *path, int binary)
{
    if (!s || !path) return 0;

    if (binary) {
        /* binary mode*/
        FILE *fp = fopen(path, "wb");
        if (!fp) { perror("stack_register (binary): fopen"); return 0; }
        fwrite(&s->size, sizeof(int), 1, fp);
        StackNode *cur = s->top;
        while (cur) {
            fwrite(cur->data, sizeof(TextRecord), 1, fp);
            cur = cur->next;
        }
        fclose(fp);
    } else {
        /* text mode*/
        FILE *fp = fopen(path, "w");
        if (!fp) { perror("stack_register (text): fopen"); return 0; }
        fprintf(fp, "=== STACK REGISTER  (size=%d) ===\n\n", s->size);
        StackNode *cur = s->top;
        int pos = 1;
        while (cur) {
            fprintf(fp, "[%d]\n", pos++);
            write_record_file(fp, cur->data);
            cur = cur->next;
        }
        fclose(fp);
    }
    printf("Saved to: %s  (%s mode)]\n", path, binary ? "binary" : "text");
    return 1;
}

static inline int stack_load_binary(Stack *s, const char *path)
{
    if (!s || !path) return 0;
    FILE *fp = fopen(path, "rb");
    if (!fp) { perror("stack_load_binary: fopen"); return 0; }

    int count = 0;
    fread(&count, sizeof(int), 1, fp);

    /* Read into a temp array then push in reverse so order is preserved */
    TextRecord *arr = (TextRecord *)malloc(sizeof(TextRecord) * count);
    if (!arr) { fclose(fp); return 0; }
    fread(arr, sizeof(TextRecord), count, fp);
    fclose(fp);

    for (int i = count - 1; i >= 0; i--)
        stack_push(s, &arr[i]);

    free(arr);
    printf("  [Loaded %d record(s) from: %s]\n", count, path);
    return 1;
}

#endif
