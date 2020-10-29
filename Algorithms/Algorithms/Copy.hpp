//
//  Copy.h
//  Algorithms
//
//  Created by Foma Mironenko on 29.10.2020.
//

#ifndef Copy_h
#define Copy_h

#include "SFINAE.hpp"



template<typename Iterator1, typename Iterator2, bool canCopy>
struct Copy;

template<typename Iterator1, typename Iterator2>
struct Copy<Iterator1, Iterator2, false>
{
    static void go(Iterator1 s, Iterator1 e, Iterator2 to)
    {
        for (;s != e; ++s)
        {
            *to = *s;
            ++to;
        }
    }
};

template<typename Iterator1, typename Iterator2>
struct Copy<Iterator1, Iterator2, true>
{
    static void go(Iterator1 s, Iterator1 e, Iterator2 to)
    {
        memcpy(&*to, &*s, (int)(&*e - &*s));
    }
};

template<typename Iterator1, typename Iterator2>
void do_copy(Iterator1 s, Iterator1 e, Iterator2 to)
{
    static_assert(
        std::is_same<   decltype(*std::declval<Iterator1>()),
                        decltype(*std::declval<Iterator2>())>::value, "different types");
    constexpr bool canCpy =
        canMemcpy<Iterator1>::value &&
        canMemcpy<Iterator2>::value;
    Copy< Iterator1, Iterator2, canCpy >::go(s, e, to);
}


#endif /* Copy_h */
