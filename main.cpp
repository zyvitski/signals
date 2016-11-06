


#include "signal.hpp"
#include <iostream>
#include <numeric>

int main(int argc, const char * argv[]) {
    signals::signal_t<double(std::vector<double> const&)> sig;
    auto&& id = signals::connect(sig,[](std::vector<double> const& v){
        return std::accumulate(v.begin(),v.end(),0);
    });
    std::vector<double> v = {1,2,3,4,5};
    auto&& result = signals::result(id,signals::emit(sig,v));
    std::cout<<result<<std::endl;

    return 0;
}
