//
//  main.cpp
//  openMPintro
//
//  Created by Foma Mironenko on 03.09.2020.
//  Copyright © 2020 Foma Mironenko. All rights reserved.
//

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>


template<template<typename...> class X>
struct foo
{
    X<int> container;
    void push(int a)
    {
        container.push(a);
    }
    int top()
    {
        return container.top();
    }
    void pop()
    {
        container.pop();
    }
};


int main()
{
    foo<std::queue> Qcon;
    foo<std::stack> Scon;
    for(int i = 0; i < 5; i++)
    {
        Qcon.push(i);
        Scon.push(i);
    }
    for(int i = 0; i < 5; i++)
    {
        std::cout << Qcon.top() << " " << Scon.top() << std::endl;
        Qcon.pop();
        Scon.pop();
    }
    
    
    //std::list<int>   lst = f<list>(3);
    return 0;
}
