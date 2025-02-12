#include <string>
#include <type_traits>

using namespace std;

template<int N, int I>
struct is_prime_impl : conditional_t<(N%I == 0), false_type, is_prime_impl<N, I - 1>> {};

template<int N>
struct is_prime_impl<N, 1> : true_type {};

template<int N>
struct is_prime : is_prime_impl<N, N / 2>{};

template<size_t N>
struct find_prime : conditional_t<is_prime<N>::value, integral_constant<size_t, N>, find_prime<N + 2>> {};

template<size_t N>
struct prime_at : find_prime<prime_at<N - 1>::value + 2>{};
template<>
struct prime_at<0> : integral_constant<size_t, 2>{};

template<>
struct prime_at<1> : integral_constant<size_t, 3>{};

template<int... I>
struct pr_vector {};
template<class IntVector, int i> struct push_back;

template<int... I, int i>
struct push_back<pr_vector<I...>, i> {
    using type = pr_vector<I..., i>;
};
template<class IntVector, int n>
struct at;

template<int i, int... I, int n>
struct at<pr_vector<i, I...>, n> : at<pr_vector<I...>, n - 1> {};

template<int i, int... I>
struct at<pr_vector<i, I...>, 0> : integral_constant<int, i> {};

template<unsigned n>
struct primes {
    using type = typename push_back<
                                typename primes<n - 1>::type,
                                prime_at<n - 1>::value
                                >::type;
};
template<>
struct primes<0> {
    using type = pr_vector<>;
};

struct SomeStruct {

	template <typename TT,
			  typename Fake =
				  typename std::enable_if<
					  !std::is_same<typename std::remove_reference<TT>::type,
									SomeStruct>::value,
					  void>::type>
	SomeStruct(TT &&value) : m_value{std::forward<TT>(value)} {
	}

	SomeStruct(const SomeStruct &other) : m_value{other.m_value} {
	}

	const std::string &getValue() const {
		return m_value;
	}

private:
	std::string m_value;
	std::string m_value2;
};

int main() {

    using primes_t =  primes<120>::type;

    primes_t prim;
    prim = 1;
	SomeStruct first("Hello, World!");
	SomeStruct second = first;

	return 0;
}
