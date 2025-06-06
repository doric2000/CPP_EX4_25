// test.cpp

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "MyContainer.hpp"
#include <sstream>
#include <type_traits>

using namespace ariel;

//-----------------------------------------------------------------------------
// Helper: collectIterator(begin, end) → std::vector<non-const T>
//-----------------------------------------------------------------------------
//
// We want to build a std::vector<T> from “begin..end”.  In order to avoid
// ever writing `std::vector<const T>`, we must remove both reference and
// const/volatile qualifiers from whatever `*begin` returns.
//
//     RawType   = decltype(*begin)
//     ValueType = std::remove_cv_t<std::remove_reference_t<RawType>>
//
// Then we can safely push_back(*it) into a std::vector<ValueType>.
//
template<typename Iterator>
auto collectIterator(Iterator begin, Iterator end) {
    using RawType   = decltype(*begin); 
    using ValueType = std::remove_cv_t<std::remove_reference_t<RawType>>;
    std::vector<ValueType> result;
    for (auto it = begin; it != end; ++it) {
        result.push_back(*it);
    }
    return result;
}


//-----------------------------------------------------------------------------
// --------------- Edge cases for basic functions ---------------
//-----------------------------------------------------------------------------

TEST_CASE("Empty container: size, remove, operator<<, iterators") {
    MyContainer<int> c;

    // size() == 0
    CHECK(c.size() == 0);

    // remove(…) on empty → throws
    CHECK_THROWS_AS(c.remove(42), std::runtime_error);

    // operator<< on empty → "[]"
    {
        std::ostringstream os;
        os << c;
        CHECK(os.str() == "[]");
    }

    // All iterators: begin == end, * and ++ on it must throw std::out_of_range
    {
        // insertion‐order
        auto it1  = c.begin_order();
        auto end1 = c.end_order();
        CHECK(it1 == end1);
        CHECK_THROWS_AS(*it1, std::out_of_range);
        CHECK_THROWS_AS(++it1, std::out_of_range);

        // ascending
        auto it2  = c.begin_ascending_order();
        auto end2 = c.end_ascending_order();
        CHECK(it2 == end2);
        CHECK_THROWS_AS(*it2, std::out_of_range);
        CHECK_THROWS_AS(++it2, std::out_of_range);

        // descending
        auto it3  = c.begin_descending_order();
        auto end3 = c.end_descending_order();
        CHECK(it3 == end3);
        CHECK_THROWS_AS(*it3, std::out_of_range);
        CHECK_THROWS_AS(++it3, std::out_of_range);

        // side-cross
        auto it4  = c.begin_side_cross_order();
        auto end4 = c.end_side_cross_order();
        CHECK(it4 == end4);
        CHECK_THROWS_AS(*it4, std::out_of_range);
        CHECK_THROWS_AS(++it4, std::out_of_range);

        // reverse
        auto it5  = c.begin_reverse_order();
        auto end5 = c.end_reverse_order();
        CHECK(it5 == end5);
        CHECK_THROWS_AS(*it5, std::out_of_range);
        CHECK_THROWS_AS(++it5, std::out_of_range);

        // middle-out
        auto it6  = c.begin_middle_out_order();
        auto end6 = c.end_middle_out_order();
        CHECK(it6 == end6);
        CHECK_THROWS_AS(*it6, std::out_of_range);
        CHECK_THROWS_AS(++it6, std::out_of_range);
    }
}

TEST_CASE("Remove until empty") {
    MyContainer<int> c;
    c.addElement(7);
    c.addElement(7);
    c.addElement(7);

    CHECK(c.size() == 3);

    // remove(7) should delete all three
    c.remove(7);
    CHECK(c.size() == 0);

    // Removing again → still throws
    CHECK_THROWS_AS(c.remove(7), std::runtime_error);
}

TEST_CASE("operator<< on container<string>") {
    MyContainer<std::string> c;
    c.addElement("foo");
    c.addElement("bar");

    std::ostringstream os1;
    os1 << c;
    CHECK(os1.str() == "[foo, bar]");

    c.remove("foo");
    std::ostringstream os2;
    os2 << c;
    CHECK(os2.str() == "[bar]");
}


//-----------------------------------------------------------------------------
// ------------- Edge cases for iterators in detail -------------
//-----------------------------------------------------------------------------

TEST_CASE("Insertion-order: empty, duplicates, partial removal") {
    MyContainer<int> c;

    // Initially empty
    {
        auto it0  = c.begin_order();
        auto end0 = c.end_order();
        CHECK(it0 == end0);
    }

    // Add single element
    c.addElement(42);
    {
        auto it1  = c.begin_order();
        auto end1 = c.end_order();
        auto result = collectIterator(it1, end1);
        CHECK(result == std::vector<int>{42});

        ++it1;
        CHECK(it1 == c.end_order());
        CHECK_THROWS_AS(*it1, std::out_of_range);
    }

    // Add duplicates
    c.addElement(42);
    c.addElement(7);
    // Now data == [42, 42, 7]
    {
        auto it2  = c.begin_order();
        auto end2 = c.end_order();
        auto result = collectIterator(it2, end2);
        CHECK(result == std::vector<int>{42, 42, 7});
    }

    // Partial removal of all 42's
    c.remove(42);
    // Now data == [7]
    {
        auto it3  = c.begin_order();
        auto end3 = c.end_order();
        auto result = collectIterator(it3, end3);
        CHECK(result == std::vector<int>{7});
    }
    // Remove 7 → empty again
    c.remove(7);
    {
        auto it4  = c.begin_order();
        auto end4 = c.end_order();
        CHECK(it4 == end4);
        CHECK_THROWS_AS(*it4, std::out_of_range);
    }
}

TEST_CASE("AscendingOrderIterator: empty, single, duplicates, negative values") {
    MyContainer<int> c;
    c.addElement(-5);
    c.addElement(0);
    c.addElement(3);
    c.addElement(-5);
    // Original data [ -5, 0, 3, -5 ] → sorted ascending [ -5, -5, 0, 3 ]
    {
        auto it  = c.begin_ascending_order();
        auto end = c.end_ascending_order();
        auto result = collectIterator(it, end);
        CHECK(result == std::vector<int>{-5, -5, 0, 3});
    }

    // Single-element container
    MyContainer<int> d;
    d.addElement(100);
    {
        auto it  = d.begin_ascending_order();
        auto end = d.end_ascending_order();
        auto result = collectIterator(it, end);
        CHECK(result == std::vector<int>{100});
        ++it;
        CHECK(it == d.end_ascending_order());
        CHECK_THROWS_AS(*it, std::out_of_range);
    }
}

TEST_CASE("DescendingOrderIterator: empty, single, duplicates, large values") {
    MyContainer<int> c;
    c.addElement(1);
    c.addElement(1);
    c.addElement(5);
    c.addElement(1000000);
    // [1,1,5,1000000] → sorted descending [1000000, 5, 1, 1]
    {
        auto it  = c.begin_descending_order();
        auto end = c.end_descending_order();
        auto result = collectIterator(it, end);
        CHECK(result == std::vector<int>{1000000, 5, 1, 1});
    }

    // Single-element container
    MyContainer<int> d;
    d.addElement(-10);
    {
        auto it  = d.begin_descending_order();
        auto end = d.end_descending_order();
        auto result = collectIterator(it, end);
        CHECK(result == std::vector<int>{-10});
        ++it;
        CHECK(it == d.end_descending_order());
        CHECK_THROWS_AS(*it, std::out_of_range);
    }
}

TEST_CASE("ReverseOrderIterator: empty, single, multiple elements") {
    MyContainer<int> c;
    c.addElement(10);
    c.addElement(20);
    c.addElement(30);
    // Insertion order [10,20,30] → reversed [30,20,10]
    {
        auto it  = c.begin_reverse_order();
        auto end = c.end_reverse_order();
        auto result = collectIterator(it, end);
        CHECK(result == std::vector<int>{30, 20, 10});
    }

    // Single-element container
    MyContainer<int> d;
    d.addElement(99);
    {
        auto it  = d.begin_reverse_order();
        auto end = d.end_reverse_order();
        auto result = collectIterator(it, end);
        CHECK(result == std::vector<int>{99});
        ++it;
        CHECK(it == d.end_reverse_order());
        CHECK_THROWS_AS(*it, std::out_of_range);
    }
}

TEST_CASE("SideCrossOrderIterator: empty, single, even count, odd count, duplicates") {
    // Odd count example from demo: [7,15,6,1,2]
    MyContainer<int> c;
    c.addElement(7);
    c.addElement(15);
    c.addElement(6);
    c.addElement(1);
    c.addElement(2);
    // Sorted ascending: [1,2,6,7,15] → side-cross: [1,15,2,7,6]
    {
        auto it  = c.begin_side_cross_order();
        auto end = c.end_side_cross_order();
        auto result = collectIterator(it, end);
        CHECK(result == std::vector<int>{1, 15, 2, 7, 6});
    }

    // Even count: [4,1,3,2] → sorted [1,2,3,4] → side-cross: [1,4,2,3]
    MyContainer<int> d;
    d.addElement(4);
    d.addElement(1);
    d.addElement(3);
    d.addElement(2);
    {
        auto it  = d.begin_side_cross_order();
        auto end = d.end_side_cross_order();
        auto result = collectIterator(it, end);
        CHECK(result == std::vector<int>{1, 4, 2, 3});
    }

    // Single-element container
    MyContainer<int> e;
    e.addElement(5);
    {
        auto it  = e.begin_side_cross_order();
        auto end = e.end_side_cross_order();
        auto result = collectIterator(it, end);
        CHECK(result == std::vector<int>{5});
        ++it;
        CHECK(it == e.end_side_cross_order());
        CHECK_THROWS_AS(*it, std::out_of_range);
    }

    // Empty container
    MyContainer<int> f;
    {
        auto it  = f.begin_side_cross_order();
        auto end = f.end_side_cross_order();
        CHECK(it == end);
        CHECK_THROWS_AS(*it, std::out_of_range);
    }

    // Duplicates: [2,2,2,2] → sorted [2,2,2,2] → side-cross [2,2,2,2]
    MyContainer<int> g;
    for (int i = 0; i < 4; ++i) {
        g.addElement(2);
    }
    {
        auto it  = g.begin_side_cross_order();
        auto end = g.end_side_cross_order();
        auto result = collectIterator(it, end);
        CHECK(result == std::vector<int>{2, 2, 2, 2});
    }
}

TEST_CASE("MiddleOutOrderIterator: empty, single, even, odd, duplicates") {
    // Empty container
    MyContainer<int> c;
    {
        auto it  = c.begin_middle_out_order();
        auto end = c.end_middle_out_order();
        CHECK(it == end);
        CHECK_THROWS_AS(*it, std::out_of_range);
    }

    // Single-element container
    MyContainer<int> d;
    d.addElement(100);
    {
        auto it  = d.begin_middle_out_order();
        auto end = d.end_middle_out_order();
        auto result = collectIterator(it, end);
        CHECK(result == std::vector<int>{100});
        ++it;
        CHECK(it == end);
        CHECK_THROWS_AS(*it, std::out_of_range);
    }

    // Odd count example: [7,15,6,1,2]
    MyContainer<int> e;
    e.addElement(7);
    e.addElement(15);
    e.addElement(6);
    e.addElement(1);
    e.addElement(2);
    // Sorted ascending: [1,2,6,7,15],
    {
        auto it  = e.begin_middle_out_order();
        auto end = e.end_middle_out_order();
        auto result = collectIterator(it, end);
        CHECK(result == std::vector<int>{6, 15, 1, 7, 2});
    }

    // Even count: [4,1,3,2] → middle index = 2 → [3, 2, 4, 1]
    MyContainer<int> f;
    f.addElement(4);
    f.addElement(1);
    f.addElement(3);
    f.addElement(2);
    {
        auto it  = f.begin_middle_out_order();
        auto end = f.end_middle_out_order();
        auto result = collectIterator(it, end);
        CHECK(result == std::vector<int>{3, 1, 2, 4});
    }

    // Duplicates: [5,5,5,5] → sorted [5,5,5,5], middle index = 2 → [5,5,5,5]
    MyContainer<int> g;
    for (int i = 0; i < 4; ++i) {
        g.addElement(5);
    }
    {
        auto it  = g.begin_middle_out_order();
        auto end = g.end_middle_out_order();
        auto result = collectIterator(it, end);
        CHECK(result == std::vector<int>{5, 5, 5, 5});
    }
}

TEST_CASE("Iterator comparison on edge cases") {
    MyContainer<int> c;
    // empty → begin == begin, begin != something else?
    {
        auto i1 = c.begin_order();
        auto i2 = c.begin_order();
        CHECK(i1 == i2);
        CHECK(!(i1 != i2));
    }

    // after single advance in single-element container
    MyContainer<int> d;
    d.addElement(42);
    {
        auto i1 = d.begin_order();
        auto i2 = d.begin_order();
        CHECK(i1 == i2);
        ++i1;
        CHECK(i1 != i2);
        CHECK(i1 == d.end_order());
    }
}


TEST_CASE("Remove non-existing from non-empty container throws") {
    MyContainer<int> c;
    c.addElement(1);
    c.addElement(2);
    c.addElement(3);
    // Attempt to remove an element not present
    CHECK_THROWS_AS(c.remove(42), std::runtime_error);
    // Verify original data is unchanged
    auto it   = c.begin_order();
    auto end  = c.end_order();
    auto data = collectIterator(it, end);
    CHECK(data == std::vector<int>{1, 2, 3});
}

TEST_CASE("Postfix increment returns previous iterator state") {
    MyContainer<int> c;
    c.addElement(10);
    c.addElement(20);
    c.addElement(30);
    auto it      = c.begin_order();
    auto old_it  = it++;  // postfix: old_it should point to first element (10)
    CHECK(*old_it == 10);
    // it should now point to second element
    CHECK(*it == 20);

    // another postfix: old_it2 == element 20, iter advances to 30
    auto old_it2 = it++;
    CHECK(*old_it2 == 20);
    CHECK(*it == 30);

    // now it points to last; one more postfix should throw on increment
    auto at_last = it++;
    CHECK(*at_last == 30);
    CHECK_THROWS_AS(it++, std::out_of_range);
}

TEST_CASE("Iterator inequality across different containers") {
    MyContainer<int> a;
    MyContainer<int> b;
    a.addElement(5);
    b.addElement(5);
    auto ia = a.begin_order();
    auto ib = b.begin_order();
    // Even though both begin() yield an iterator to value 5, they refer to different containers
    CHECK(ia != ib);
}

TEST_CASE("SideCrossOrderIterator with exactly two elements") {
    MyContainer<int> c;
    c.addElement(4);
    c.addElement(2);
    // Sorted ascending: [2,4] → side-cross order: [2,4]
    auto it  = c.begin_side_cross_order();
    auto end = c.end_side_cross_order();
    auto result = collectIterator(it, end);
    CHECK(result == std::vector<int>{2, 4});
}

TEST_CASE("MiddleOutOrderIterator with exactly two elements") {
    MyContainer<int> c;
    c.addElement(100);
    c.addElement(50);
    // No prior sorting: original [100,50], n=2, middle_idx = n/2 = 1 → first element: 50
    // Then left_idx=0 → second element: 100
    auto it  = c.begin_middle_out_order();
    auto end = c.end_middle_out_order();
    auto result = collectIterator(it, end);
    CHECK(result == std::vector<int>{50, 100});
}

TEST_CASE("Iterating beyond end throws exception repeatedly") {
    MyContainer<int> c;
    c.addElement(7);
    // Begin and end
    auto it  = c.begin_ascending_order();
    auto end = c.end_ascending_order();
    // First dereference is fine
    CHECK(*it == 7);
    // Increment to end
    ++it;
    CHECK(it == end);
    // Dereference at end should throw
    CHECK_THROWS_AS(*it, std::out_of_range);
    // Prefix increment at end should throw
    CHECK_THROWS_AS(++it, std::out_of_range);
    // Second try also throws
    CHECK_THROWS_AS(++it, std::out_of_range);
}

TEST_CASE("Container of non-int type (std::string) and iterators") {
    MyContainer<std::string> c;
    c.addElement("alpha");
    c.addElement("beta");
    c.addElement("alpha");  // duplicate
    // Insertion-order: ["alpha","beta","alpha"]
    {
        auto it   = c.begin_order();
        auto end  = c.end_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<std::string>{"alpha", "beta", "alpha"});
    }
    // Ascending (lexicographic): ["alpha","alpha","beta"]
    {
        auto it   = c.begin_ascending_order();
        auto end  = c.end_ascending_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<std::string>{"alpha", "alpha", "beta"});
    }
    // Descending: ["beta","alpha","alpha"]
    {
        auto it   = c.begin_descending_order();
        auto end  = c.end_descending_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<std::string>{"beta", "alpha", "alpha"});
    }
    // Reverse insertion: ["alpha","beta","alpha"] → ["alpha","beta","alpha"] reversed = ["alpha","beta","alpha"]
    {
        auto it   = c.begin_reverse_order();
        auto end  = c.end_reverse_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<std::string>{"alpha", "beta", "alpha"});
    }
}

TEST_CASE("SideCrossOrderIterator with one element then removal changes behavior") {
    MyContainer<int> c;
    c.addElement(42);
    // Single element: side-cross is [42]
    {
        auto it  = c.begin_side_cross_order();
        auto end = c.end_side_cross_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<int>{42});
    }
    // Remove it → now empty, begin==end
    c.remove(42);
    auto it2  = c.begin_side_cross_order();
    auto end2 = c.end_side_cross_order();
    CHECK(it2 == end2);
    CHECK_THROWS_AS(*it2, std::out_of_range);
}

TEST_CASE("MiddleOutOrderIterator with all-equal elements") {
    MyContainer<int> c;
    for (int i = 0; i < 5; ++i) {
        c.addElement(9);
    }
    // Original [9,9,9,9,9], middle_idx=5/2=2 → first=9, then always 9
    auto it  = c.begin_middle_out_order();
    auto end = c.end_middle_out_order();
    auto result = collectIterator(it, end);
    CHECK(result == std::vector<int>{9, 9, 9, 9, 9});
}

TEST_CASE("AscendingOrderIterator stability when values are already sorted") {
    MyContainer<int> c;
    c.addElement(1);
    c.addElement(2);
    c.addElement(3);
    // Already sorted ascending: [1,2,3]
    auto it   = c.begin_ascending_order();
    auto end  = c.end_ascending_order();
    auto data = collectIterator(it, end);
    CHECK(data == std::vector<int>{1, 2, 3});
    // Confirm that two increments past end throw
    ++it; ++it; ++it;
    CHECK(it == end);
    CHECK_THROWS_AS(++it, std::out_of_range);
}

TEST_CASE("Partial removal then verify all iterator orders reflect update") {
    MyContainer<int> c;
    for (int v : {5, 3, 5, 1, 5, 2}) {
        c.addElement(v);
    }
    // Initial insertion order: [5,3,5,1,5,2]
    c.remove(5);
    // After remove(5): [3,1,2]
    CHECK(c.size() == 3);

    // Insertion-order iterator should yield [3,1,2]
    {
        auto it   = c.begin_order();
        auto end  = c.end_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<int>{3, 1, 2});
    }
    // Ascending-order: sorted [1,2,3]
    {
        auto it   = c.begin_ascending_order();
        auto end  = c.end_ascending_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<int>{1, 2, 3});
    }
    // Descending-order: [3,2,1]
    {
        auto it   = c.begin_descending_order();
        auto end  = c.end_descending_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<int>{3, 2, 1});
    }
    // Side-cross on [3,1,2] → sorted [1,2,3] → [1,3,2]
    {
        auto it   = c.begin_side_cross_order();
        auto end  = c.end_side_cross_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<int>{1, 3, 2});
    }
    // Reverse-order: [2,1,3]
    {
        auto it   = c.begin_reverse_order();
        auto end  = c.end_reverse_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<int>{2, 1, 3});
    }
    // Middle-out on [3,1,2]: n=3, middle_idx=1 → [1,] then left=0→3, right=2→2 → [1,3,2]
    {
        auto it   = c.begin_middle_out_order();
        auto end  = c.end_middle_out_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<int>{1, 3, 2});
    }
}

TEST_CASE("Iterator independence from container modifications after construction") {
    MyContainer<int> c;
    c.addElement(10);
    c.addElement(20);
    c.addElement(30);
    // Construct ascending iterator before any removal
    auto itAsc0  = c.begin_ascending_order();
    auto endAsc0 = c.end_ascending_order();
    // Remove middle element from container
    c.remove(20);
    // The iterator still should produce original sorted order [10,20,30]
    {
        auto data = collectIterator(itAsc0, endAsc0);
        CHECK(data == std::vector<int>{10, 20, 30});
    }
    // But a newly constructed ascending iterator reflects the updated container [10,30]
    {
        auto itAsc1  = c.begin_ascending_order();
        auto endAsc1 = c.end_ascending_order();
        auto data    = collectIterator(itAsc1, endAsc1);
        CHECK(data == std::vector<int>{10, 30});
    }
}

TEST_CASE("operator<< formats multiple elements with no trailing comma or space") {
    MyContainer<std::string> c;
    c.addElement("one");
    c.addElement("two");
    c.addElement("three");
    std::ostringstream os;
    os << c;
    CHECK(os.str() == "[one, two, three]");
}

TEST_CASE("SideCrossOrderIterator with exactly two elements and removal between uses") {
    MyContainer<int> c;
    c.addElement(8);
    c.addElement(4);
    // First side-cross: [4,8]
    {
        auto it   = c.begin_side_cross_order();
        auto end  = c.end_side_cross_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<int>{4, 8});
    }
    // Remove 4, container == [8]
    c.remove(4);
    {
        auto it   = c.begin_side_cross_order();
        auto end  = c.end_side_cross_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<int>{8});
    }
}

TEST_CASE("MiddleOutOrderIterator with even count uses floor division for middle index") {
    MyContainer<int> c;
    for (int v : {100, 200, 300, 400}) {
        c.addElement(v);
    }
    // Original: [100,200,300,400], n=4, floor(n/2)=2 → first element = 300
    // Then left=1→200, right=3→400, left=0→100
    auto it   = c.begin_middle_out_order();
    auto end  = c.end_middle_out_order();
    auto data = collectIterator(it, end);
    CHECK(data == std::vector<int>{300, 200, 400, 100});
}

TEST_CASE("AscendingOrderIterator with mix of negative, zero, and positive values including INT_MIN/INT_MAX") {
    MyContainer<int> c;
    c.addElement(0);
    c.addElement(std::numeric_limits<int>::max());
    c.addElement(-1);
    c.addElement(std::numeric_limits<int>::min());
    c.addElement(1);
    // Sorted ascending: [INT_MIN, -1, 0, 1, INT_MAX]
    auto it   = c.begin_ascending_order();
    auto end  = c.end_ascending_order();
    auto data = collectIterator(it, end);
    CHECK(data.size() == 5);
    CHECK(data.front() == std::numeric_limits<int>::min());
    CHECK(data[1] == -1);
    CHECK(data[2] == 0);
    CHECK(data[3] == 1);
    CHECK(data.back() == std::numeric_limits<int>::max());
}

TEST_CASE("Remove all occurrences when value appears at front, middle, and end") {
    MyContainer<int> c;
    for (int v : {7, 7, 8, 7, 9, 7}) {
        c.addElement(v);
    }
    // [7,7,8,7,9,7]; remove(7) should leave [8,9]
    c.remove(7);
    CHECK(c.size() == 2);
    {
        auto it   = c.begin_order();
        auto end  = c.end_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<int>{8, 9});
    }
    // Removing 7 again should throw
    CHECK_THROWS_AS(c.remove(7), std::runtime_error);
}

TEST_CASE("DescendingOrderIterator on single-element then check postfix increment") {
    MyContainer<int> c;
    c.addElement(42);
    auto it   = c.begin_descending_order();
    auto end  = c.end_descending_order();
    // First dereference
    CHECK(*it == 42);
    // Postfix increment returns old iterator
    auto oldIt = it++;
    CHECK(*oldIt == 42);
    // Now it == end
    CHECK(it == end);
    CHECK_THROWS_AS(*it, std::out_of_range);
    CHECK_THROWS_AS(it++, std::out_of_range);
}

TEST_CASE("Chained removes and re-adding elements maintains correct iteration order") {
    MyContainer<int> c;
    c.addElement(1);
    c.addElement(2);
    c.addElement(3);
    // Remove 2 → [1,3]
    c.remove(2);
    // Add 4 → [1,3,4]
    c.addElement(4);
    // Insertion-order: [1,3,4]
    {
        auto it   = c.begin_order();
        auto end  = c.end_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<int>{1, 3, 4});
    }
    // Reverse-order: [4,3,1]
    {
        auto it   = c.begin_reverse_order();
        auto end  = c.end_reverse_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<int>{4, 3, 1});
    }
    // Middle-out: n=3, mid=1→3, then 1, then 4 → [3,1,4]
    {
        auto it   = c.begin_middle_out_order();
        auto end  = c.end_middle_out_order();
        auto data = collectIterator(it, end);
        CHECK(data == std::vector<int>{3, 1, 4});
    }
}
