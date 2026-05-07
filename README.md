# Lab 6 - Dynamic Queue Implementation

Procedural C/C++ implementation of four queue data structure variants using dynamically allocated doubly-linked lists. Each queue type demonstrates different insertion and deletion semantics while maintaining the same underlying memory structure.

**Course:** C/C++ Programming – Lab 6  
**Author:** David Nenita  
**Institution:** Technical University of Moldova (FAF)

---

## Overview

This branch (Variant_B) implements four distinct queue types on a unified linked-list base:

1. Simple Queue - FIFO order (enqueue rear, dequeue front)
2. Double-Ended Queue (Deque) - flexible insertion/deletion at both ends
3. Circular Queue - fixed-capacity ring buffer with overflow protection
4. Priority Queue - max-heap ordered by interrogative sentence count

All queue types operate on TextRecord elements and support complete CRUD operations through an interactive menu interface.

---

## File Structure

```
version_B/
├── main.c           Entry point and text analysis utility
├── menu.h           Interactive menu system (header-only)
├── queue.h          Queue implementation - all four types (header-only)
├── cdt.h            Custom data type: TextRecord
├── file_utils.h     File I/O helper functions
├── lab6_B           Compiled executable
└── README.md        Complete documentation
```

---

## Compilation and Execution

```bash
cd version_B/
gcc -Wall -Wextra -o lab6_B main.c
./lab6_B
```

Compilation flags used:
- `-Wall`: Enable all common compiler warnings
- `-Wextra`: Enable extra warnings for code quality

---

## Custom Data Type

The TextRecord structure (defined in cdt.h) stores:

- text: user-provided string (up to 1024 characters)
- interrog_count: number of interrogative sentences (ending with '?')
- second_len: character length of the second interrogative sentence
- source_file: origin hint (typically "keyboard" for input)

Sentences are detected by delimiters: period (.), exclamation mark (!), and question mark (?).

---

## Queue Operations

Both versions use the same `TextRecord` (CDT) structure defined in `cdt.h`:

```c
typedef struct {
    char   text[MAX_TEXT];              // Raw user input (1024 chars)
    int    interrog_count;              // # of interrogative sentences
    int    second_len;                  // Length of 2nd interrogative
    char   source_file[MAX_PATH];       // Origin file/source hint
} TextRecord;
```

**Metadata Analysis:**
- Interrogative sentences detected by ending `?`
- Sentences delimited by `.`, `!`, `?`
- Automatic analysis on record creation
- Metadata preserved in file storage

---

## 📋 Operations Supported

### Version A: Stack Operations

| Operation | Code | Description |
|-----------|------|-------------|
| Push | 1 | Add element to top |
| Pop | 2 | Remove from top |
| Peek | 3 | View top without removal |
| Display | 4 | Show all elements |
| Search by position | 5 | Retrieve element at index |
| Search by value | 6 | Find by text match |
| Delete at position | 7 | Remove at index |
| Load from file | 8 | Restore from binary |
| Display from file | 9 | Preview file contents |
| Clear stack | 10 | Empty stack |
| Lab-10 helper | 11 | Text analysis tool |

### Version B: Queue Operations (All Types)

## Queue Operations

All queue types support the following operations:

| Operation | Code | Description |
|-----------|------|-------------|
| Enqueue (rear) | 1 | Insert element at rear |
| Enqueue (front) | 2 | Insert at front (Deque only) |
| Dequeue (front) | 3 | Remove from front |
| Dequeue (rear) | 4 | Remove from rear (Deque only) |
| Peek front | 5 | View front element without removal |
| Peek rear | 6 | View rear element without removal |
| Display all | 7 | Show all elements with metadata |
| Search by position | 8 | Retrieve element at 1-based position |
| Search by text value | 9 | Find element by exact text match |
| Delete at position | 10 | Remove element at indexed position |
| Register/Save | 11 | Save queue to file (text or binary) |
| Load from binary | 12 | Restore queue from binary file |

Main menu option 5 launches the Input/Output utility for text analysis.

---

## Queue Types and Behavior

**Simple Queue**: Traditional FIFO discipline. Insert at rear, remove from front.

**Double-Ended Queue (Deque)**: Allows insertion and removal at both ends. Enables more flexible data access patterns.

**Circular Queue**: Fixed-capacity ring buffer using linked nodes. User specifies capacity at initialization. Enforces capacity limits with overflow protection.

**Priority Queue**: Maintains max-heap ordering by interrogative count. Elements with more question marks are prioritized and appear first during dequeue operations. Insertion automatically maintains sorted order.

---

## Input/Output Utility

The text analysis feature (main menu option 5) performs:

- Counts total interrogative sentences (sentences ending with ?)
- Locates the 2nd interrogative sentence and reports its character length
- Writes raw input to input.txt
- Writes analysis results to output.txt
- Displays results to console

Example output:

```
Number of interrogative sentences : 2
Length of 2nd interrogative sent. : 12 characters
```

---

## File I/O Support

Two file storage modes are available:

**Text Mode**: Human-readable format with labeled fields. Useful for inspection and debugging.

**Binary Mode**: Compact binary serialization of TextRecord structures. More efficient for large datasets.

Both modes preserve all queue data and metadata. Files can be loaded back into a queue of the same type.

---

## Menu Interface

The program presents an interactive menu system:

```
MAIN MENU
1. Work with Simple Queue
2. Work with Double-Ended Queue (Deque)
3. Work with Circular Queue
4. Work with Priority Queue
5. Input/Output (input.txt / output.txt)
0. Exit
```

After selecting a queue type, a sub-menu allows all standard queue operations.

---

## Implementation Details

Data Structures:

- QNode: Doubly-linked node containing TextRecord pointer and forward/backward pointers
- Queue: Handle structure with front/rear pointers, size counter, capacity limit, and type tag

Memory Management:

- All nodes and data allocated dynamically with malloc()
- Cleanup performed with free() on dequeue and queue destruction
- No memory leaks with proper operation

Internal Functions:

- _qnode_new(): Create and populate a new queue node
- _qnode_free(): Free node and associated data
- _link_rear(): Insert node at queue rear
- _link_front(): Insert node at queue front
- _unlink_front(): Remove and return front node
- _unlink_rear(): Remove and return rear node

---

## Performance Characteristics

| Operation | Time |
|-----------|------|
| Enqueue (Simple/Deque/Circular) | O(1) |
| Enqueue (Priority) | O(n) |
| Dequeue | O(1) |
| Peek | O(1) |
| Search by position | O(n) |
| Search by value | O(n) |

---

## Requirements

- GCC compiler with C support
- Standard POSIX environment or equivalent
- stdio.h, stdlib.h, string.h libraries

---

## Usage Example

Build and run:

```bash
cd version_B/
gcc -Wall -Wextra -o lab6_B main.c
./lab6_B
```

Sample session:

```
Choice: 2
Circular queue capacity (e.g. 5): 10
[Circular Queue created]

Choice: 1
Enter text (sentences end with . ! or ?): What is your name?
[Enqueued. Size: 1]

Choice: 1
Enter text (sentences end with . ! or ?): How are you? I am fine.
[Enqueued. Size: 2]

Choice: 7
Circular Queue, size=2
# | Text (first 45 chars) | IQ | 2ndL
1 | What is your name?   | 1  | 18
2 | How are you? I am fi | 1  | 8

Choice: 11
File path to save: myqueue.bin
Mode (0=text, 1=binary): 1
[Saved to: myqueue.bin (binary mode)]
```

---

## Author

David Nenita

Course: C/C++ Programming  
Institution: Technical University of Moldova (FAF)

For detailed information about all features, review the source code comments in queue.h, menu.h, and cdt.h.

