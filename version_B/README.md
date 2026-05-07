# Lab 6 - Version B: Dynamic Queue Variants

## Overview

This is **Version B** of the Lab 6 DSA (Data Structures & Algorithms) project. It implements four different queue data structure types using a dynamically allocated doubly-linked list as the underlying Abstract Data Structure (ADS).

## Features

Version B provides an interactive menu-driven program supporting all four queue variants:

### Queue Types Implemented

1. **Simple Queue (FIFO)**
   - Enqueue at rear, dequeue from front
   - First-in, first-out behavior

2. **Double-Ended Queue (Deque)**
   - Enqueue/dequeue at both front and rear
   - More flexible access patterns

3. **Circular Queue**
   - Fixed-capacity ring buffer (linked-list based)
   - User-specified capacity at initialization
   - Prevents overflow with capacity checking

4. **Priority Queue**
   - Min-heap ordering by interrogative count
   - Highest interrogative count sentences appear first (max-heap)
   - Sorted insertion based on metadata

## Custom Data Type (CDT)

All queues store `TextRecord` structures containing:
- **text**: Raw user input (up to 1024 characters)
- **interrog_count**: Number of interrogative sentences (ending with `?`)
- **second_len**: Length of the 2nd interrogative sentence
- **source_file**: Origin file/source hint

## Operations Supported

Every queue type supports these operations:

| Operation | Code | Description |
|-----------|------|-------------|
| Enqueue (rear) | 1 | Insert element at rear |
| Enqueue (front) | 2 | Insert element at front [Deque only] |
| Dequeue (front) | 3 | Remove from front |
| Dequeue (rear) | 4 | Remove from rear [Deque only] |
| Peek front | 5 | View front element without removal |
| Peek rear | 6 | View rear element without removal |
| Display all | 7 | Show all elements with metadata |
| Search by position | 8 | Retrieve element at 1-based position |
| Search by text value | 9 | Find element by exact text match |
| Delete at position | 10 | Remove element at indexed position |
| Register (save) | 11 | Save queue to text or binary file |
| Load from binary | 12 | Restore queue from binary file |
| Input/Output | 5 (main) | Analyze text file for interrogative sentences |

## File Structure

```
version_B/
├── main.c           # Entry point, Input/Output function
├── menu.h           # Interactive menu system (header-only)
├── queue.h          # Queue implementation (header-only, all 4 types)
├── cdt.h            # Custom data type: TextRecord
├── file_utils.h     # File I/O utilities
├── lab6_B           # Compiled executable
└── README.md        # This file
```

## Compilation

```bash
gcc -Wall -Wextra -o lab6_B main.c
```

## Usage

### Run the Program

```bash
./lab6_B
```

### Main Menu

```
├─ 1. Work with Simple Queue
├─ 2. Work with Double-Ended Queue (Deque)
├─ 3. Work with Circular Queue (prompts for capacity)
├─ 4. Work with Priority Queue
├─ 5. Input/Output (input.txt / output.txt analysis)
└─ 0. Exit
```

### Example Workflow

1. Select queue type (e.g., `2` for Deque)
2. Enqueue sentences:
   - Text with `.`, `!`, or `?` sentence delimiters
   - Example: `"What is this? This is a test."`
3. View queue with option 7 to see interrogative counts
4. Save to file (option 11) in text or binary mode
5. Load back (option 12)

## Input Format

- **Text input**: Sentences must end with `.`, `!`, or `?`
- **Interrogative sentences**: Those ending with `?`
- Example input:
  ```
  What is your name? My name is John. How are you today? I am fine!
  ```

## Output Example

### Display Output (Option 7)

```
── Simple Queue  (size=3) ────────────────────────────────────
  #     Text (first 45 chars)                  IQ  2ndL
  -------------------------------------------------------------------
  1     What is your name?                      1     17
  2     My name is John.                        0     0
  3     How are you?                            1     12
```

### Saved File (Text Mode)

```
=== Simple Queue  REGISTER (size=2) ===

[1]
TEXT    : What is your name? How are you?
INTERROG: 2
2ND_LEN : 12
SOURCE  : keyboard
---

[2]
TEXT    : This is a test.
INTERROG: 0
2ND_LEN : 0
SOURCE  : keyboard
---
```

## Priority Queue Behavior

The Priority Queue orders by **interrogative count (descending)**:
- Items with the most question marks appear first
- When dequeuing, you get the highest interrogative count first
- Insertion maintains sorted order automatically

Example:
- Input: "Hello?" (1 question) → "What? How? Why?" (3 questions)
- Queue order: ["What? How? Why?", "Hello?"]
- Dequeue returns "What? How? Why?" first

## Input/Output (Option 5)

Specialized text analysis tool:
- Prompts for text input
- Counts total interrogative sentences
- Finds length of 2nd interrogative sentence
- Generates `input.txt` and `output.txt` files
- Outputs:
  ```
  ── Input/Output ───────────────────────
  Enter your text string for input.txt:
  > What is this? This is a test. How are you?
  
  ── Analysis results ───────────────────
  Number of interrogative sentences : 2
  Length of 2nd interrogative sent. : 12 characters
  [input.txt written]
  [output.txt written]
  ```

## Technical Details

### Memory Management
- All nodes and records dynamically allocated with `malloc()`
- Proper cleanup with `free()` on dequeue/delete
- Queue destruction frees entire linked list

### Linked List Implementation
- Doubly-linked nodes: `prev` and `next` pointers
- Direct front/rear access: O(1) operations
- Position-based search: O(n) traversal

### File I/O
- **Text mode**: Human-readable format with metadata
- **Binary mode**: Binary `TextRecord` structures for efficiency
- Both modes preserve all queue data

## Compilation Flags

- `-Wall`: Enable all common warnings
- `-Wextra`: Enable extra warnings for code quality

## Key Differences from Version A

- **Version A**: Stack-based implementation (LIFO)
- **Version B**: Queue-based implementation (FIFO/variants)
- Version B supports 4 queue types vs. single stack
- Different operation semantics and ordering

## Requirements Met

✓ Uses pointers and dynamic memory allocation  
✓ Procedural C/C++ implementation  
✓ Functions organized in header files  
✓ Complete interactive menu system  
✓ Supports text and binary file I/O  
✓ Custom data type (TextRecord) with metadata  
✓ All CRUD operations (Create, Read, Update, Delete)  
✓ Git repository with branch commits  

## Author

David Nenita

## Course

C/C++ Programming – Lab 6 (Average Level)  
DSA – FAF – 2022-23

---

**Version B – Dynamic Queue Variants**  
All queue types based on List ADS implementation.
