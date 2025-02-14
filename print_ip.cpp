#include <type_traits>
#include <utility>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
#include <list>

using namespace std;

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
struct my_tuple_impl<index_sequence<K...>, Args...> : init<K, Args>... {
    static constexpr size_t size_ = sizeof...(Args);
    template<typename ...Args1>
    explicit constexpr my_tuple_impl(Args1&&... args) : init<K, Args>(static_cast<Args1&&>(args))... {}
};
template<typename ...Args>
struct my_tuple : my_tuple_impl<make_index_sequence<sizeof...(Args)>, Args...> {
    using base = my_tuple_impl<make_index_sequence<sizeof...(Args)>, Args...>;
    template<typename ...Args1, enable_if_t<base::size_ == sizeof...(Args1), bool> = true>
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
struct unwrap_<reference_wrapper<V>> {
    using type = V&;
};
template<typename V>
using unwrap_t = typename unwrap_<decay_t<V>>::type;
template<typename ...Args>
constexpr my_tuple<unwrap_t<Args>...> make_my_tuple(Args&&... value) {
    return my_tuple<unwrap_t<Args>...>(forward<Args>(value)...);
}

template <typename Arg>
void print_ip(Arg&& arg) {
    if constexpr(is_same_v<Arg, int8_t> || is_same_v<Arg, int16_t>) {
        int16_t sdc = static_cast<unsigned char>(forward<Arg>(arg));
        cout << sdc;
    } else if constexpr(is_same_v<Arg, int32_t>) {
        int32_t sdc = forward<Arg>(arg);
        cout << ((sdc & 0xff000000) >> 24) << '.' << ((sdc & 0x00ff0000) >> 16)
             << '.' << ((sdc & 0x0000ff00) >> 8) << '.' << ((sdc & 0x000000ff));
    } else if constexpr(is_same_v<Arg, int64_t>) {
        int64_t sdc = forward<Arg>(arg);
        for(int i = 7; i >= 0; --i) {
            if(i == 0)
                cout << ((sdc & (0xff00000000000000 >> 8*(7 - i))) >> (i*8));
            else
                cout << ((sdc & (0xff00000000000000 >> 8*(7 - i))) >> (i*8)) << '.';
        }
    } else if constexpr(is_same_v<Arg, vector<int>> || is_same_v<Arg, list<short>>) {
        const auto& vec = forward<Arg>(arg);
        int i = 0;
        for(const auto& x : vec ) {
            if(i == vec.size() - 1)
                cout << x;
            else
                cout << x << '.';
            ++i;
        }
    } else if constexpr(is_same_v<Arg, tuple<int, int, int, int>>) {
        const auto& vec = forward<Arg>(arg);
        cout << get<0>(vec) << '.' << get<1>(vec)
             << '.' << get<2>(vec) << '.' << get<3>(vec);
    } else if constexpr(is_same_v<Arg, my_tuple<int, int, int, int>>) {
        const auto& vec = forward<Arg>(arg);
        cout << my_get<0>(vec) << '.' << my_get<1>(vec)
             << '.' << my_get<2>(vec) << '.' << my_get<3>(vec);
    } else {
        cout << forward<Arg>(arg);
    }
    cout << endl;

}
int main()
{

    print_ip( int8_t{-1} ); // 255
    print_ip( int16_t{0} ); // 0.0
    print_ip( int32_t{2130706433} ); // 127.0.0.1
    print_ip( int64_t{8875824491850138409} );// 123.45.67.89.101.112.131.41
    print_ip( string{"Hello, World!"} ); // Hello, World!
    print_ip( vector<int>{100, 200, 300, 400} ); // 100.200.300.400
    print_ip( list<short>{400, 300, 200, 100} ); // 400.300.200.100
    print_ip( make_tuple(123, 456, 789, 0) ); // 123.456.789.0
    print_ip( make_my_tuple(400, 300, 200, 100) ); // 400.300.200.100

    return 0;
}
