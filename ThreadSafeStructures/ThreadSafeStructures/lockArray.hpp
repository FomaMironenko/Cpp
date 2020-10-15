#ifndef lockArray_hpp
#define lockArray_hpp


#include <iostream>
#include <vector>
#include <thread>
#include <cassert>


class LockArray
{
public:
    explicit LockArray(const std::vector<int> & vec)
    {
        data = vec;
    }
    virtual size_t size();
    virtual int read(size_t i) = 0;
    virtual void write(size_t i, int val) = 0;
    
protected:
    bool inBounds(size_t i)
    {
        return i >= 0 && i <= data.size();
    }
    
    std::vector<int> data;
};


// HARD IMPLEMENTATION
class LockClassArray: public LockArray
{
public:
    LockClassArray(const std::vector<int> & vec): LockArray(vec)
    {   }
    static void lock();
    static void unlock();
    
    size_t size();
    int read(size_t i);
    void write(size_t i, int val);
    
private:
    static std::mutex flagOwner;
    static bool hasOwner;
    static std::thread::id owner;
    
    static std::mutex arrMutex;
};




class LockObjectArray: LockArray
{
public:
    explicit LockObjectArray(const std::vector<int> & vec): LockArray(vec)
    {    }
    
    size_t size();
    int read(size_t i);
    void write(size_t i, int val);
    
private:
    std::mutex objMutex;
};



class LockElementArray: LockArray
{
public:
    explicit LockElementArray(const std::vector<int> & vec);
    
    size_t size();
    int read(size_t i);
    void write(size_t i, int val);
    
private:
    std::vector<std::mutex> mtx;
};


class LockRangeArray: LockArray
{
public:
    explicit LockRangeArray(const std::vector<int> & vec);
    
    void lockRange(unsigned s, unsigned e);
    void unlockRange(unsigned s, unsigned e);
    
    size_t size();
    int read(size_t i);
    void write(size_t i, int val);
private:
    std::vector<std::mutex> mtx;
};


#endif /* lockArray_hpp */
