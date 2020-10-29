//
//  Find.h
//  Algorithms
//
//  Created by Foma Mironenko on 29.10.2020.
//

#ifndef Find_h
#define Find_h

#include "SFINAE.hpp"


template <typename Iterator, bool isOk>
struct Find;

template <typename Iterator>
struct Find<Iterator, true>
{
    static Iterator go(Iterator s, Iterator e, decltype(*std::declval<Iterator>()) element)
    {
        for(; s != e; s++)
        {
            if( memcmp(&*s, &element, sizeof(element)) ) {
                break;
            }
        }
        return s;
    }
};

template <typename Iterator>
struct Find<Iterator, false>
{
    static Iterator go(Iterator s, Iterator e, decltype(*std::declval<Iterator>()) element)
    {
        for(; s != e; s++)
        {
            if(*s == element) {
                break;
            }
        }
        return s;
    }
};

// std::remove_reference<decltype(*std::declval<Iterator>())>::value
template<typename Iterator>
Iterator do_find(Iterator s, Iterator e, typename std::remove_reference<decltype(*std::declval<Iterator>())>::type element)
{
    constexpr bool canMem = canMemcpy<Iterator>::value;
    return Find< Iterator, canMem >::go(s, e, element);
}


#endif /* Find_h */
