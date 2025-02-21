
#include <vector>
#include <iostream>
#include <string>
#include "my_tuple.hpp"


template <typename Arg, typename std::enable_if_t<(std::is_same_v<Arg, int8_t> || std::is_same_v<Arg, int16_t>), bool> = true>
void print_ip(Arg&& arg) {
    int16_t sdc = static_cast<unsigned char>(std::forward<Arg>(arg));
    std::cout << sdc << std::endl;
}
template <typename Arg, typename std::enable_if_t<std::is_same_v<Arg, int32_t>, bool> = true>
void print_ip(Arg&& arg) {
    int32_t sdc = std::forward<Arg>(arg);
    std::cout << ((sdc & 0xff000000) >> 24) << '.' << ((sdc & 0x00ff0000) >> 16)
         << '.' << ((sdc & 0x0000ff00) >> 8) << '.' << ((sdc & 0x000000ff)) << std::endl;
}
template <typename Arg, typename std::enable_if_t<std::is_same_v<Arg, int64_t>, bool> = true>
void print_ip(Arg&& arg) {
    int64_t sdc = std::forward<Arg>(arg);
    for(int i = 7; i >= 0; --i) {
        if(i == 0)
            std::cout << ((sdc & (0xff00000000000000 >> 8*(7 - i))) >> (i*8));
        else
            std::cout << ((sdc & (0xff00000000000000 >> 8*(7 - i))) >> (i*8)) << '.';
    }
    std::cout << std::endl;
}
template <typename Arg, typename std::enable_if_t<std::is_same_v<Arg, std::string>, bool> = true>
void print_ip(Arg&& arg) {
    std::cout << std::forward<Arg>(arg) << std::endl;
}
template <typename Arg, typename std::enable_if_t<(std::is_same_v<Arg, std::vector<int>> || std::is_same_v<Arg, std::list<short>>), bool> = true>
void print_ip(Arg&& arg) {
    const auto& vec = std::forward<Arg>(arg);
    int i = 0;
    for(const auto& x : vec ) {
        if(i == (int)vec.size() - 1)
            std::cout << x;
        else
            std::cout << x << '.';
        ++i;
    }
    std::cout << std::endl;
}
template <typename Arg, typename std::enable_if_t<std::is_same_v<Arg, std::tuple<int, int, int, int>>, bool> = true>
void print_ip(Arg&& arg) {
    const auto& vec = std::forward<Arg>(arg);
    std::cout << std::get<0>(vec) << '.' << std::get<1>(vec)
         << '.' << std::get<2>(vec) << '.' << std::get<3>(vec) << std::endl;
}
template <typename Arg, typename std::enable_if_t<std::is_same_v<Arg, my_tuple<int, int, int, int>>, bool> = true>
void print_ip(Arg&& arg) {
    const auto& vec = std::forward<Arg>(arg);
    std::cout << my_get<0>(vec) << '.' << my_get<1>(vec)
         << '.' << my_get<2>(vec) << '.' << my_get<3>(vec) << std::endl;
}


int main()
{

    print_ip( int8_t{-1} ); // 255
    print_ip( int16_t{0} ); // 0.0
    print_ip( int32_t{2130706433} ); // 127.0.0.1
    print_ip( int64_t{8875824491850138409} );// 123.45.67.89.101.112.131.41
    print_ip( std::string{"Hello, World!"} ); // Hello, World!
    print_ip( std::vector<int>{100, 200, 300, 400} ); // 100.200.300.400
    print_ip( std::list<short>{400, 300, 200, 100} ); // 400.300.200.100
    print_ip( std::make_tuple(123, 456, 789, 0) ); // 123.456.789.0
    print_ip( make_my_tuple(400, 300, 200, 100) ); // 400.300.200.100

    return 0;
}
