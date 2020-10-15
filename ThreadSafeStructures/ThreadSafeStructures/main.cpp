#include <iostream>
#include <stdio.h>
#include <vector>

#include "lockArray.hpp"



int main() {
    std::vector<int> v = {2, 3, 4, 4};
    v.push_back(5);
    LockClassArray arr(v);
    
    
    
    return 0;
}
