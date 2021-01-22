//
//  main.cpp
//  MetaProgramming
//
//  Created by Foma Mironenko on 01.10.2020.
//

#include <iostream>
#include <type_traits>
#include <vector>


//---- TYPE LIST ----//

struct Terminator
{};

template<class H, class T>
struct TypeList
{
    typedef H Head;
    typedef T Tail;
};

template<class Left, class Right>
struct TypeTree
{
    typedef Left L;
    typedef Right R;
};

//~~~~ TYPE LIST ~~~~//



//---- LENGTH ----//

template <class TL>
struct Length;

template <>
struct Length<Terminator>
{
    enum {value = 0};
};

template <class H, class T>
struct Length<TypeList<H, T>>
{
    enum {value = 1 + Length<T>::value};
};

//~~~~ LENGTH ~~~~//


//---- INDEX ----//

template <class TL, unsigned i>
struct Getter;

template <class H, class T>
struct Getter<TypeList<H, T>, 0>
{
    typedef H type;
};

template <class H, class T, unsigned i>
struct Getter<TypeList<H, T>, i>
{
    typedef typename Getter<T, i-1>::type type;
};

//~~~~ INDEX ~~~~//


//        1       //
//---- MinMax ----//

#define MM_pattern(MM, sign)                            \
constexpr unsigned CT_ ## MM (unsigned l, unsigned r)   \
{                                                       \
    return l sign r ? l : r;                            \
}                                                       \
                                                        \
template <class TL>                                     \
struct MM;                                              \
                                                        \
template <class H>                                      \
struct MM<TypeList<H, Terminator>>                      \
{                                                       \
    enum {value = sizeof(H)};                           \
};                                                      \
                                                        \
template <class H, class T>                             \
struct MM<TypeList<H, T>>                               \
{                                                       \
    enum {value = CT_##MM(sizeof(H), MM<T>::value)};    \
};

MM_pattern(min, <)
MM_pattern(max, >)

//~~~~ MinMax ~~~~//


//      2       //
//---- SWAP ----//

template <class TL, class first>
struct TmpSwap;

template <class first>
struct TmpSwap<Terminator, first>
{
    typedef TypeList<first, Terminator> type;
};

template <class H, class T, class first>
struct TmpSwap<TypeList<H, T>, first>
{
    typedef TypeList<H, typename TmpSwap<T, first>::type> type;
};


template <class TL>
struct Swap;

template <class H, class T>
struct Swap<TypeList<H, T> >
{
    typedef typename TmpSwap<T, H>::type type;
};

//~~~~ SWAP ~~~~//


//          3          //
//---- INHERITANCE ----//

template <class Father, class Mother>
struct Son: Father, Mother
{   };
template <class Mother>
struct Single: Mother
{   };



template <class TL>
struct ListInherit;

template <class H, class T>
struct ListInherit<TypeList<H, T>>
{
    typedef Son<H, ListInherit<T>> type;
};

template <class H>
struct ListInherit<TypeList<H, Terminator>>
{
    typedef Single<H> type;
};



template<class TT>
struct TreeInherit;

template<class L, class R>
struct TreeInherit<TypeTree<L, R>>
{
    typedef Son<TreeInherit<L>, TreeInherit<R>> type;
};

template <>
struct TreeInherit<Terminator>
{
    typedef Terminator type;
};

//~~~~ INHERITANCE ~~~~//



//          4          //
//---- DELETE EVEN ----//

template <class TL, bool del>
struct TmpDel;

template <bool del>
struct TmpDel<Terminator, del>
{
    typedef Terminator type;
};

template <class H, class T>
struct TmpDel<TypeList<H, T>, true>
{
    typedef typename TmpDel<T, false>::type type;
};

template <class H, class T>
struct TmpDel<TypeList<H, T>, false>
{
    typedef TypeList<H, typename TmpDel<T, true>::type> type;
};


template <class TL>
struct Del
{
    typedef typename TmpDel<TL, false>::type type;
};

//~~~~ DELETE EVEN ~~~~//


//       5       //
//---- Apply ----//

template <template <class, class> class Pair, class TL>
struct Applier;

template <template <class, class> class Pair, class H>
struct Applier<Pair, TypeList<H, Terminator>>
{
    typedef Terminator type;
};

template <template <class, class> class Pair, class H, class T>
struct Applier<Pair, TypeList<H, T>>
{
    typedef TypeList<Pair<H, typename T::Head>, typename Applier<Pair, T>::type > type;
};

//~~~~ Apply ~~~~//





//----NAgrs----//
template <int ... Args>
struct NArgs
{
    static const size_t value = sizeof ...(Args);
};
//~~~~NAgrs~~~~//


//----IntList----//
template<int ... members>
struct IntList;
template<int h, int ... t>
struct IntList<h, t...>
{
    static const int Head = h;
    using Tail = IntList<t ...>;
};
template<>
struct IntList <> {};
//~~~~IntList~~~~//


//----Length----//
template<typename T>
struct IntLength;
template<int ... Args>
struct IntLength<IntList<Args...> >
{
    static const size_t value = NArgs<Args...>::value;
};
//~~~~Length~~~~//


//----Cons----//
template<int E, typename IL>
struct Cons;
template<int E, int ... Args>
struct Cons<E, IntList<Args...>>
{
    using type = IntList<E, Args...>;
};
//~~~~Cons~~~~//


//----Concat----//
template<typename IL1, typename IL2>
struct Concat;
template <int ... A1, int ... A2>
struct Concat<IntList<A1...>, IntList<A2...>>
{
    using type = IntList<A1..., A2...>;
};
//~~~~Concat~~~~//


//----Generate----//
template<int N, int E = 0>
struct Generate
{
    using type = typename Cons<E, typename Generate<N-1, E+1>::type>::type;
};
template<unsigned E>
struct Generate<0, E>
{
    using type = IntList<>;
};
//~~~~Generate~~~~//


//----Print----//
template<typename IL>
void printIntList()
{
    std::cout << IL::Head << " ";
    printIntList<typename IL::Tail>();
}
template<>
void printIntList<IntList<>>()
{
    std::cout << "\n";
}
//~~~~Print~~~~//


//----Zip----//
template<template <int A, int B> class Bin, typename IL1, typename IL2>
struct Zip
{
    using type = typename Cons< Bin<IL1::Head, IL2::Head>::value,
                                typename Zip<Bin,
                                            typename IL1::Tail,
                                            typename IL2::Tail
                                            >::type
                                >::type;
};
template<template <int A, int B> class Bin>
struct Zip<Bin, IntList<>, IntList<> >
{
    using type = IntList<>;
};
//~~~~Zip~~~~//


template <bool ...Vals>
bool all()
{
    return ((Vals == 1) && ...);
}





template<int A, int B>
struct Mult
{
    static const int value = A * B;
};


int main()
{
    /*
    typedef TypeList<uint64_t,
                TypeList<char,
                    TypeList<short,
                        TypeList<bool, Terminator> > > > TL;
    std::cout << "Length " << Length<TL>::value << "\n";
    
    const unsigned i = 0;
    std::cout << "SizeOf " << i << " param is " << sizeof(Getter<TL, i>::type) << "\n\n";
    
    std::cout << "Min size " << min<TL>::value << "\n";
    std::cout << "Max size " << max<TL>::value << "\n";
    std::cout << "\n";
    
    
    typedef Swap<TL>::type Swapped;
    static_assert(std::is_same< Getter<TL, 0>::type,
                                Getter<Swapped, 3>::type >::value);
    static_assert(std::is_same< Getter<TL, 1>::type,
                                Getter<Swapped, 0>::type >::value);
    static_assert(std::is_same< Getter<TL, 2>::type,
                                Getter<Swapped, 1>::type >::value);
    
    
    
    typedef TypeList<std::vector<int>,
                TypeList<std::vector<char>,
                    TypeList<std::vector<bool>, Terminator> > > Family;
    typedef ListInherit<Family> MultiIn;
    
    
    typedef Del<TL>::type Shorten;
    std::cout << Length<Shorten>::value << "\n\n";
    
    
    typedef TypeTree<
                TypeTree<
                    int,
                    short>,
                TypeTree<
                    TypeTree<
                        float,
                        std::vector<bool>>,
                    char
                >
            > TreeFamily;
    
    typedef TreeInherit<TreeFamily> Multi;
     */
    
    
    using Lst = IntList<1, 2, 3, 4, 5>;
    using L = IntList<>;
    using L1 = Cons<3, L >::type;
    using L2 = Cons<2, L1>::type;
    using L3 = Cons<1, L2>::type;
    using L4 = Concat<L3, L3>::type;
    
    using L5 = Generate<100>::type;
    
    using L6 = Zip<Mult, L4, L4>::type;
    
    printIntList<L6>();
    std::cout << all<IntLength<L1>::value == 1,
                     IntLength<L4>::value == 6,
                     sizeof(int) == 4>()
              << "\n";
    
    
    return EXIT_SUCCESS;
}
