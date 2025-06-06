//dor.cohen15@msmail.ariel.ac.il

#pragma once

#include "MyContainer.hpp"
#include <vector>
#include <algorithm>   // for std::sort
#include <cstddef>     // for std::size_t
#include <stdexcept>   // for std::out_of_range

namespace ariel {

/**
 * @brief Iterator that traverses a container’s elements in Middle Out order.
 * 
 * MiddleOutOrderIterator takes a copy of the container’s data, sorts it in
 * Middle Out order, and then allows sequential access via iterator semantics.
 */
template<typename T>
class MiddleOutOrderIterator {
private:
    /// Sorted copy of all elements, in MiddleOut order
    std::vector<T> sorted_data;
    /// Current index within sorted_data (0-based)
    std::size_t index;

public:
    /**
     * @brief Construct a new MiddleOutOrderIterator.
     * 
     * Builds a sorted copy of all_data (MiddleOut), and sets the starting index.
     * 
     * @param all_data  Copy of the container’s data (will be moved into sorted_data).
     * @param idx       Starting index (default 0).
     */
    MiddleOutOrderIterator(std::vector<T> all_data, std::size_t idx = 0)
        : sorted_data(), index(idx)
    {
        //check that we do have elements
        int n = all_data.size();
        if (n == 0) {
            return; // no elements at all.
        }
       
        // we will create another vector that will contain the MiddleOut Sort:
        std::vector<T> alternating;
        alternating.reserve(all_data.size());
        
        int middle_idx = n / 2; // i choose to round up the inx;
        
        int left_idx = (middle_idx > 0)? middle_idx - 1 : -1; // if n = 1 might cause problems.
        int right_idx = middle_idx + 1;

        alternating.push_back(all_data[middle_idx]);

        while (left_idx >= 0 || right_idx < n) {
            if (left_idx >= 0) {
                alternating.push_back(all_data[left_idx]);
                --left_idx;
            }
            if (right_idx < n) {
                alternating.push_back(all_data[right_idx]);
                ++right_idx;
            }
        }
        sorted_data = std::move(alternating);
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
     * @return MiddleOutOrderIterator&  Reference to this iterator after increment.
     * @throws std::out_of_range if index >= sorted_data.size()
     */
    MiddleOutOrderIterator& operator++() {
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
     * @return MiddleOutOrderIterator  Copy of this iterator before increment.
     * @throws std::out_of_range if index >= sorted_data.size()
     */
    MiddleOutOrderIterator operator++(int) {
        MiddleOutOrderIterator copy = *this;
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
     * @param other  Another MiddleOutOrderIterator to compare with.
     * @return true  If both indices match.
     * @return false Otherwise.
     */
    bool operator==(const MiddleOutOrderIterator& other) const {
        return index == other.index;
    }

    /**
     * @brief Inequality comparison operator.
     * 
     * Returns the negation of operator==.
     * 
     * @param other  Another MiddleOutOrderIterator to compare with.
     * @return true  If indices differ.
     * @return false If indices match.
     */
    bool operator!=(const MiddleOutOrderIterator& other) const {
        return !(*this == other);
    }
};

} // namespace ariel
