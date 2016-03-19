#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "common.hpp"

template <class T, class Compare = std::less<T>>
class PriorityQueue : public std::priority_queue<T, std::vector<T>, Compare>
{    
public:
    explicit PriorityQueue(const Compare& comp_ = Compare())
        : std::priority_queue<T, std::vector<T>, Compare>(comp_)
    {
        
    }


    typename std::vector<T>::const_iterator find(const T& val) const
    {
        auto first = this->c.cbegin();
        auto last = this->c.cend();
        while (first!=last) {
            if (*first==val) return first;
            ++first;
        }
        return last;
    }

    typename std::vector<T>::const_iterator begin() const {return this->c.cbegin();};
    typename std::vector<T>::const_iterator end() const {return this->c.cend();};
};

#endif //PRIORITY_QUEUE_H