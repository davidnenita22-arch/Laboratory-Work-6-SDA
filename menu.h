#ifndef MENU_H
#define MENU_H

#include "stack.h"
#include "file_utils.h"

/* forward declaration of the input.txt writer (defined in main.c) */
void lab10_write_input(void);

static inline void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static inline int read_int(const char *prompt)
{
    int val;
    printf("%s", prompt);
    if (scanf("%d", &val) != 1) { val = -1; clear_input_buffer(); }
    else clear_input_buffer();
    return val;
}

static inline void print_banner(void)
{
    printf("\n");
    printf(" \n");
    printf("      Version A : Dynamic STACK (List ADS)  \n");
    printf(" \n");
}

static inline void print_menu(void)
{
    printf("\n");
    printf("  ┌─────────────────────────────────────────────────┐\n");
    printf("  │  1. Create / Reset stack                        │\n");
    printf("  │  2. Push record (from keyboard)                 │\n");
    printf("  │  3. Pop  record (remove top)                    │\n");
    printf("  │  4. Peek top record (no removal)                │\n");
    printf("  │  5. Display full stack                          │\n");
    printf("  │  6. Search by position                          │\n");
    printf("  │  7. Search by text value                        │\n");
    printf("  │  8. Delete element at position                  │\n");
    printf("  │  9. Register (save) stack to file               │\n");
    printf("  │ 10. Load stack from binary file                 │\n");
    printf("  │ 11. Write Lab-10 input.txt                      │\n");
    printf("  │  0. Exit                                        │\n");
    printf("  └─────────────────────────────────────────────────┘\n");
    printf("  Choice: ");
}

static inline void run_menu(void)
{
    Stack *stack = NULL;
    int choice;

    print_banner();

    while (1) {
        if (!stack) printf("\n  [No active stack – choose 1 to create one]\n");
        print_menu();
        if (scanf("%d", &choice) != 1) { clear_input_buffer(); continue; }
        clear_input_buffer();

        switch (choice) {

        /* 1. Create / Reset */
        case 1:
            if (stack) { stack_destroy(stack); printf("  [Previous stack destroyed]\n"); }
            stack = stack_create();
            if (stack) printf("  [New empty stack created]\n");
            break;

        /* 2. Push */
        case 2:
            if (!stack) { printf("  [Create a stack first (option 1)]\n"); break; }
            {
                TextRecord rec;
                memset(&rec, 0, sizeof(rec));
                if (read_record_keyboard(&rec, "keyboard")) {
                    if (stack_push(stack, &rec))
                        printf("  [Pushed. Stack size: %d]\n", stack->size);
                }
            }
            break;

        /* 3. Pop */
        case 3:
            if (!stack) { printf("  [No stack]\n"); break; }
            {
                TextRecord *r = stack_pop(stack);
                if (r) {
                    printf("  [Popped record:]\n");
                    print_record(r);
                    free(r);
                }
            }
            break;

        /* 4. Peek */
        case 4:
            if (!stack) { printf("  [No stack]\n"); break; }
            {
                TextRecord *r = stack_peek(stack);
                if (r) { printf("  [Top record (not removed):]\n"); print_record(r); }
                else     printf("  [Stack is empty]\n");
            }
            break;

        /* 5. Display */
        case 5:
            if (!stack) { printf("  [No stack]\n"); break; }
            stack_display(stack);
            break;

        /* 6. Search by position */
        case 6:
            if (!stack) { printf("  [No stack]\n"); break; }
            {
                int pos = read_int("  Enter position (1 = top): ");
                TextRecord *r = stack_search_by_pos(stack, pos);
                if (r) { printf("  [Found at pos %d:]\n", pos); print_record(r); }
                else    printf("  [Not found at pos %d]\n", pos);
            }
            break;

        /* 7. Search by value */
        case 7:
            if (!stack) { printf("  [No stack]\n"); break; }
            {
                char txt[MAX_TEXT];
                printf("  Enter exact text to find: ");
                if (fgets(txt, MAX_TEXT, stdin)) {
                    size_t l = strlen(txt);
                    if (l > 0 && txt[l-1] == '\n') txt[l-1] = '\0';
                    TextRecord *r = stack_search_by_value(stack, txt);
                    if (r) { printf("  [Found:]\n"); print_record(r); }
                    else    printf("  [Not found]\n");
                }
            }
            break;

        /* 8. Delete at position */
        case 8:
            if (!stack) { printf("  [No stack]\n"); break; }
            {
                int pos = read_int("  Enter position to delete (1 = top): ");
                if (stack_delete_at(stack, pos))
                    printf("  [Deleted pos %d. Stack size: %d]\n", pos, stack->size);
            }
            break;

        /* 9. Register */
        case 9:
            if (!stack) { printf("  [No stack]\n"); break; }
            {
                char path[PATH_BUF];
                read_path(path, PATH_BUF, "  File path to save: ");
                int bin = read_int("  Mode – 0=text, 1=binary: ");
                stack_register(stack, path, bin);
            }
            break;

        /* 10. Load from binary */
        case 10:
            if (!stack) { printf("  Create a stack first (option 1)\n"); break; }
            {
                char path[PATH_BUF];
                read_path(path, PATH_BUF, "  Binary file path to load: ");
                stack_load_binary(stack, path);
            }
            break;

        /* 11. Lab-10 helper */
        case 11:
            lab10_write_input();
            break;

        /* 0. Exit */
        case 0:
            printf("  [Exiting – cleaning up...]\n");
            if (stack) stack_destroy(stack);
            return;

        default:
            printf("  [Unknown option]\n");
        }
    }
}

#endif 
