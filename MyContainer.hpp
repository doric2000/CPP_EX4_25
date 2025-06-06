#ifndef MYCONTAINER_HPP
#define MYCONTAINER_HPP

#include <stdexcept> // to use exceptions
#include <vector> // to store elements.
#include <algorithm>

#include "OrderIterator.hpp"
#include "AscendingOrderIterator.hpp"
#include "DescendingOrderIterator.hpp"
#include "SideCrossOrderIterator.hpp"

namespace ariel {
    template<typename T = int> //should default be int
    class MyContainer {
    
        // will be able to use all private class memebers.
        template<typename U> friend class OrderIterator;

        template<typename U> friend class AscendingOrderIterator;

        template<typename U> friend class DescendingOrderIterator;
        

        private:
            std::vector<T> data; // בהתחלה אפשר להשתמש ב-vector כדי להתמקד בלוגיקה
        public:
            MyContainer() : data{} {}

            ~MyContainer() = default;

            void addElement(const T& value){
                data.push_back(value);
            }

            void remove(const T& value){
                
            }

            size_t size() const noexcept{
                return data.size();
            }

            friend std::ostream& operator<<(std::ostream& os, const MyContainer<T>& c){

            }

            OrderIterator<T> begin_order() {
                return OrderIterator(this, 0);
            }
            
            OrderIterator<T> end_order() {
                return OrderIterator(this, data.size());
            }

            AscendingOrderIterator<T> begin_ascending_order() {
                return AscendingOrderIterator(data,0);
            }

            AscendingOrderIterator<T> end_ascending_order() {
                return AscendingOrderIterator(data,data.size());
            }

            DescendingOrderIterator<T> begin_descending_order() {
                return DescendingOrderIterator(data,0);
            }

            DescendingOrderIterator<T> end_descending_order() {
                return DescendingOrderIterator(data,data.size());
            }

            SideCrossOrderIterator<T> begin_side_cross_order() {
                return SideCrossOrderIterator(data,0);
            }

            SideCrossOrderIterator<T> end_side_cross_order() {
                return SideCrossOrderIterator(data,data.size());
            }


    };


};


#endif // MYCONTAINER_HPP