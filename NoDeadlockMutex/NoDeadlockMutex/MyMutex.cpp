//
//  MyMutex.cpp
//  NoDeadlockMutex
//
//  Created by Foma Mironenko on 24.09.2020.
//

#include "MyMutex.hpp"

// initialized with 0 by default
std::atomic<unsigned> NoDeadlockMutex::allocated;

std::mutex LockMaster::masterGuard;


//----NoDeadlockMutex----//

void NoDeadlockMutex::lock()
{
    mx.lock();
}
void NoDeadlockMutex::unlock()
{
    mx.unlock();
}


//----LockMaster----//

bool LockMaster::pred_lock(NoDeadlockMutex *Mx)
{
    std::lock_guard<std::mutex> LG(masterGuard);
    if(!applied) {
        lockQuery.insert(Mx);
    }
    return !applied;
}

void LockMaster::lock_now()
{
    masterGuard.lock();
    applied = true;
    masterGuard.unlock();
    
    for(auto s = lockQuery.begin(),
             e = lockQuery.end();
        s != e; ++s )
    {
        (*s)->lock();
    }
}

void LockMaster::unlock_now()
{
    std::lock_guard<std::mutex> LG(masterGuard);
    for(auto e = lockQuery.rbegin(),
             s = lockQuery.rend();
        e != s; ++e)
    {
        (*e)->unlock();
    }
    applied = false;
}

