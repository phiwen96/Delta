import Delta;

#include <iostream>
#include <string>

// using std;

template <typename T, typename... U>
using first = T;

template <typename T, auto predicate, typename U, typename... V>
struct any_of_t
{
	constexpr static auto value = predicate.template operator() <T, U> () ? true : any_of_t <T, predicate, V...>::value;
};

template <typename T, auto predicate, typename U>
struct any_of_t <T, predicate, U>
{
	constexpr static auto value = predicate.template operator() <T, U> () ? true : false;
};


template <typename T, auto predicate, typename... U>
constexpr auto any_of = any_of_t <T, predicate, U...>::value;

template <typename T, auto predicate, typename... U>
concept AnyOf = any_of <T, predicate, U...>;

constexpr auto run (AnyOf <[]<typename T, typename U>{return Same <T, U>;}, int, char> auto yay)
{

}


auto main (int, char**) -> int 
{

	run (4.4);
		
	auto msg = "start program by hitting enter\n";
	// async::out << msg;

	// std::string input;

	// std::cin >> input;

	// async::out << input;



	return 0;
}