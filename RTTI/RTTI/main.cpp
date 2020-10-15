//
//  main.cpp
//  RTTI
//
//  Created by Foma Mironenko on 15.10.2020.
//

#include <iostream>
#include "JsonClass.hpp"


int main()
{
    Number n(2);
    String s("asd");
    Number m(4);
    std::vector<Data *> v = {&n, &m, &s};
    List l(v);
    Float f(5.);
    
    Json j1 (&n);
    Json j2 (&s);
    Json j3 (&m);
    Json j4 (&l);
    Json j5 (&f);
    
    std::cout << (j4 < j1) << "\n";
    
}
