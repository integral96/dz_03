#include <type_traits>
#include <iostream>
#include <numeric>
#include <map>
#include <memory>

#include <cmath>

constexpr size_t N_all = 300;

template <int N, int I, class Closure>
typename std::enable_if_t<(I == N)> is_meta_loop(Closure&) {}

template <int N, int I, class Closure>
typename std::enable_if_t<(I < N)> is_meta_loop(Closure& closure) {
    closure.template apply<I>();
    is_meta_loop<N, I + 1>(closure);
}
template <int N, class Closure>
inline void meta_loop(Closure& closure) {
    is_meta_loop<N, 3>(closure);
}

size_t count_devider(size_t sum) {
    size_t c = 0;
    for (size_t i = 1; i <= sqrt(sum); i++) {
        if (sum % i == 0) {
            if (sum / i == i)
                c++;
            else
                c += 2;
        }
    }
    return c;
}

struct sequence_k {
private:
    std::array<size_t, N_all> start_arr;
    std::array<size_t, N_all> deviders;
public:
    sequence_k() {
        std::iota(start_arr.begin(), start_arr.end(), 1);
        size_t summ = 0;
        for(size_t i = 0; i < N_all; ++i) {
            summ += start_arr[i];
            deviders[i] = (count_devider(summ));
        }
    }
    template<size_t N>
    void apply () {
        std::cout << N << ": \n";
        std::array<size_t, N> a;
        auto result = std::make_unique<std::map<size_t, std::array<size_t, N>>>();
        for(const auto & x : deviders) {
            a[N - 1] = x;
            a[0] = 1;
            for(auto str : start_arr) {
                size_t sum = a[0];
                for(size_t i = 1; i < N - 1; ++i) {
                    a[i] = ++str;
                    sum += a[i];
                }
                sum += a[N - 1];

                if(count_devider(sum) == a[N - 1]) {
                    result->insert(std::make_pair(sum, a));
                    break;
                }
            }
        }
        for(const auto & x : *result) {
            std::cout << "\t";
            for(const auto & y : x.second) {
                std::cout << y << " ";
            }
            std::cout << "\n";
        }
    }
};

template<size_t N>
void print_sequence() {
    sequence_k closure;
    meta_loop<N>( closure);
}

int main()
{
    print_sequence<10>();

    return 0;
}
