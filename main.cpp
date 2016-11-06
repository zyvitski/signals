//
//  main.cpp
//  playground
//
//  Created by Alexander zywicki on 10/30/16.
//  Copyright © 2016 Alexander zywicki. All rights reserved.
//


#include "signal.hpp"
#include <iostream>
#include <numeric>



int main(int argc, const char * argv[]) {
    thread_pool pool;
    
    
    signals::signal_t<int(int)> sig;
    auto&& id = signals::connect(sig, [](int i){
        return i;
    });
    auto&& res = signals::emit(sig,pool, 0);
    auto&& val = signals::result(id,res);
    std::cout<<val<<std::endl;
    
    return 0;
}
