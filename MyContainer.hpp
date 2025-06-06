//dor.cohen15@msmail.ariel.ac.il

#ifndef MYCONTAINER_HPP
#define MYCONTAINER_HPP

#include <stdexcept> // to use exceptions
#include <vector> // to store elements.
#include <algorithm>
#include <ostream> // to print

#include "OrderIterator.hpp"
#include "AscendingOrderIterator.hpp"
#include "DescendingOrderIterator.hpp"
#include "SideCrossOrderIterator.hpp"
#include "ReverseOrderIterator.hpp"
#include "MiddleOutOrderIterator.hpp"

namespace ariel {
    template<typename T = int> //should default be int
    class MyContainer {
    
        // will be able to use all private class memebers.
        template<typename U> friend class OrderIterator;

        template<typename U> friend class AscendingOrderIterator;

        template<typename U> friend class DescendingOrderIterator;

        template<typename U> friend class SideCrossOrderIterator;

        template<typename U> friend class MiddleOutOrderIterator;

        template<typename U> friend class ReverseOrderIterator;

        

        private:
            std::vector<T> data; 
        public:
            MyContainer() : data{} {}

            ~MyContainer() = default;

            void addElement(const T& value){
                data.push_back(value);
            }

            void remove(const T& value){
                if (this->size() == 0 )
                    throw std::runtime_error("Container is empty");
                
                auto newEnd = std::remove(data.begin(), data.end(), value); // logicly moves all elemets that are not value to the start of the vector.
                                                                            // and returns an iterator that points to where the removal values starts.
                if (newEnd == data.end()) {
                    throw std::runtime_error("Value to remove not found in container");
                }
                data.erase(newEnd, data.end());
            }

            size_t size() const noexcept{
                return data.size();
            }

            friend std::ostream& operator<<(std::ostream& os, const MyContainer<T>& c){
                if (c.size() == 0)
                {
                    os << "[]";
                    return os;
                }
                os << "[";
                for (size_t i = 0; i < c.data.size(); ++i) {
                    os << c.data[i];
                    if (i + 1 < c.data.size()) os << ", ";
                }
                os << "]";
                return os;
            }

            OrderIterator<T> begin_order () const {
                return OrderIterator(this, 0);
            }
            
            OrderIterator<T> end_order () const {
                return OrderIterator(this, data.size());
            }

            AscendingOrderIterator<T> begin_ascending_order () const{
                return AscendingOrderIterator(data,0);
            }

            AscendingOrderIterator<T> end_ascending_order () const{
                return AscendingOrderIterator(data,data.size());
            }

            DescendingOrderIterator<T> begin_descending_order () const {
                return DescendingOrderIterator(data,0);
            }

            DescendingOrderIterator<T> end_descending_order () const{
                return DescendingOrderIterator(data,data.size());
            }

            SideCrossOrderIterator<T> begin_side_cross_order () const{
                return SideCrossOrderIterator(data,0);
            }

            SideCrossOrderIterator<T> end_side_cross_order () const{
                return SideCrossOrderIterator(data,data.size());
            }

            ReverseOrderIterator<T> begin_reverse_order () const{
                return ReverseOrderIterator(data,0);
            }

            ReverseOrderIterator<T> end_reverse_order () const{
                return ReverseOrderIterator(data,data.size());
            }


            MiddleOutOrderIterator<T> begin_middle_out_order () const {
                return MiddleOutOrderIterator(data,0);
            }

            MiddleOutOrderIterator<T> end_middle_out_order () const{
                return MiddleOutOrderIterator(data,data.size());
            }


    };


};


#endif // MYCONTAINER_HPP