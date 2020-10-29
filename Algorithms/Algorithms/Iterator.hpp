//
//  Iterator.hpp
//  Algorithms
//
//  Created by Foma Mironenko on 29.10.2020.
//

#ifndef Iterator_hpp
#define Iterator_hpp


#include "SFINAE.hpp"

template <typename Iterator, bool has_plus>
struct advance;

template <typename Iterator>
struct advance<Iterator, true>
{
    static void go(Iterator & it, int offset)
    {
        it += offset;
    }
};

template <typename Iterator>
struct advance<Iterator, false>
{
    static void go(Iterator & it, int offset)
    {
        if(offset < 0) {
            while(offset < 0)
            {
                it--;
                offset++;
            }
        } else {
            while(offset > 0)
            {
                it++;
                offset--;
            }
        }
    }
};

template<typename Iterator>
void do_advance(Iterator & it, int offset)
{
    advance<decltype(it), has_plus_oper<decltype(it)>::value>::go(it, offset);
}

#endif /* Iterator_hpp */
