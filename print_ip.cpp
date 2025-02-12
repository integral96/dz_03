#include <iostream>
#include <utility>
#include <vector>
#include <list>
#include <tuple>

using namespace std;

template <typename Arg>
void print_ip(Arg&& arg) {
    if constexpr(is_same_v<Arg, int8_t> || is_same_v<Arg, int16_t>) {
        int sdc = static_cast<unsigned char>(forward<Arg>(arg));
        cout << sdc;
    } else if constexpr(is_same_v<Arg, int32_t>) {
        cout << ((forward<Arg>(arg) & 0xff000000) >> 24) << '.' << ((forward<Arg>(arg) & 0x00ff0000) >> 16)
             << '.' << ((forward<Arg>(arg) & 0x0000ff00) >> 8) << '.' << ((forward<Arg>(arg) & 0x000000ff));
    } else if constexpr(is_same_v<Arg, int64_t>) {
        for(int i = 7; i >= 0; --i) {
            if(i == 0)
                cout << ((forward<Arg>(arg) & (0xff00000000000000 >> 8*(7 - i))) >> (i*8));
            else
                cout << ((forward<Arg>(arg) & (0xff00000000000000 >> 8*(7 - i))) >> (i*8)) << '.';
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
    } else {
        cout << forward<Arg>(arg);
    }
    cout << endl;

}

int main()
{
    //print_ip(123, 456, 789, 0);

    print_ip( int8_t{-1} ); // 255
    print_ip( int16_t{0} ); // 0.0
    print_ip( int32_t{2130706433} ); // 127.0.0.1
    print_ip( int64_t{8875824491850138409} );// 123.45.67.89.101.112.131.41
    print_ip( string{"Hello, World!"} ); // Hello, World!
    print_ip( vector<int>{100, 200, 300, 400} ); // 100.200.300.400
    print_ip( list<short>{400, 300, 200, 100} ); // 400.300.200.100
    print_ip( make_tuple(123, 456, 789, 0) ); // 123.456.789.0
    return 0;
}
