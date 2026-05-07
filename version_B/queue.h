/*=============================================================================
 *  FILE:    queue.h
 *  PURPOSE: All four Queue variants based on a doubly-linked List ADS:
 *             1. Simple Queue          (FIFO)
 *             2. Double-Ended Queue    (Deque – insert/delete both ends)
 *             3. Circular Queue        (fixed-capacity ring using linked nodes)
 *             4. Priority Queue        (min-heap property via interrog_count)
 *
 *  Every variant uses the same QNode / Queue struct; the queue_type field
 *  selects which invariant to enforce at runtime.
 *============================================================================*/

#ifndef QUEUE_H
#define QUEUE_H

#include "cdt.h"

/* ── Queue type tag ──────────────────────────────────────────────────────── */
typedef enum {
    QUEUE_SIMPLE    = 1,
    QUEUE_DEQUE     = 2,
    QUEUE_CIRCULAR  = 3,
    QUEUE_PRIORITY  = 4
} QueueType;

/* ── Doubly-linked node ──────────────────────────────────────────────────── */
typedef struct QNode {
    TextRecord  *data;
    struct QNode *prev;
    struct QNode *next;
} QNode;

/* ── Queue handle ────────────────────────────────────────────────────────── */
typedef struct {
    QNode     *front;      /* for Simple/Circular/Priority: dequeue end      */
    QNode     *rear;       /* enqueue end                                     */
    int        size;
    int        capacity;   /* >0 only for QUEUE_CIRCULAR; 0 = unlimited      */
    QueueType  type;
} Queue;

/* ═══════════════════════════════════════════════════════════════════════════
 *  INTERNAL helpers
 * ═══════════════════════════════════════════════════════════════════════════*/
static inline QNode *_qnode_new(const TextRecord *rec)
{
    TextRecord *copy = (TextRecord *)malloc(sizeof(TextRecord));
    if (!copy) { perror("_qnode_new: malloc record"); return NULL; }
    *copy = *rec;
    QNode *n = (QNode *)malloc(sizeof(QNode));
    if (!n) { perror("_qnode_new: malloc node"); free(copy); return NULL; }
    n->data = copy;
    n->prev = n->next = NULL;
    return n;
}

static inline void _qnode_free(QNode *n)
{
    if (!n) return;
    free(n->data);
    free(n);
}

/* Link node at rear */
static inline void _link_rear(Queue *q, QNode *n)
{
    n->prev = q->rear;
    n->next = NULL;
    if (q->rear) q->rear->next = n;
    else         q->front = n;
    q->rear = n;
    q->size++;
}

/* Link node at front */
static inline void _link_front(Queue *q, QNode *n)
{
    n->next = q->front;
    n->prev = NULL;
    if (q->front) q->front->prev = n;
    else          q->rear = n;
    q->front = n;
    q->size++;
}

/* Unlink and return front node (caller frees) */
static inline QNode *_unlink_front(Queue *q)
{
    if (!q->front) return NULL;
    QNode *n   = q->front;
    q->front   = n->next;
    if (q->front) q->front->prev = NULL;
    else          q->rear = NULL;
    n->next = n->prev = NULL;
    q->size--;
    return n;
}

/* Unlink and return rear node (caller frees) */
static inline QNode *_unlink_rear(Queue *q)
{
    if (!q->rear) return NULL;
    QNode *n = q->rear;
    q->rear  = n->prev;
    if (q->rear) q->rear->next = NULL;
    else         q->front = NULL;
    n->next = n->prev = NULL;
    q->size--;
    return n;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  CREATE / DESTROY
 * ═══════════════════════════════════════════════════════════════════════════*/
static inline Queue *queue_create(QueueType type, int capacity)
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    if (!q) { perror("queue_create: malloc"); return NULL; }
    q->front    = q->rear = NULL;
    q->size     = 0;
    q->type     = type;
    q->capacity = (type == QUEUE_CIRCULAR && capacity > 0) ? capacity : 0;
    return q;
}

static inline void queue_destroy(Queue *q)
{
    if (!q) return;
    QNode *cur = q->front;
    while (cur) {
        QNode *tmp = cur->next;
        _qnode_free(cur);
        cur = tmp;
    }
    free(q);
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  ENQUEUE  – inserts element respecting the queue's type invariant
 *
 *   Simple    → insert at rear
 *   Deque     → insert at rear  (default; caller may use enqueue_front)
 *   Circular  → insert at rear, reject if full
 *   Priority  → insert at sorted position (ascending interrog_count = min)
 * ═══════════════════════════════════════════════════════════════════════════*/
static inline int queue_enqueue(Queue *q, const TextRecord *rec)
{
    if (!q || !rec) return 0;

    /* Circular capacity check */
    if (q->type == QUEUE_CIRCULAR && q->capacity > 0 && q->size >= q->capacity) {
        printf("  [Circular queue full (capacity=%d)]\n", q->capacity);
        return 0;
    }

    QNode *n = _qnode_new(rec);
    if (!n) return 0;

    if (q->type == QUEUE_PRIORITY) {
        /* Insert before the first node whose priority (interrog_count) > rec */
        int pri = rec->interrog_count;
        QNode *cur = q->front;
        while (cur && cur->data->interrog_count <= pri) cur = cur->next;

        if (!cur) {
            /* insert at rear */
            _link_rear(q, n);
        } else if (!cur->prev) {
            /* insert at front */
            _link_front(q, n);
        } else {
            /* insert before cur */
            QNode *p  = cur->prev;
            n->prev   = p;
            n->next   = cur;
            p->next   = n;
            cur->prev = n;
            q->size++;
        }
    } else {
        _link_rear(q, n);
    }
    return 1;
}

/* Deque: enqueue at front */
static inline int queue_enqueue_front(Queue *q, const TextRecord *rec)
{
    if (!q || !rec) return 0;
    if (q->type != QUEUE_DEQUE) {
        printf("  [enqueue_front only valid for DEQUE]\n"); return 0;
    }
    QNode *n = _qnode_new(rec);
    if (!n) return 0;
    _link_front(q, n);
    return 1;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  DEQUEUE
 *   Simple / Circular / Priority → remove from front
 *   Deque  → also expose dequeue_rear
 * ═══════════════════════════════════════════════════════════════════════════*/
static inline TextRecord *queue_dequeue(Queue *q)
{
    if (!q || !q->front) { printf("  [Queue is empty]\n"); return NULL; }
    QNode *n = _unlink_front(q);
    TextRecord *r = n->data;
    n->data = NULL;
    free(n);
    return r; /* caller must free() */
}

static inline TextRecord *queue_dequeue_rear(Queue *q)
{
    if (!q || q->type != QUEUE_DEQUE) {
        printf("  [dequeue_rear only valid for DEQUE]\n"); return NULL;
    }
    if (!q->rear) { printf("  [Queue is empty]\n"); return NULL; }
    QNode *n = _unlink_rear(q);
    TextRecord *r = n->data;
    n->data = NULL;
    free(n);
    return r;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  PEEK (front / rear)
 * ═══════════════════════════════════════════════════════════════════════════*/
static inline TextRecord *queue_peek_front(const Queue *q)
{
    return (q && q->front) ? q->front->data : NULL;
}
static inline TextRecord *queue_peek_rear(const Queue *q)
{
    return (q && q->rear) ? q->rear->data : NULL;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  SEARCH
 * ═══════════════════════════════════════════════════════════════════════════*/
static inline TextRecord *queue_search_by_pos(const Queue *q, int pos)
{
    if (!q || pos < 1 || pos > q->size) return NULL;
    QNode *cur = q->front;
    for (int i = 1; i < pos && cur; i++) cur = cur->next;
    return cur ? cur->data : NULL;
}

static inline TextRecord *queue_search_by_value(const Queue *q, const char *txt)
{
    if (!q || !txt) return NULL;
    QNode *cur = q->front;
    while (cur) {
        if (strcmp(cur->data->text, txt) == 0) return cur->data;
        cur = cur->next;
    }
    return NULL;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  DELETE element at 1-based position
 * ═══════════════════════════════════════════════════════════════════════════*/
static inline int queue_delete_at(Queue *q, int pos)
{
    if (!q || pos < 1 || pos > q->size) {
        printf("  [Invalid position]\n"); return 0;
    }
    QNode *cur = q->front;
    for (int i = 1; i < pos && cur; i++) cur = cur->next;
    if (!cur) return 0;

    /* Unlink */
    if (cur->prev) cur->prev->next = cur->next;
    else           q->front        = cur->next;
    if (cur->next) cur->next->prev = cur->prev;
    else           q->rear         = cur->prev;
    q->size--;
    _qnode_free(cur);
    return 1;
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  DISPLAY
 * ═══════════════════════════════════════════════════════════════════════════*/
static inline const char *queue_type_name(QueueType t)
{
    switch (t) {
    case QUEUE_SIMPLE:   return "Simple Queue";
    case QUEUE_DEQUE:    return "Double-Ended Queue";
    case QUEUE_CIRCULAR: return "Circular Queue";
    case QUEUE_PRIORITY: return "Priority Queue";
    default:             return "Unknown";
    }
}

static inline void queue_display(const Queue *q)
{
    if (!q) { printf("  [No queue]\n"); return; }
    printf("  %s, size=%d", queue_type_name(q->type), q->size);
    if (q->type == QUEUE_CIRCULAR && q->capacity > 0)
        printf(", capacity=%d", q->capacity);
    printf("\n");
    if (!q->front) { printf("  [Empty]\n"); return; }
    printf("  %-4s  %-45s  %5s  %5s\n",
           "#", "Text (first 45 chars)", "IQ", "2ndL");
    printf("  %s\n","----------------------------------------------------------------------");
    QNode *cur = q->front;
    int pos = 1;
    while (cur) {
        char preview[46];
        strncpy(preview, cur->data->text, 45);
        preview[45] = '\0';
        printf("  %-4d  %-45s  %5d  %5d\n",
               pos++, preview,
               cur->data->interrog_count,
               cur->data->second_len);
        cur = cur->next;
    }
}

/* ═══════════════════════════════════════════════════════════════════════════
 *  REGISTER (save) – text or binary
 * ═══════════════════════════════════════════════════════════════════════════*/
static inline int queue_register(const Queue *q, const char *path, int binary)
{
    if (!q || !path) return 0;

    if (binary) {
        FILE *fp = fopen(path, "wb");
        if (!fp) { perror("queue_register (binary)"); return 0; }
        int t = (int)q->type;
        fwrite(&t,        sizeof(int), 1, fp);
        fwrite(&q->size,  sizeof(int), 1, fp);
        fwrite(&q->capacity, sizeof(int), 1, fp);
        QNode *cur = q->front;
        while (cur) {
            fwrite(cur->data, sizeof(TextRecord), 1, fp);
            cur = cur->next;
        }
        fclose(fp);
    } else {
        FILE *fp = fopen(path, "w");
        if (!fp) { perror("queue_register (text)"); return 0; }
        fprintf(fp, "=== %s  REGISTER (size=%d) ===\n\n",
                queue_type_name(q->type), q->size);
        QNode *cur = q->front;
        int pos = 1;
        while (cur) {
            fprintf(fp, "[%d]\n", pos++);
            write_record_file(fp, cur->data);
            cur = cur->next;
        }
        fclose(fp);
    }
    printf("  [Saved to: %s  (%s mode)]\n", path, binary ? "binary" : "text");
    return 1;
}

/* ── Load from binary ──────────────────────────────────────────────────── */
static inline int queue_load_binary(Queue *q, const char *path)
{
    if (!q || !path) return 0;
    FILE *fp = fopen(path, "rb");
    if (!fp) { perror("queue_load_binary"); return 0; }

    int t, count, cap;
    fread(&t,     sizeof(int), 1, fp);
    fread(&count, sizeof(int), 1, fp);
    fread(&cap,   sizeof(int), 1, fp);

    for (int i = 0; i < count; i++) {
        TextRecord rec;
        fread(&rec, sizeof(TextRecord), 1, fp);
        queue_enqueue(q, &rec);
    }
    fclose(fp);
    printf("  [Loaded %d record(s) from: %s]\n", count, path);
    return 1;
}

#endif /* QUEUE_H */
