//
//  main.cpp
//  SFINAE
//
//  Created by Foma Mironenko on 08.10.2020.
//



#include <iostream>
#include <vector>
#include <type_traits>


                     
template<typename T> struct has_fun_foo{
private:
    static int detect(...);
    template<typename U> static decltype(std::declval<U>().foo(std::declval<int>())) detect(const U&);
public:
    static constexpr bool value = std::is_same<void, decltype(detect(std::declval<T>()))>::value;
};




template<typename T>
struct has_fil_name
{
private:
    static void detect(...);
    template<typename U>
    static decltype(std::declval<U>().name) detect(const U&);
public:
    static constexpr bool value = !std::is_same<void, decltype(detect(std::declval<T>()))>::value;
};



template<typename T, typename type>
struct has_filType_name
{
private:
    static void detect(...);
    template<typename U>
    static decltype(std::declval<U>().name) detect(const U&);
public:
    static constexpr bool value = std::is_same<type, decltype(detect(std::declval<T>()))>::value;
};



template<typename T>
struct has_same_fields
{
private:
    class NullClass1;
    class NullClass2;
    
    static NullClass1 detect1(...);
    static NullClass2 detect2(...);
    
    template<typename U>
    static decltype(std::declval<U>().name1) detect1(const U&);
    template<typename U>
    static decltype(std::declval<U>().name2) detect2(const U&);
public:
    static constexpr bool value = std::is_same<
        decltype(detect1(std::declval<T>())),
        decltype(detect2(std::declval<T>()))
    >::value;
};



template<typename T, typename Fun>
struct has_function;

template<typename T, typename R, typename ... Args>
struct has_function<T, R(Args ...)>
{
private:
    class NullClass;
    static NullClass detect(...);
    
    template<typename U> static decltype(
            std::declval<U>().foo( std::declval<Args>()... )) detect(const U&);
public:
    static constexpr bool value = std::is_same<
        R,   decltype(detect(std::declval<T>()))
    >::value;
};



typedef int Func(int, char, float);

struct A
{
    int lol;
    std::vector<int> field;
    int foo(int x, char c, float f);
    char name1;
    char name2;
};




int main() {
    std::cout << has_function<A, Func>::value << std::endl;
    
    return 0;
}