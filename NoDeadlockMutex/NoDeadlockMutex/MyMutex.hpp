//
//  MyMutex.hpp
//  NoDeadlockMutex
//
//  Created by Foma Mironenko on 24.09.2020.
//

#ifndef MyMutex_hpp
#define MyMutex_hpp

#include <stdio.h>
#include <iostream>
#include <set>


class NoDeadlockMutex
{
public:
    NoDeadlockMutex(): id(allocated.fetch_add(1))
    {    }
    
    void lock();
    void unlock();
    
    const unsigned id;
private:
    std::mutex mx;
    static std::atomic<unsigned> allocated;
};

struct MxComp
{
    bool operator()(NoDeadlockMutex * lhs,                           NoDeadlockMutex * rhs)
    {
        return lhs->id < rhs->id;
    }
};


class LockMaster
{
public:
    LockMaster(): applied(false)
    {   }
    
    bool pred_lock(NoDeadlockMutex * Mx);
    void lock_now();
    void unlock_now();
    
private:
    static std::mutex masterGuard;
    bool applied;
    std::set<NoDeadlockMutex * , MxComp> lockQuery;
};

#endif /* MyMutex_hpp */
