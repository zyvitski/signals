

#ifndef signal_t_h
#define signal_t_h


#include <map>
#include <functional>
#include <vector>
#include <future>
#include <atomic>
#include <mutex>
#include <type_traits>



namespace signals{

    namespace{



        template <typename... slot_signature_t>
        class signal_base;

        template <typename return_t,typename... args_t>
        class signal_base<return_t(args_t...)>{
        public:
            using return_type = return_t;
            using result_type = std::vector<return_type>;
            using slot_t = std::function<return_t(args_t...)>;
            using slot_id_t = std::size_t;

            signal_base():_current_id(0){}

            signal_base(signal_base const& other):_slots(other._slots),_current_id(other._current_id){}

            signal_base(signal_base&& other):_slots(),_current_id(0)
            {
                std::swap(_slots,other._slots);
                std::swap(_current_id,other._current_id);
            }

            signal_base& operator=(signal_base other)
            {
                std::swap(_slots,other._slots);
                std::swap(_current_id,other._current_id);
                return *this;
            }

            signal_base& operator=(signal_base&& other)
            {
                if(this != &other)
                {
                    std::swap(_slots,other._slots);
                    std::swap(_current_id,other._current_id);
                }
                return *this;
            }
            slot_id_t connect(slot_t const& slot)
            {
                slot_id_t out;
                {
                    std::unique_lock<std::mutex> lk{_slots_mtx};
                    _slots.emplace(_current_id,slot);
                    out = _current_id++;
                }
                return out;
            }
            template<typename F,typename... a_t>
            slot_id_t attach(F&& f, a_t&&... args)
            {
                return connect([&](args_t... a)
                {
                    return f(args...,a...);
                });
            }



            slot_t disconnect(slot_id_t const& id)
            {
                std::unique_lock<std::mutex> lk{_slots_mtx};
                if( ! ( id > _current_id -1) )
                {
                    slot_t s =_slots.at(id);
                    _slots.erase(id);
                    return s;
                }else return nullptr;
            }

            void disconnect_all()
            {
                std::unique_lock<std::mutex> lk{_slots_mtx};

                _slots.clear();
            }
            template<typename thread_pool>
            std::vector<std::future<return_t>> emit(thread_pool& pool, args_t... p)
            {

                std::unique_lock<std::mutex> lk{_slots_mtx};

                std::vector<std::future<return_t>> out(_slots.size());

                for (auto&& it:_slots)
                {
                    out[it.first]=std::move(pool.push(it.second, p...));
                }

                return out;
            }
            template<typename thread_pool>
            std::future<return_t> emit(thread_pool& pool, slot_id_t target,args_t... p)
            {
                std::unique_lock<std::mutex> lk{_slots_mtx};

                return pool.push(_slots[target], p...);

            }
        protected:
            std::map<slot_id_t, slot_t>_slots;
            slot_id_t _current_id;

            std::mutex _slots_mtx;
        };

    }

    template <typename... slot_signature_t>
    class signal_t;

    template <typename... args_t>
    class signal_t<void(args_t...)> : signal_base<void(args_t...)> {
    public:
        using base_type = signal_base<void(args_t...)>;
        using return_type  = typename base_type::return_type;
        using result_type = typename base_type::result_type;
        using slot_t = typename base_type::slot_t;
        using slot_id_t = typename base_type::slot_id_t;

        using base_type::base_type;
        using base_type::operator=;
        using base_type::connect;
        using base_type::attach;
        using base_type::disconnect;
        using base_type::disconnect_all;
        using base_type::emit;

        //calls all slots synchronously
        void emit(args_t... p)
        {
            std::unique_lock<std::mutex> lk{_slots_mtx};
            for(auto&& it :_slots)
            {
                it.second(p...);
            }
        }
        void emit(slot_id_t target, args_t... p)
        {
            std::unique_lock<std::mutex> lk{_slots_mtx};
            _slots[target](p...);
        }
    protected:
        using base_type::_slots;
        using base_type::_current_id;
        using base_type::_slots_mtx;

    };




    template <typename return_t,typename... args_t>
    class signal_t<return_t(args_t...)> : signal_base<return_t(args_t...)>
    {
    public:
        using base_type = signal_base<return_t(args_t...)>;
        using return_type  = typename base_type::return_type;
        using result_type = typename base_type::result_type;
        using slot_t = typename base_type::slot_t;
        using slot_id_t = typename base_type::slot_id_t;

        using base_type::base_type;
        using base_type::operator=;
        using base_type::connect;
        using base_type::disconnect;
        using base_type::disconnect_all;
        using base_type::emit;


        //calls all slots synchronously
        result_type emit(args_t... p)
        {
            std::unique_lock<std::mutex> lk{_slots_mtx};

            result_type out(_slots.size());

            for(auto&& it :_slots)
            {
                out[it.first]=it.second(p...);
            }
            return out;
        }
        return_t emit(slot_id_t target, args_t... p)
        {
            std::unique_lock<std::mutex> lk{_slots_mtx};
            return _slots[target](p...);
        }



    protected:
        using base_type::_slots;
        using base_type::_current_id;
        using base_type::_slots_mtx;

    };

}

#endif /* signal_t_h */
