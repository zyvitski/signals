


#include "signal.hpp"
#include "property.hpp"
#include <iostream>
#include <numeric>

int main(int argc, const char * argv[]) {
    value_property<int> prop;
    prop = 8;
    value_property<int> p = prop;
    std::cout<<p<<std::endl;
    return 0;
}
