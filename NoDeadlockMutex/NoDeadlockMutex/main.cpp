//
//  main.cpp
//  NoDeadlockMutex
//
//  Created by Foma Mironenko on 24.09.2020.
//

#include <iostream>
#include <chrono>
#include <thread>

#include "MyMutex.hpp"

using namespace std::chrono_literals;


void twoMx(NoDeadlockMutex * mx1, NoDeadlockMutex * mx2)
{
    LockMaster master;
    master.pred_lock(mx1);
    master.pred_lock(mx2);
    
    master.lock_now();
    for(int i = 0; i <= 1e8; i++)
    {
        if(i % (int)1e7 == 0) {
            std::cout << std::this_thread::get_id() << ":  " << i/(int)1e7 << "\n";
        }
    }
    std::cout << "\n";
    master.unlock_now();
}


int main()
{
    NoDeadlockMutex *mx1 = new NoDeadlockMutex();
    NoDeadlockMutex *mx2 = new NoDeadlockMutex();
    
    std::thread t1(twoMx, mx1, mx2);
    std::thread t2(twoMx, mx2, mx1);
    t1.join();
    t2.join();
    
    std::cout << "\nPROCESS FINISHED\n\n";
    
    delete mx1;
    delete mx2;
    
    return 0;
}
