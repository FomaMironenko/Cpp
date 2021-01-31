//
//  Header.hpp
//  Algorithms
//
//  Created by Foma Mironenko on 29.10.2020.
//

#ifndef Header_h
#define Header_h

#include <iostream>
#include <type_traits>


template<typename T> struct has_decr_oper{
private:
    static int detect(...);
    template<typename U> static decltype(std::declval<U>().operator--()) detect(const U&);
public:
    static constexpr bool value = !std::is_same<int, decltype(detect(std::declval<T>()))>::value;
};

template<typename T> struct has_plus_oper{
private:
    struct InvType;
    static InvType detect(...);
    template<typename U> static decltype(std::declval<U>().operator+=(std::declval<int>())) detect(const U&);
public:
    static constexpr bool value = !std::is_same<InvType, decltype(detect(std::declval<T>()))>::value;
};

template<typename T> struct has_eq_oper{
private:
    struct InvType;
    static InvType detect(...);
    template<typename U> static decltype(std::declval<U>() == std::declval<U>()) detect(const U&);
public:
    static constexpr bool value = std::is_same<bool, decltype(detect(std::declval<T>()))>::value;
};



template<typename Iterator>
struct canMemcpy
{
    static constexpr bool value =
        std::is_pointer<Iterator>::value &&
        std::is_trivially_assignable<
            decltype(*std::declval<Iterator>()),
            decltype(*std::declval<Iterator>())
        >::value;
};



template<class Iterator>
constexpr bool is_rand_access()
{
    return std::is_same<
        typename std::iterator_traits<Iterator>::iterator_category,
        std::random_access_iterator_tag
    >::value;
}



#endif /* Header_h */
