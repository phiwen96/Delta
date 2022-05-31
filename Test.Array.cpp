#include <iostream>
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

	auto* begin () noexcept {return data;}
	auto* end () noexcept {return data + N;}
};

static_assert (Array <super_array2 <int, 10>>);


auto main (int, char**) -> int 
{

	return 0;
} 