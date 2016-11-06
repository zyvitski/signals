# signals [![Build Status](https://travis-ci.org/zyvitski/signals.svg?branch=master)](https://travis-ci.org/zyvitski/signals)

A Signals and Slots implementation using C++11

Basic Usage:

    //create a signal of type double(std::vector<double> const&)
    signals::signal_t<double(std::vector<double> const&)> sig;

    //connect slots and get the slot ids
    auto&& id = signals::connect(sig,[](std::vector<double> const& v){
        return std::accumulate(v.begin(),v.end(),0);
    });
    auto&& id2 = signals::connect(sig,[](std::vector<double> const& v){
        return std::accumulate(v.begin(),v.end(),1,std::multiplies<double>{});
    });
    //data to be emitted
    std::vector<double> v = {1,2,3,4,5};

    //call all slots with data v
    auto&& em = signals::emit(sig,v);
    //get result of emit
    auto&& res = signals::result(em);
    //print all
    for(auto&& r: res){
        std::cout<<r<<std::endl;
    }
Access a result from a specific slot:

    //get the result of a specific slot by using its id
    auto&& slot_id1_result = signals::result(id,em);
    std::cout<<slot_id1_result<<std::endl;

Emit Asynchronously with a thread pool:

    //a thread pool. see: thread_pool.hpp
    thread_pool pool;
    //call emit async using a thread pool
    auto&& em2 = signals::emit(sig,pool,v);

    //get results, when using pool this will wait on the futures provided by the pool for each emit
    auto&& res2 = signals::result(em2);

    //print
    for(auto&& r: res2){
        std::cout<<r<<std::endl;
    }

