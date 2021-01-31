//
//  MapReduce.h
//  Algorithms
//
//  Created by Foma Mironenko on 31.01.2021.
//

#ifndef MapReduce_h
#define MapReduce_h

#include <vector>
#include <future>
#include <type_traits>
#include <iterator>

#include "SFINAE.hpp"

template <class Iterator, class FUn, class FBin>
using Ret = decltype(std::declval<FBin>()(
    std::declval<FUn>()(*std::declval<Iterator>()),
    std::declval<FUn>()(*std::declval<Iterator>())
));


template<class Iterator, class FUn, class FBin>
Ret<Iterator, FUn, FBin>
reduce(Iterator l, Iterator r,
       FUn fun, FBin fbin)
{
    Ret<Iterator, FUn, FBin> res = fun(*l);
    while(++l != r) {
        res = fbin(res, fun(*l));
    }
    return res;
}


template<class Iterator, class FUn, class FBin>
Ret<Iterator, FUn, FBin>
map_reduce(Iterator l, Iterator r,
           FUn fun, FBin fbin,
           size_t n_threads)
{
    std::vector<Iterator> gaps;
    gaps.reserve(n_threads + 1);
    size_t len = 0;
    size_t batch_size = 0;
    if constexpr(is_rand_access<Iterator>()) {
        len = r - l;
        batch_size = len / n_threads;
        for(int i = 0; i < n_threads; i++) {
            gaps.push_back(l+i*batch_size);
        }
    } else {
        //define structure length
        for(auto tmp = l; tmp != r; ++len, ++tmp);
        //define borders for each thread
        batch_size = len / n_threads;
        size_t upper = batch_size * n_threads;
        for(int i = 0; l != r && i < upper; ++l, ++i) {
            if(i % batch_size == 0) {
                gaps.push_back(l);
            }
        }
    }
    gaps.push_back(r);
    //pass each gapped sequence into a separate thread
    std::vector<std::future<Ret<Iterator, FUn, FBin> > > reduced(n_threads);
    for(int i = 0; i < n_threads; ++i) {
        reduced[i] = std::async(std::launch::async,
                                reduce<Iterator, FUn, FBin>,
                                gaps[i], gaps[i+1],
                                fun, fbin);
    }
    auto ans = reduced[0].get();
    for(int i = 1; i < n_threads; ++i) {
        ans = fbin(ans, reduced[i].get());
    }
    return ans;
}

#endif /* MapReduce_h */
