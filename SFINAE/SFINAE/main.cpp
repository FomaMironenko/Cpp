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



template<class T>
struct is_pointer
{
    static const bool value = false;
};
template<class T>
struct is_pointer<T*>
{
    static const bool value = true;
};

template<class T>
struct remove_ptr
{
    using type = T;
};
template<class T>
struct remove_ptr<T*>
{
    using type = T;
};


template<class T>
struct is_ref
{
    static const bool value = false;
};
template<class T>
struct is_ref<T&>
{
    static const bool value = true;
};


template <typename T>
typename remove_ptr<T>::type get_value(T t)
{
    if constexpr(is_pointer<T>::value) {
        return *t;
    } else {
        return t;
    }
}




typedef int Func(int, char, float);

struct A
{
    int lol;
    std::vector<int> field;
    int foo(int x, char c, float f);
    char name1;
    char name2;
};


bool f1(int x)
{
    return x + 2;
}
long long f2(int x)
{
    return x * 5;
}

struct Test
{
    bool foo(int a);
    bool boo(int b);
};


int main() {
    /*std::cout << has_function<A, Func>::value << std::endl;*/
    
    /*std::cout << std::is_same<
        decltype(&Test::foo),
        decltype(f1)>::value
    << std::endl;*/
    
    std::vector<int> V {2, 3, 4};
    int a  = V[0];
    int *b = &V[1];
    
    std::cout << get_value(a) << " " << get_value(b) << "\n";
    
    return 0;
}
