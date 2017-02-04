#ifndef _property_hpp_
#define _property_hpp_

#include <signal.hpp>

template<typename T>
class property
{
private:
    signals::signal_t<void(const T&)> _set;
    signals::signal_t<T()> _get;
public:
    using set_signal_t = signals::signal_t<void(const T&)>;
    using set_slot_t = typename set_signal_t::slot_t;
    using get_signal_t = signals::signal_t<T()>;
    using get_slot_t = typename get_signal_t::slot_t;
    property(){}
    property(set_slot_t&& setter, get_slot_t&& getter)
    {
        signals::connect(_set,std::forward<set_slot_t&&>(setter));
        signals::connect(_get,std::forward<get_slot_t&&>(getter));
    }
    property(set_signal_t&& setter,get_signal_t&& getter):_set(setter),_get(getter){}
    property(const property& other):_set(other._set),_get(other._get){}
    property(property&& other):_set(std::move(other._set)),_get(std::move(other._get)){}

    property& operator=(property const& other)
    {
        property o(other);
        std::swap(_set,o._set);
        std::swap(_get,o._get);
        return *this;
    }
    property& operator=(property&& other)
    {
        _set = std::move(other._set);
        _get = std::move(other._get);
        return *this;
    }
    property& operator=(const T& value)
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
    void on_get(get_slot_t&& action)
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
    using set_signal_t = typename base_type::set_signal_t;
    using get_signal_t = typename base_type::get_signal_t;

    value_property():base_type(std::bind(&value_property<T>::_set,this,std::placeholders::_1),
                               std::bind(&value_property<T>::_get,this)),
                               value(){}
    value_property(set_slot_t&& setter,
                   get_slot_t&& getter):base_type(std::forward<set_slot_t&&>(setter),
                                                  std::forward<get_slot_t&&>(getter)),
                                                  value(){}
    value_property(set_signal_t&& setter,
                   get_signal_t&& getter):base_type(std::forward<set_signal_t&&>(setter),
                                                    std::forward<get_signal_t&>(getter)),
                                                    value(){}

    using base_type::operator T;
    using base_type::operator=;
protected:
    T value;
    void _set(const T& v){
        value =v;
    }
    T _get(){
        return value;
    }
};

#endif
