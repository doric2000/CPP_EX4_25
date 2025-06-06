//dor.cohen15@msmail.ariel.ac.il

#pragma once

#include "MyContainer.hpp"
#include <vector>
#include <cstddef>
#include <stdexcept>

namespace ariel {

//fw declaration:
template<typename T> class MyContainer;

/**
 * @brief Iterator for MyContainer that traverses elements in insertion order.
 * 
 * OrderIterator allows iterating over a MyContainer<T> from the first added element
 * up to the last. Supports both prefix and postfix increment, dereference, and
 * comparison operators.
 */
template<typename T>
class OrderIterator {
private:
    /// Reference to the container being iterated over
    const MyContainer<T>& container;
    /// Current index within the container (0-based)
    std::size_t index;

public:
    /**
     * @brief Construct a new OrderIterator.
     * 
     * @param cont Reference to the MyContainer to iterate over.
     * @param idx  Starting index (default is 0, i.e. the first element).
     */
    OrderIterator(const MyContainer<T>* cont, std::size_t idx = 0)
        : container(*cont), index(idx) {
            
        }

    /**
     * @brief Dereference operator.
     * 
     * Returns a reference to the current element. Throws std::out_of_range if
     * the iterator is past the end of the container.
     * 
     * @return T& Reference to the element at the current index.
     * @throws std::out_of_range if index >= container.size().
     */
    const T& operator*() const {
        if (index >= container.size()) {
            throw std::out_of_range("Iterator is out of bounds");
        }
        return container.data[index];
    }

    /**
     * @brief Prefix increment operator.
     * 
     * Advances the iterator to the next element. Throws std::out_of_range if
     * incrementing would go past the end of the container.
     * 
     * @return OrderIterator& Reference to this iterator after increment.
     * @throws std::out_of_range if index >= container.size().
     */
    OrderIterator& operator++() {
        if (index >= container.size()) {
            throw std::out_of_range("Cannot increment iterator: out of bounds");
        }
        ++index;
        return *this;
    }

    /**
     * @brief Postfix increment operator.
     * 
     * Creates a copy of the current iterator, advances this iterator by one,
     * and returns the copy (the old state). If advancing goes past the end,
     * the prefix increment will throw std::out_of_range.
     * 
     * @param int Dummy parameter to distinguish postfix from prefix.
     * @return OrderIterator Copy of the iterator before increment.
     * @throws std::out_of_range if index >= container.size().
     */
    OrderIterator operator++(int) {
        OrderIterator copy = *this;
        ++(*this);  // Calls prefix increment (which checks bounds)
        return copy;
    }

    /**
     * @brief Equality comparison operator.
     * 
     * Returns true if both iterators refer to the same container and the same index.
     * 
     * @param other Another OrderIterator to compare with.
     * @return true  If both iterators have the same container reference and index.
     * @return false Otherwise.
     */
    bool operator==(const OrderIterator& other) const {
        return (&container == &other.container) && (index == other.index);
    }

    /**
     * @brief Inequality comparison operator.
     * 
     * Returns the negation of operator==.
     * 
     * @param other Another OrderIterator to compare with.
     * @return true  If the iterators differ in container reference or index.
     * @return false If they are equal.
     */
    bool operator!=(const OrderIterator& other) const {
        return !(*this == other);
    }
};

} // namespace ariel