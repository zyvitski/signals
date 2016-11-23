#include "signal.hpp"
#include "property.hpp"
#include <iostream>
#include <numeric>
#include <thread_pool.hpp>

int main(int argc, const char * argv[]) {
    thread_pool pool;

    signals::signal_t<void()> sig;
    signals::emit(sig,pool);
    return 0;
}
