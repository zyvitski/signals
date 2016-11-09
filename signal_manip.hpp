

#ifndef signal_manip_h
#define signal_manip_h

#include <utility>
#include <vector>
#include <load_balancing_thread_pool.hpp>

namespace signals {

    template<typename s_t>
    typename s_t::slot_id_t connect(s_t const& sig, typename s_t::slot_t&& slot)
    {
        return const_cast<s_t&>(sig).connect(std::forward<typename s_t::slot_t&&>(slot));
    }

    template<typename s_t>
    typename s_t::slot_t disconect(s_t const& sig, typename s_t::slot_id_t const& id)
    {
        return const_cast<s_t&>(sig).disconnect(id);
    }


    template<typename s_t, typename... args_t>
    auto emit(s_t const& sig, args_t&&... args) -> decltype(std::declval<s_t>().emit(std::declval<args_t>()...))
    {
        return const_cast<s_t&>(sig).emit(std::forward<args_t&&>(args)...);
    }

    template<typename s_t, typename s_id_t,typename... args_t>
    auto emit(s_t const& sig, s_id_t const& target, args_t&&...args)->decltype(std::declval<s_t>().emit(std::declval<s_id_t>(),
                                                                                                        std::declval<args_t>()...))
    {
        return const_cast<s_t&>(sig).emit(target,std::forward<args_t&&>(args)...);
    }

    template<typename s_t, typename... args_t>
    auto emit(s_t const& sig,thread_pool const& pool, args_t&&... args) ->decltype(std::declval<s_t>().emit(std::declval<thread_pool>(),
                                                                                                            std::declval<args_t>()...))
    {
        return const_cast<s_t&>(sig).emit(pool,std::forward<args_t&&>(args)...);
    }

    template<typename s_t, typename s_id_t,typename... args_t>
    auto emit(s_t const& sig,thread_pool const& pool,  s_id_t const& target, args_t&&...args) ->decltype(std::declval<s_t>().emit(std::declval<s_t>(),
                                                                                                                                  std::declval<thread_pool>(),
                                                                                                                                  std::declval<args_t>()...))
    {
        return const_cast<s_t&>(sig).emit(target,pool,std::forward<args_t&&>(args)...);
    }




    template<typename T,typename sid_t>
    T result(sid_t&& sid,std::vector<std::future<T>>&& v){
        return v[sid].get();
    }
    template<typename T,typename sid_t>
    T result(sid_t& sid,std::vector<std::future<T>>& v){
        return v[sid].get();
    }
    template<typename T,typename sid_t>
    T result(sid_t&& sid,std::vector<T>&& v){
        return v[sid];
    }
    template<typename T,typename sid_t>
    T result(sid_t& sid,std::vector<T>& v){
        return v[sid];
    }

    template<typename T>
    std::vector<T> result(std::vector<std::future<T>>&& v){
        std::vector<T> out(v.size());
        for(std::size_t i=0;i<v.size();++i){
            out[i]=v[i].get();
        }
        return out;
    }
    template<typename T>
    std::vector<T> result(std::vector<std::future<T>>& v){
        std::vector<T> out(v.size());
        for(std::size_t i=0;i<v.size();++i){
            out[i]=v[i].get();
        }
        return out;
    }
    template<typename T>
    inline std::vector<T>& result(std::vector<T>&& v){
        return v;
    }
    template<typename T>
    inline std::vector<T>& result(std::vector<T>& v){
        return v;
    }
    template<typename T>
    inline T& result(std::future<T>&& v){
        return v.get();
    }
    template<typename T>
    inline T result(std::future<T>& v){
        return v.get();
    }
}

#endif /* signal_manip_h */
