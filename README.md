# Lab 6 - Data Structures: Stack & Queue Implementation

A comprehensive C/C++ implementation of Stack and Queue data structures using dynamically allocated linked lists. This project demonstrates procedural programming with custom data types, file I/O, and an interactive menu system.

**Course:** C/C++ Programming – Lab 6 (Average Level)  
**Author:** David Nenita  
**Semester:** DSA – FAF – 2022-23

---

## 📁 Project Structure

```
Lab-6/
├── README.md                      # This file (GitHub overview)
├── .git/                          # Git repository
│
├── Version A: Stack Implementation
│   ├── main.c                     # Entry point with Lab-10 helper
│   ├── menu.h                     # Interactive stack menu (header-only)
│   ├── stack.h                    # Stack implementation (header-only)
│   ├── cdt.h                      # Custom data type: TextRecord
│   ├── file_utils.h               # File I/O utilities
│   ├── lab6_A                     # Compiled executable
│   └── [Supporting files]         # Stack operations, data files
│
└── Version B: Queue Implementation
    ├── README.md                  # Complete Version B documentation
    ├── main.c                     # Entry point with Input/Output helper
    ├── menu.h                     # Interactive queue menu (header-only)
    ├── queue.h                    # Queue implementation (header-only)
    ├── cdt.h                      # Custom data type: TextRecord (shared)
    ├── file_utils.h               # File I/O utilities (shared)
    └── lab6_B                     # Compiled executable
```

---

## 🎯 Overview

### Version A: Stack (LIFO - Last In, First Out)

A procedural-style stack implementation using dynamically allocated doubly-linked list nodes. Follows Last-In-First-Out semantics with memory management functions.

**Key Features:**
- Dynamic memory allocation with `malloc()` / `free()`
- Interactive menu-driven interface
- Text and binary file I/O
- Custom TextRecord data type with interrogative sentence analysis
- Complete CRUD operations (Create, Read, Update, Delete)

### Version B: Queue (Multiple Variants)

Four different queue implementations on a unified linked-list base structure, supporting different semantics and use cases.

**Four Queue Types:**

1. **Simple Queue (FIFO)**
   - Enqueue at rear, dequeue from front
   - Traditional queue behavior

2. **Double-Ended Queue (Deque)**
   - Enqueue/dequeue at both front and rear
   - Maximum flexibility

3. **Circular Queue**
   - Fixed-capacity ring buffer implementation
   - User-specified capacity
   - Overflow prevention

4. **Priority Queue**
   - Max-heap ordering by interrogative count
   - Automatic sorted insertion
   - Highest interrogative count sentences first

---

## 🔧 Compilation

### Version A (Stack)

```bash
cd /path/to/repo
gcc -Wall -Wextra -o lab6_A main.c
./lab6_A
```

### Version B (Queue)

```bash
cd version_B/
gcc -Wall -Wextra -o lab6_B main.c
./lab6_B
```

Both use `-Wall` and `-Wextra` flags for comprehensive warning detection.

---

## 📖 Custom Data Type (TextRecord)

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

| Operation | Code | Description |
|-----------|------|-------------|
| Enqueue (rear) | 1 | Insert at rear |
| Enqueue (front) | 2 | Insert at front [Deque only] |
| Dequeue (front) | 3 | Remove from front |
| Dequeue (rear) | 4 | Remove from rear [Deque only] |
| Peek front | 5 | View front element |
| Peek rear | 6 | View rear element |
| Display all | 7 | Show all elements |
| Search by position | 8 | Retrieve at index |
| Search by text value | 9 | Find by text match |
| Delete at position | 10 | Remove at index |
| Register (save) | 11 | Save to file |
| Load from binary | 12 | Restore from file |
| Input/Output | 5 (main) | Text analysis tool |

---

## 🔍 Input/Output Analysis Feature

Both versions include a text analysis tool (menu option) that:

- Counts interrogative sentences (ending with `?`)
- Finds length of the 2nd interrogative sentence
- Generates `input.txt` (raw input)
- Generates `output.txt` (analysis results)
- Provides immediate feedback on analysis

**Example:**
```
Input:  "What is your name? My name is John. How are you today?"
Output: 
  Number of interrogative sentences: 2
  Length of 2nd interrogative: 17 characters
```

---

## 💾 File I/O

### Two Modes Supported

**1. Text Mode**
- Human-readable records
- Metadata preserved
- Good for debugging and portability

**2. Binary Mode**
- Compact binary format
- Faster I/O operations
- Direct memory serialization
- Cross-platform compatible

### File Operations

```bash
# Typical workflow
./lab6_B
  > Select queue type (1-4)
  > Perform operations
  > Save to file (option 11, choose binary mode)
  > Load from file (option 12)
```

---

## 🎨 User Interface

### Version A: Stack Menu
```
╔════════════════════════════════════════╗
║  LAB 6 – Version A : STACK (List ADS) ║
╚════════════════════════════════════════╝

MAIN MENU
1. Stack Operations
2. Load/Display Operations
3. Configuration
0. Exit
```

### Version B: Queue Menu
```
╔═════════════════════════════════════════════════╗
║  LAB 6 – Version B : Dynamic Queue Variants   ║
╚═════════════════════════════════════════════════╝

MAIN MENU
1. Work with Simple Queue
2. Work with Double-Ended Queue (Deque)
3. Work with Circular Queue
4. Work with Priority Queue
5. Input/Output (text analysis)
0. Exit
```

---

## 🚀 Quick Start

### Try Version B (Recommended)

```bash
cd version_B/
gcc -Wall -Wextra -o lab6_B main.c
./lab6_B
```

**Sample Interaction:**
```
Choice: 2                          # Select Deque

Enqueue example:
  Choice: 1                        # Add element
  > "What is this? Where am I?"
  [Enqueued. Size: 1]

Display:
  Choice: 7                        # Show queue
  # | Text (first 45 chars)                  | IQ | 2ndL
  1 | What is this? Where am I?              | 2  | 9

Save to file:
  Choice: 11                       # Register queue
  > my_queue.bin
  > 1 (binary mode)
  [Saved to: my_queue.bin (binary mode)]
```

---

## 📚 Technical Details

### Memory Management

- Nodes dynamically allocated with `malloc()`
- Proper cleanup with `free()` on removal
- Queue/Stack destruction frees entire structure
- No memory leaks detected with proper usage

### Data Structures

**Doubly-Linked Node:**
```c
typedef struct QNode {
    TextRecord  *data;
    struct QNode *prev;
    struct QNode *next;
} QNode;
```

### Performance

| Operation | Version A | Version B |
|-----------|-----------|-----------|
| Push/Enqueue | O(1) | O(1) to O(n)* |
| Pop/Dequeue | O(1) | O(1) |
| Peek | O(1) | O(1) |
| Search by position | O(n) | O(n) |
| Search by value | O(n) | O(n) |
| Display | O(n) | O(n) |

*Priority Queue - O(n) for sorted insertion

---

## ✅ Requirements Met

- ✓ Uses pointers and dynamic memory allocation
- ✓ Procedural C/C++ implementation
- ✓ Functions organized in header files
- ✓ Interactive menu system with CRUD operations
- ✓ Custom data type (TextRecord) with analysis
- ✓ Text and binary file I/O support
- ✓ Comprehensive error handling
- ✓ Compiler flags: `-Wall -Wextra`
- ✓ Git version control with branch commits
- ✓ Complete documentation in README files

---

## 📝 Version Control

### Branches

- `main`: Main development branch
- `Variant_A`: Stack implementation
- `Variant_B`: Queue implementation

### Commits

Each version includes structured commits for:
- Initial implementation
- Bug fixes and refinements
- Documentation updates
- Final releases

---

## 🔗 Documentation

- **Version A Details**: See Stack implementation files in root
- **Version B Details**: See [version_B/README.md](version_B/README.md)
- **Code Comments**: Extensive inline documentation in header files

---

## 🛠️ Building from Source

### Prerequisites

- GCC compiler (or Clang)
- Make (optional)
- Unix-like environment (Linux, macOS) or Cygwin on Windows

### Build Steps

```bash
# Clone or navigate to repository
cd /path/to/Lab-6

# Build Version A
gcc -Wall -Wextra -o lab6_A main.c

# Build Version B
cd version_B/
gcc -Wall -Wextra -o lab6_B main.c
cd ..

# Run either version
./lab6_A    # Stack
./version_B/lab6_B   # Queue
```

---

## 📄 License

This is an educational project for a university course.

---

## 👤 Author

**David Nenita**  
DSA Course – Spring 2022-23  
Technical University of Moldova (FCIM - FAF)

---

**Last Updated:** May 7, 2026

For detailed information about Version B, see [version_B/README.md](version_B/README.md)
