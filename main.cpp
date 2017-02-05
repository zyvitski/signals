#include "signal.hpp"
#include "property.hpp"
#include <iostream>
#include <numeric>
#include <thread_pool.hpp>

int main(int argc, const char * argv[]) {
    thread_pool pool;

    signals::signal_t<int(int)> sig;
    sig.connect([](int i){return i;});
    auto&& result = signals::result(0,signals::emit(sig,pool,0));
    std::cout<<result<<std::endl;
    return 0;
}
