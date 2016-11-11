#ifndef __property_hpp__
#define __property_hpp__

#include <signal.hpp>

template<typename T>
class property
{
private:
    signals::signal_t<void(T)> _set;
    signals::signal_t<T()> _get;
public:
    using set_slot_t = typename decltype(_set)::slot_t;
    using get_slot_t = typename decltype(_get)::slot_t;
    property(){}
    property(set_slot_t&& setter, get_slot_t&& getter)
    {
        signals::connect(_set,std::forward<set_slot_t&&>(setter));
        signals::connect(_get,std::forward<get_slot_t&&>(getter));
    }
    property& operator=(T value)
    {
        signals::emit(_set,value);
        return *this;
    }
    operator T()
    {
        return signals::result(0,signals::emit(_get));
    }
    void on_set(set_slot_t&& action)
    {
        _set.disconnect_all();
        signals::connect(_set,std::forward<set_slot_t&&>(action));
    }
    void on_get(typename decltype(_get)::slot_t&& action)
    {
        _get.disconnect_all();
        signals::connect(_get,std::forward<get_slot_t&&>(action));
    }
};

#endif
