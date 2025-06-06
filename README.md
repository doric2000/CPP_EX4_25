# MyContainer Project (Assignment 4)

**Author:** dor.cohen15@msmail.ariel.ac.il  
**Default Template Type:** `int` (supports any comparable type)

---

## Overview

`MyContainer<T>` is a generic container of comparable elements, offering:

- `addElement(const T&)`  
- `remove(const T&)` (erases all occurrences; throws `std::runtime_error` if none found)  
- `size() const noexcept`  
- `operator<<` (prints as `[e1, e2, …]` or `[]` when empty)  

Six specialized iterators traverse its contents in different orders:

1. **OrderIterator** (insertion order)  
2. **AscendingOrderIterator** (sorted ascending)  
3. **DescendingOrderIterator** (sorted descending)  
4. **SideCrossOrderIterator** (smallest, largest, next smallest, next largest, …)  
5. **ReverseOrderIterator** (reverse insertion order)  
6. **MiddleOutOrderIterator** (middle element first, then alternately left/right)

Dereferencing or incrementing past `end()` throws `std::out_of_range`.

---

## Project Structure

.
├── Demo.cpp
├── Makefile
├── MyContainer.hpp
├── OrderIterator.hpp
├── AscendingOrderIterator.hpp
├── DescendingOrderIterator.hpp
├── SideCrossOrderIterator.hpp
├── ReverseOrderIterator.hpp
├── MiddleOutOrderIterator.hpp
├── test.cpp
└── README.md

markdown
Copy
Edit

- **Demo.cpp** Demonstrates all six iterators on `MyContainer<int>`.  
- **Makefile** Targets:
  - `make Main` (build & run Demo)  
  - `make test` (build & run unit tests)  
  - `make valgrind` (check memory leaks via Valgrind)  
  - `make clean` (remove binaries)  
- **MyContainer.hpp** Defines the `MyContainer<T>` class.  
- **OrderIterator.hpp** Insertion‐order iterator.  
- **AscendingOrderIterator.hpp** Iterates a sorted copy ascending.  
- **DescendingOrderIterator.hpp** Iterates a sorted copy descending.  
- **SideCrossOrderIterator.hpp** Iterates “side‐cross” from a sorted copy.  
- **ReverseOrderIterator.hpp** Iterates a reversed copy of insertion order.  
- **MiddleOutOrderIterator.hpp** Iterates “middle‐out” from a sorted copy.  
- **test.cpp** Unit tests using doctest.

---

## Build & Run

```bash
# Build and run the demonstration:
make Main

# Build and run unit tests:
make test

# Check memory leaks with Valgrind:
make valgrind

# Clean up:
make clean
Usage Example (Demo.cpp)
cpp
Copy
Edit
MyContainer<int> c;
c.addElement(7); c.addElement(15); c.addElement(6);
c.addElement(1); c.addElement(2);

// Insertion order: 7 15 6 1 2
for (auto it = c.begin_order(); it != c.end_order(); ++it)
    std::cout << *it << ' ';

// Ascending order: 1 2 6 7 15
for (auto it = c.begin_ascending_order(); it != c.end_ascending_order(); ++it)
    std::cout << *it << ' ';

// Descending order: 15 7 6 2 1
for (auto it = c.begin_descending_order(); it != c.end_descending_order(); ++it)
    std::cout << *it << ' ';

// Side-cross order: 1 15 2 7 6
for (auto it = c.begin_side_cross_order(); it != c.end_side_cross_order(); ++it)
    std::cout << *it << ' ';

// Reverse order: 2 1 6 15 7
for (auto it = c.begin_reverse_order(); it != c.end_reverse_order(); ++it)
    std::cout << *it << ' ';

// Middle-out order: 6 15 1 7 2
for (auto it = c.begin_middle_out_order(); it != c.end_middle_out_order(); ++it)
    std::cout << *it << ' ';
Unit Testing
Framework: doctest

test.cpp checks:

Empty‐container behavior (size(), remove, operator<<, iterators throw).

Removing until empty and duplicate removal.

Iterators over empty, single‐element, even/odd‐sized, and duplicate‐valued containers.

Exception paths (std::out_of_range, std::runtime_error).

Iterator comparisons (operator==, operator!=).

Run:

bash
Copy
Edit
make test
Exception Handling
remove(...) throws std::runtime_error if the container is empty or the value is not found.

All iterators’ operator*() and prefix operator++() throw std::out_of_range when out of bounds.

Memory Safety
After building Demo:

bash
Copy
Edit
make valgrind
to confirm no memory leaks.

Notes
All “sorted” or “rearranged” iterators operate on a copy of the container’s data—original remains unchanged.

MiddleOutOrderIterator: for size n, picks mid = n/2 (rounds down when even), then alternates left and right indices.

Default template parameter is int; you may also use MyContainer<std::string>, MyContainer<double>, etc., as long as T is comparable.

End of README