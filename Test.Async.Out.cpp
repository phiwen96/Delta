import Delta;

#include <iostream>
#include <string>

// using std;

template <typename T, typename... U>
using first = T;



constexpr auto run (AnyOf <[]<typename T, typename U>{return Same <T, U>;}, int, char> auto yay)
{

}


auto main (int, char**) -> int 
{

	run (4);
		
	auto msg = "start program by hitting enter\n";
	// async::out << msg;

	// std::string input;

	// std::cin >> input;

	// async::out << input;



	return 0;
}