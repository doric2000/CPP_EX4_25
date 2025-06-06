#pragma once

#include "MyContainer.hpp"
#include <vector>
#include <algorithm>   // for std::sort
#include <cstddef>     // for std::size_t
#include <stdexcept>   // for std::out_of_range

namespace ariel {

/**
 * @brief Iterator that traverses a container’s elements in descending order.
 * 
 * DescendingOrderIterator takes a copy of the container’s data, sorts it in
 * Descending order, and then allows sequential access via iterator semantics.
 */
template<typename T>
class DescendingOrderIterator {
private:
    /// Sorted copy of all elements, in descending order
    std::vector<T> sorted_data;
    /// Current index within sorted_data (0-based)
    std::size_t index;

public:
    /**
     * @brief Construct a new DescendingOrderIterator.
     * 
     * Builds a sorted copy of all_data (descending), and sets the starting index.
     * 
     * @param all_data  Copy of the container’s data (will be moved into sorted_data).
     * @param idx       Starting index (default 0).
     */
    DescendingOrderIterator(std::vector<T> all_data, std::size_t idx = 0)
        : sorted_data(std::move(all_data)), index(idx)
    {
        // Sort the data in descending order upon construction
        std::sort(sorted_data.begin(), sorted_data.end(),std::greater<T>());
    }

    /**
     * @brief Dereference operator.
     * 
     * Returns a reference to the element at the current index in sorted_data.
     * Throws std::out_of_range if index is beyond the last element.
     * 
     * @return T&  Reference to sorted_data[index]
     * @throws std::out_of_range if index >= sorted_data.size()
     */
    const T& operator*() const {
        if (index >= sorted_data.size()) {
            throw std::out_of_range("Iterator is out of bounds");
        }
        return sorted_data[index];
    }

    /**
     * @brief Prefix increment operator.
     * 
     * Advances the iterator to the next element (index + 1). Throws
     * std::out_of_range if incrementing would pass the end of sorted_data.
     * 
     * @return DescendingOrderIterator&  Reference to this iterator after increment.
     * @throws std::out_of_range if index >= sorted_data.size()
     */
    DescendingOrderIterator& operator++() {
        if (index >= sorted_data.size()) {
            throw std::out_of_range("Cannot increment iterator: out of bounds");
        }
        ++index;
        return *this;
    }

    /**
     * @brief Postfix increment operator.
     * 
     * Creates a copy of the current iterator, advances this iterator by one,
     * and returns the copy. If incrementing is out of range, the prefix
     * increment will throw std::out_of_range.
     * 
     * @param int  Dummy parameter to distinguish postfix from prefix.
     * @return DescendingOrderIterator  Copy of this iterator before increment.
     * @throws std::out_of_range if index >= sorted_data.size()
     */
    DescendingOrderIterator operator++(int) {
        DescendingOrderIterator copy = *this;
        ++(*this);  // Calls prefix increment (bound check included)
        return copy;
    }

    /**
     * @brief Equality comparison operator.
     * 
     * Returns true if both iterators have the same index (i.e., point to
     * the same position in sorted_data). It is assumed they operate on
     * the same sorted_data context.
     * 
     * @param other  Another DescendingOrderIterator to compare with.
     * @return true  If both indices match.
     * @return false Otherwise.
     */
    bool operator==(const DescendingOrderIterator& other) const {
        return index == other.index;
    }

    /**
     * @brief Inequality comparison operator.
     * 
     * Returns the negation of operator==.
     * 
     * @param other  Another DescendingOrderIterator to compare with.
     * @return true  If indices differ.
     * @return false If indices match.
     */
    bool operator!=(const DescendingOrderIterator& other) const {
        return !(*this == other);
    }
};

} // namespace ariel
