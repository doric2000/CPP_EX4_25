# Container and Iterators Project (Assignment 4)

**Author:** Dor Cohen  <br>
**Default Template Type:** `int` (supports any comparable type)  

---

## Project Overview

`MyContainer<T>` is a generic container of comparable elements, supporting:

- `addElement(const T&)` – add an element to the container.
- `remove(const T&)` – remove **all** occurrences of a value (throws `std::runtime_error` if not found).
- `size() const noexcept` – returns number of elements.
- `operator<<` – prints as `[a, b, c]` or `[]`.

### Iterators:

Each of the following iterators supports `begin()` and `end()` and throws `std::out_of_range` when overused:

- **OrderIterator** – Insertion order.
- **AscendingOrderIterator** – Sorted ascending.
- **DescendingOrderIterator** – Sorted descending.
- **SideCrossOrderIterator** – Alternating smallest/largest (e.g. `[1, 15, 2, 7, 6]`).
- **ReverseOrderIterator** – Reverse of insertion order.
- **MiddleOutOrderIterator** – Starts from middle, alternates left/right (e.g. `[6, 15, 1, 7, 2]`).

---

### Supported Operators in Iterators

Each iterator in this project implements the standard iterator interface:

- `operator*()` – Dereferences the iterator to return the current element.  
  Throws `std::out_of_range` if the iterator is at the end.

- `operator++()` – Prefix increment. Advances the iterator to the next element.  
  Throws `std::out_of_range` if incrementing past the end.

- `operator++(int)` – Postfix increment. Advances and returns the previous state.

- `operator==()` and `operator!=()` – Compare two iterators for equality or inequality.  
  Equality means both the container and the index are the same.

All iterators behave consistently with the C++ STL style:  
when `begin() == end()` the iteration is complete, and dereferencing is invalid.

---

## Project Structure

```
.
├── Demo.cpp                   # Demonstration of all iterators
├── Makefile                   # Build targets
├── MyContainer.hpp            # Container definition
├── OrderIterator.hpp
├── AscendingOrderIterator.hpp
├── DescendingOrderIterator.hpp
├── SideCrossOrderIterator.hpp
├── ReverseOrderIterator.hpp
├── MiddleOutOrderIterator.hpp
├── test.cpp                   # Unit tests using doctest
└── README.md
```

---

## Makefile Targets

- `make Main` – Build and run the demo program.
- `make test` – Build and run unit tests (requires `doctest.h`).
- `make valgrind` – Check for memory leaks on the Demo and the Tests.
- `make clean` – Remove generated binaries.

---

## Unit Tests

The file `test.cpp` covers:

- Empty containers: size, removal, printing, iterator behavior.
- Valid and invalid `remove()`.
- All iterator traversal patterns (including single, even, odd, duplicates).
- `operator==`, `operator!=`, exception cases.
- `valgrind` tested for memory leaks.

---

## Notes

- Each iterator operates on a copy (e.g., sorted or reversed).
- All behavior conforms to standard STL-like expectations.
- Template supports any type with `<` and `==` operators.

---

## License & Academic Integrity

This project was developed as part of a university assignment. Any use of AI or external help is declared according to the academic honesty policy.

```
