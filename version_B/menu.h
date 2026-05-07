#ifndef MENU_H
#define MENU_H

#include "queue.h"
#include "file_utils.h"

void lab6_write_input(void);  /* defined in main.c */

/* utility */
static inline void clear_input_buffer(void)
{ int c; while ((c = getchar()) != '\n' && c != EOF); }

static inline int read_int(const char *prompt)
{
    int val;
    printf("%s", prompt);
    if (scanf("%d", &val) != 1) { val = -1; clear_input_buffer(); }
    else clear_input_buffer();
    return val;
}

/* banners */
static inline void print_banner(void)
{
    printf("\n");
    printf("\n");
    printf("    Version B : Dynamic QUEUE  \n");
    printf("\n");
}

static inline void print_type_menu(void)
{
    printf("\n  Select queue type:\n");
    printf("  1. Simple Queue\n");
    printf("  2. Double-Ended Queue (Deque)\n");
    printf("  3. Circular Queue\n");
    printf("  4. Priority Queue\n");
    printf("  0. Back\n");
    printf("  Choice: ");
}

static inline void run_queue_menu(Queue *q)
{
    int choice;
    while (1) {
        printf("\n  %s  (size=%d)\n",
               queue_type_name(q->type), q->size);
        printf("   1. Enqueue (at rear)\n");
        if (q->type == QUEUE_DEQUE)
            printf("   2. Enqueue at FRONT  [Deque only]\n");
        else
            printf("   2. (Deque only)\n");
        printf("   3. Dequeue (from front)\n");
        if (q->type == QUEUE_DEQUE)
            printf("   4. Dequeue from REAR [Deque only]\n");
        else
            printf("   4. (Deque only)\n");
        printf("   5. Peek front\n");
        printf("   6. Peek rear\n");
        printf("   7. Display all\n");
        printf("   8. Search by position\n");
        printf("   9. Search by text value\n");
        printf("  10. Delete element at position\n");
        printf("  11. Register (save) to file\n");
        printf("  12. Load from binary file\n");
        printf("   0. Back to main menu\n");
        printf("  Choice: ");

        if (scanf("%d", &choice) != 1) { clear_input_buffer(); continue; }
        clear_input_buffer();

        switch (choice) {

        case 1: {   /* enqueue rear */
            TextRecord rec; memset(&rec, 0, sizeof(rec));
            if (read_record_keyboard(&rec, "keyboard")) {
                if (queue_enqueue(q, &rec))
                    printf("  [Enqueued. Size: %d]\n", q->size);
            }
            break;
        }
        case 2: {   /* enqueue front (Deque) */
            if (q->type != QUEUE_DEQUE) { printf("  [Deque only]\n"); break; }
            TextRecord rec; memset(&rec, 0, sizeof(rec));
            if (read_record_keyboard(&rec, "keyboard")) {
                if (queue_enqueue_front(q, &rec))
                    printf("  [Enqueued at front. Size: %d]\n", q->size);
            }
            break;
        }
        case 3: {   /* dequeue front */
            TextRecord *r = queue_dequeue(q);
            if (r) { printf("  [Dequeued:]\n"); print_record(r); free(r); }
            break;
        }
        case 4: {   /* dequeue rear (Deque) */
            if (q->type != QUEUE_DEQUE) { printf("  [Deque only]\n"); break; }
            TextRecord *r = queue_dequeue_rear(q);
            if (r) { printf("  [Dequeued from rear:]\n"); print_record(r); free(r); }
            break;
        }
        case 5: {   /* peek front */
            TextRecord *r = queue_peek_front(q);
            if (r) { printf("  [Front:]\n"); print_record(r); }
            else    printf("  [Empty]\n");
            break;
        }
        case 6: {   /* peek rear */
            TextRecord *r = queue_peek_rear(q);
            if (r) { printf("  [Rear:]\n"); print_record(r); }
            else    printf("  [Empty]\n");
            break;
        }
        case 7:     /* display */
            queue_display(q);
            break;
        case 8: {   /* search by pos */
            int pos = read_int("  Position (1=front): ");
            TextRecord *r = queue_search_by_pos(q, pos);
            if (r) { printf("  [Found:]\n"); print_record(r); }
            else    printf("  [Not found]\n");
            break;
        }
        case 9: {   /* search by value */
            char txt[MAX_TEXT];
            printf("  Enter exact text: ");
            if (fgets(txt, MAX_TEXT, stdin)) {
                size_t l = strlen(txt);
                if (l > 0 && txt[l-1] == '\n') txt[l-1] = '\0';
                TextRecord *r = queue_search_by_value(q, txt);
                if (r) { printf("  [Found:]\n"); print_record(r); }
                else    printf("  [Not found]\n");
            }
            break;
        }
        case 10: {  /* delete at pos */
            int pos = read_int("  Position to delete (1=front): ");
            if (queue_delete_at(q, pos))
                printf("  [Deleted pos %d. Size: %d]\n", pos, q->size);
            break;
        }
        case 11: {  /* register */
            char path[PATH_BUF];
            read_path(path, PATH_BUF, "  File path to save: ");
            int bin = read_int("  Mode – 0=text, 1=binary: ");
            queue_register(q, path, bin);
            break;
        }
        case 12: {  /* load binary */
            char path[PATH_BUF];
            read_path(path, PATH_BUF, "  Binary file path to load: ");
            queue_load_binary(q, path);
            break;
        }
        case 0:
            return;
        default:
            printf("  [Unknown option]\n");
        }
    }
}
static inline void run_menu(void)
{
    /* Keep one active queue of each type */
    Queue *queues[5] = { NULL, NULL, NULL, NULL, NULL }; /* index 1..4 */
    int choice;
    print_banner();

    while (1) {
        printf("\n \n");
        printf("  │  MAIN MENU\n");
        printf("  │  1. Work with Simple Queue \n");
        printf("  │  2. Work with Double-Ended Queue (Deque)\n");
        printf("  │  3. Work with Circular Queue\n");
        printf("  │  4. Work with Priority Queue\n");
        printf("  │  5. Input/Output\n");
        printf("  │  0. Exit  \n");
        printf(" \n");
        printf("  Choice: ");

        if (scanf("%d", &choice) != 1) { clear_input_buffer(); continue; }
        clear_input_buffer();

        if (choice == 0) {
            printf("  [Cleaning up and exiting...]\n");
            for (int i = 1; i <= 4; i++)
                if (queues[i]) queue_destroy(queues[i]);
            return;
        }

        if (choice == 5) { lab6_write_input(); continue; }

        if (choice < 1 || choice > 4) { printf("  [Invalid]\n"); continue; }

        QueueType type = (QueueType)choice;

        /* Create if not yet initialised */
        if (!queues[type]) {
            int cap = 0;
            if (type == QUEUE_CIRCULAR) {
                cap = read_int("  Circular queue capacity: ");
                if (cap <= 0) cap = 5;
            }
            queues[type] = queue_create(type, cap);
            if (queues[type])
                printf("  [%s created]\n", queue_type_name(type));
        }

        if (queues[type])
            run_queue_menu(queues[type]);
    }
}

#endif 
