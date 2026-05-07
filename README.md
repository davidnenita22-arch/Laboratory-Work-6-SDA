# Lab 6 - Dynamic Stack Implementation

Procedural C/C++ implementation of a stack data structure using dynamically allocated singly-linked list nodes. Demonstrates memory management, LIFO semantics, and complete CRUD operations through an interactive menu interface.

**Course:** Data Structures and Algorithms
**Author:** David Nenita  
**Institution:** Technical University of Moldova (FAF)

---

## Overview

This implementation provides a traditional stack (Last-In-First-Out) based on a linked-list structure. All elements are TextRecord structures containing user text and metadata about interrogative sentences. The program offers comprehensive operations for stack manipulation with text and binary file I/O support.

---

## File Structure

```
Lab-6/
├── main.c           Entry point and text analysis utility
├── menu.h           Interactive menu system (header-only)
├── stack.h          Stack implementation (header-only)
├── cdt.h            Custom data type: TextRecord
├── file_utils.h     File I/O helper functions
├── lab6_A           Compiled executable
├── README.md        This file
└── version_B/       Separate queue implementation branch
```

---

## Compilation and Execution

```bash
gcc -Wall -Wextra -o lab6_A main.c
./lab6_A
```

Compilation flags:
- `-Wall`: Enable all common compiler warnings
- `-Wextra`: Enable extra warnings for code quality

---

## Custom Data Type

The TextRecord structure (defined in cdt.h) stores:

- text: user-provided string (up to 1024 characters)
- interrog_count: number of interrogative sentences (ending with ?)
- second_len: character length of the second interrogative sentence
- source_file: origin hint (typically "keyboard" for input)

Sentences are detected by delimiters: period (.), exclamation mark (!), and question mark (?).

---

## Stack Operations

Standard stack operations supported:

| Operation | Code | Description |
|-----------|------|-------------|
| Create/Reset | 1 | Initialize new empty stack |
| Push | 2 | Add element to top |
| Pop | 3 | Remove and return top element |
| Peek | 4 | View top element without removal |
| Display | 5 | Show all elements top to bottom |
| Search by position | 6 | Retrieve element at index from top |
| Search by text value | 7 | Find element by exact text match |
| Delete at position | 8 | Remove element at indexed position |
| Register/Save | 9 | Save stack to file (text or binary) |
| Load from binary | 10 | Restore stack from binary file |
| Input | 11 | Text analysis utility |

---

## Data Structure

Stack Node (defined in stack.h):

```c
typedef struct StackNode {
    TextRecord *data;        /* pointer to heap-allocated record */
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
    int size;
} Stack;
```

- StackNode: Singly-linked node with TextRecord pointer and next pointer
- Stack: Handle containing top pointer and size counter

---

## Implementation Details

Core Functions:

- stack_create(): Allocate and initialize empty stack
- stack_destroy(): Free all nodes and destroy stack
- stack_push(): Add element to top (O(1))
- stack_pop(): Remove and return top element (O(1))
- stack_peek(): View top without removal (O(1))
- stack_search_by_pos(): Find element at 1-based position from top (O(n))
- stack_search_by_value(): Find first element matching text (O(n))
- stack_delete_at(): Remove element at position (O(n))

Memory Management:

- TextRecord data copied on push (deep copy)
- Nodes allocated with malloc(), freed with free()
- All memory released on stack_destroy()
- No memory leaks with proper operation

---

## File I/O Support

Two storage modes available:

**Text Mode**: Human-readable format with labeled fields. Suitable for inspection and debugging.

**Binary Mode**: Direct binary serialization of TextRecord structures. More efficient for storage and loading.

Both modes preserve all stack data including metadata. Saved stacks can be loaded back for continued operations.

---

## Text Analysis Utility

Main menu option 11 provides text analysis functionality:

- Counts total interrogative sentences (ending with ?)
- Determines length of 2nd interrogative sentence
- Writes raw input to input.txt
- Writes analysis results to output.txt
- Displays results to console

Example output:

```
Number of interrogative sentences : 2
Length of 2nd interrogative sent. : 12 characters
```

---

## Menu Interface

Main menu options:

```
1. Create / Reset stack
2. Push record (from keyboard)
3. Pop record (remove top)
4. Peek top record (no removal)
5. Display full stack
6. Search by position
7. Search by text value
8. Delete element at position
9. Register (save) stack to file
10. Load stack from binary file
11. Write Lab-10 input.txt
0. Exit
```

---

## Performance Characteristics

| Operation | Time Complexity |
|-----------|-----------------|
| Push | O(1) |
| Pop | O(1) |
| Peek | O(1) |
| Search by position | O(n) |
| Search by text value | O(n) |
| Delete at position | O(n) |
| Display | O(n) |

---

## Usage Example

Build and execute:

```bash
gcc -Wall -Wextra -o lab6_A main.c
./lab6_A
```

Sample session:

```
Choice: 1
[New empty stack created]

Choice: 2
Enter text (sentences end with . ! or ?): What is your name?
[Pushed. Stack size: 1]

Choice: 2
Enter text (sentences end with . ! or ?): How are you? I am fine.
[Pushed. Stack size: 2]

Choice: 5
Stack size: 2
1: Text: How are you? I am fine.
   Interrogatives: 1
   2nd sent. length: 0
   Source file: keyboard

2: Text: What is your name?
   Interrogatives: 1
   2nd sent. length: 0
   Source file: keyboard

Choice: 3
[Popped record:]
   Text: How are you? I am fine.
   Interrogatives: 1
   2nd sent. length: 0
   Source file: keyboard

Choice: 9
File path to save: mystack.bin
Mode - 0=text, 1=binary: 1
[Stack saved: mystack.bin  (binary mode)]

Choice: 0
[Exiting...]
```

---

## Requirements

- GCC compiler with C support
- Standard POSIX environment or equivalent
- stdio.h, stdlib.h, string.h libraries

---

For complete implementation details, review source code comments in stack.h, menu.h, and cdt.h.
