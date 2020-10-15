//
//  lockArray.cpp
//  LockFreeStruct
//
//  Created by Foma Mironenko on 17.09.2020.
//  Copyright © 2020 Foma Mironenko. All rights reserved.
//

#include "lockArray.hpp"

//-----LockArray-----//

size_t LockArray::size()
{
    return data.size();
}




//-----LockObjectArray-----//

std::mutex      LockClassArray::flagOwner;
bool            LockClassArray::hasOwner = false;
std::thread::id LockClassArray::owner = std::this_thread::get_id();
std::mutex      LockClassArray::arrMutex;


void LockClassArray::lock()
{
    arrMutex.lock();
    std::lock_guard<std::mutex> locker(flagOwner);
    assert(!hasOwner);
    hasOwner = true;
    owner = std::this_thread::get_id();
}
void LockClassArray::unlock()
{
    arrMutex.unlock();
    std::lock_guard<std::mutex> locker(flagOwner);
    assert(hasOwner);
    hasOwner = false;
}


size_t LockClassArray::size()
{
    {
        std::lock_guard<std::mutex> locker(flagOwner);
        assert(hasOwner &&
               "trying to get size of an unowned object");
        if(owner == std::this_thread::get_id()) {
            return data.size();
        }
    }
    std::lock_guard<std::mutex> locker(arrMutex);
    {
        std::lock_guard<std::mutex> locker(flagOwner);
        assert(hasOwner && owner == std::this_thread::get_id() &&
               "trying to get size of an unowned object");
        return data.size();
    }
}
int LockClassArray::read(size_t i)
{
    {
        std::lock_guard<std::mutex> locker(flagOwner);
        assert(hasOwner &&
               "trying to read unowned object");
        if(owner == std::this_thread::get_id()) {
            return data[i];
        }
    }
    std::lock_guard<std::mutex> locker(arrMutex);
    {
        std::lock_guard<std::mutex> locker(flagOwner);
        assert(hasOwner && owner == std::this_thread::get_id() &&
               "trying to read unowned object");
        return data[i];
    }
}
void LockClassArray::write(size_t i, int val)
{
    {
        std::lock_guard<std::mutex> locker(flagOwner);
        assert(hasOwner &&
               "trying to write to unowned object");
        if(owner == std::this_thread::get_id()) {
            data[i] = val;
            return;
        }
    }
    std::lock_guard<std::mutex> locker(arrMutex);
    {
        std::lock_guard<std::mutex> locker(flagOwner);
        assert(hasOwner && owner == std::this_thread::get_id() &&
               "trying to write to unowned object");
        data[i] = val;
        return;
    }
}




//-----LockObjectArray-----//

size_t LockObjectArray::size()
{
    objMutex.lock();
    size_t tmp = data.size();
    objMutex.unlock();
    return tmp;
}

int LockObjectArray::read(size_t i)
{
    objMutex.lock();
    int tmp = data[i];
    objMutex.unlock();
    return tmp;
}

void LockObjectArray::write(size_t i, int val)
{
    objMutex.lock();
    data[i] = val;
    objMutex.unlock();
}




//-----LockElementArray-----//

LockElementArray::LockElementArray(const std::vector<int> & vec): LockArray(vec)
{
    mtx = std::vector<std::mutex>(vec.size());
}

size_t LockElementArray::size()
{
    return data.size();
}

int LockElementArray::read(size_t i)
{
    assert(inBounds(i));
    mtx[i].lock();
    int tmp = data[i];
    mtx[i].unlock();
    return tmp;
}

void LockElementArray::write(size_t i, int val)
{
    assert(inBounds(i));
    mtx[i].lock();
    data[i] = val;
    mtx[i].unlock();;
}




//-----LockRangeArray-----//

LockRangeArray::LockRangeArray(const std::vector<int> & vec): LockArray(vec)
{
    mtx = std::vector<std::mutex>(vec.size());
}

void LockRangeArray::lockRange(unsigned s, unsigned e)
{
    if(s >= e) return;
    assert(inBounds(s) && inBounds(e - 1));
    for(; s != e; s++)
    {
        mtx[s].lock();
    }
}

void LockRangeArray::unlockRange(unsigned s, unsigned e)
{
    if(s >= e) return;
    assert(inBounds(s) && inBounds(e - 1));
    for(; s != e; s++)
    {
        mtx[s].unlock();
    }
}

size_t LockRangeArray::size()
{
    return data.size();
}

int LockRangeArray::read(size_t i)
{
    return 0;
}

void LockRangeArray::write(size_t i, int val)
{
    
}













//std::mutex LockClassArray::arrMutex = std::mutex();
