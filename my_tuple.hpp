#ifndef MY_TUPLE_HPP
#define MY_TUPLE_HPP
#include <type_traits>
#include <utility>
#include <functional>
#include <iterator>
#include <algorithm>
#include <list>

template<size_t K, typename V>
struct init {
    V value;
    constexpr init() : value() {}
    template<typename T>
    explicit constexpr init(T&& value_) : value(static_cast<T&&>(value_)) {}
};
template<size_t K, typename V>
constexpr const V& _get(const init<K, V>& x) {
    return x.value;
}
template<size_t K, typename V>
constexpr V& _get(init<K, V>& x) {
    return x.value;
}
template<size_t K, typename V>
constexpr V&& _get(init<K, V>&& x) {
    return static_cast<V&&>(x.value);
}
template<typename K, typename ...Args>
struct my_tuple_impl;
template<size_t ...K, typename ...Args>
struct my_tuple_impl<std::index_sequence<K...>, Args...> : init<K, Args>... {
    static constexpr size_t size_ = sizeof...(Args);
    template<typename ...Args1>
    explicit constexpr my_tuple_impl(Args1&&... args) : init<K, Args>(static_cast<Args1&&>(args))... {}
};
template<typename ...Args>
struct my_tuple : my_tuple_impl<std::make_index_sequence<sizeof...(Args)>, Args...> {
    using base = my_tuple_impl<std::make_index_sequence<sizeof...(Args)>, Args...>;
    template<typename ...Args1, std::enable_if_t<base::size_ == sizeof...(Args1), bool> = true>
    explicit constexpr my_tuple(Args1&&... args) : base(static_cast<Args1&&>(args)...) {}
};
template<size_t K, typename ...Args>
constexpr decltype(auto) my_get(const my_tuple<Args...>& value) {
    return _get<K>(value);
}
template<size_t K, typename ...Args>
constexpr decltype(auto) my_get(my_tuple<Args...>& value) {
    return _get<K>(value);
}
template<size_t K, typename ...Args>
constexpr decltype(auto) my_get(my_tuple<Args...>&& value) {
    return _get<K>(static_cast<my_tuple<Args...>&&>(value));
}
// создаем мейк тюпл
template<typename V>
struct unwrap_ {
    using type = V;
};
template<typename V>
struct unwrap_<std::reference_wrapper<V>> {
    using type = V&;
};
template<typename V>
using unwrap_t = typename unwrap_<std::decay_t<V>>::type;
template<typename ...Args>
constexpr my_tuple<unwrap_t<Args>...> make_my_tuple(Args&&... value) {
    return my_tuple<unwrap_t<Args>...>(std::forward<Args>(value)...);
}

#endif // MY_TUPLE_HPP
