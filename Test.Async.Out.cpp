#include <iostream>
#include <string>

import Delta;

template <typename T, auto N>
struct bajs 
{
	constexpr static auto begin (T (t) [N]) noexcept -> T*
	{
		return t;
	}
};

static_assert (Array <int[10]>);
static_assert (Range <int[10]>);

auto main (int, char**) -> int 
{


	// Array auto a = "hej";
	// run (4);
		
	auto msg = "start program by hitting enter\n";
	// async::out << msg;

	// std::string input;

	// std::cin >> input;

	// async::out << input;



	return 0;
}