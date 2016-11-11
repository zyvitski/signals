


#include "signal.hpp"
#include "property.hpp"
#include <iostream>
#include <numeric>

int main(int argc, const char * argv[]) {
    int i=0;

    property<int> prop
    {
        [&](int val){
            i=val;
        },
        [&](){
            return i;
        }
    };

    prop = 8;
    std::cout<<prop<<std::endl;
    return 0;
}
