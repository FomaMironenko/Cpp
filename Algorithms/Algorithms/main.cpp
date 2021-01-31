
#include <iostream>
#include <algorithm>

#include <set>
#include <vector>
#include <list>

#include "Iterator.hpp"
#include "Copy.hpp"
#include "Find.hpp"
#include "MapReduce.hpp"


class MyTimer {
    typedef std::chrono::steady_clock clock;
public:
    MyTimer() {
        set();
    }

    void set() {
        store = clock::now();
    }

    double get() {
        std::chrono::duration<double> period = clock::now() - store;
        return period.count();
    }

private:
    std::chrono::time_point<clock> store;
};


void testTime(int N)
{
    int *A = new int[N];
    for(int i = 0; i < N; i++)
    {
        A[i] = i;
    }
    int *B = new int[N];
    
    
    MyTimer T;
    
    T.set();
    do_copy(A, A + N, B);
    std::cout << "memcpy " << T.get() << "\n";
    
    T.set();
    for(int i = 0; i < N; i++)
    {
        B[i] = A[i];
    }
    std::cout << "silly  " << T.get() << "\n\n\n";
}

struct A;

int main()
{
    std::list<int> l;
    for(int i = 0; i < 1e6; i++) {  l.push_back(i); }
    auto itLis = l.begin();
    
    std::vector<int> v(1e6);
    auto itVec = v.begin();
    
    do_advance(itLis, 100);
    do_copy(l.begin(), itLis, itVec);
    do_advance(itVec, 100);
    std::cout <<    (do_find(v.begin(), itVec, 99) != itVec) << " " <<
                    (do_find(v.begin(), itVec, 100) != itVec) << "\n\n";
    
    const int N = 1e6;
    testTime(N);
    
    
    int sum = map_reduce(
                l.begin(), l.end(),
                [](int i){return i*i;},
                std::plus<int>(), 4);
    std::cout << "ok\n";
    
    
    return EXIT_SUCCESS;
}
