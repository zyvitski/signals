#ifndef _property_hpp_
#define _property_hpp_

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

template<typename T>
class value_property : public property<T>
{
public:
    using base_type = property<T>;
    using set_slot_t = typename base_type::set_slot_t;
    using get_slot_t = typename base_type::get_slot_t;
    value_property():base_type(std::bind(&value_property<T>::_set,this,std::placeholders::_1),std::bind(&value_property<T>::_get,this)){}
    value_property(set_slot_t&& setter, get_slot_t&& getter):base_type(std::forward<set_slot_t&&>(setter),std::forward<get_slot_t&&>(getter)){}
    using base_type::operator T;
    using base_type::operator=;
private:
    T value;
    void _set(T&& v){
        value =v;
    }
    T _get(){
        return value;
    }
};

#endif
