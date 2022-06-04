#include <iostream>
#include <utility>
import Delta;

/*
	make a new array type that satisfies concept Array explicitly 
	by defining a template specialization for array_policies_t 
*/

template <typename T, auto N>
struct super_array1 {
	T data [N];
};

template <typename T, auto N>
struct array_policies_t <super_array1 <T, N>> {
	using type = super_array1 <T, N>;
	static auto* begin (type& s) noexcept {return s.data;}
	static auto* end (type& s) noexcept {return s.data + N;}
};

static_assert (Array <super_array1 <int, 10>>);

/*
	make a new array type that satisfies concept Array implicitly 
	by defining the right functions
*/

template <typename T, auto N>
struct super_array2 {
	T data [N];

	constexpr auto* begin () noexcept {return data;}
	constexpr auto* end () noexcept {return data + N;}
};

static_assert (Range <super_array2 <int, 10>>);
// static_assert (Array <super_array2 <int, 10>>);

constexpr bool bis_constant_evaluated() noexcept
{
    if consteval {
        return true;
    }
    else {
        return false;
    }
}




// static_assert (IS_CONSTEXPR (foo (10.0, true)));

auto main (int, char**) -> int 
{
	// B0 auto b0 = super_array2 <int, 10> {};
	return 0;
} 