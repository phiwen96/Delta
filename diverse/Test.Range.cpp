#include <iostream>
import Delta;

template <typename T, auto N>
struct super_range_2 {
	T data [N];

	auto* begin () noexcept {return data;}
	auto* end () noexcept {return data + N;}
};

static_assert (Range <super_range_2 <int, 10>>);


auto main (int, char**) -> int 
{

	return 0;
} 