export module Delta.Concepts.Function;

import Delta.Concepts.Number.Signed;

export template <typename T>
concept FunctionTraits = requires ()
{
	typename T::return_value;

	{T::params} -> Signed;
};

export template <typename T>
struct function_traits_t;

export template <typename T>
concept Function = FunctionTraits <function_traits_t <T>>;

export template <typename T>
struct function_traits_t <T()>
{
	using return_value = T;
	constexpr static auto params = 0;
};

export template <typename T, typename... U>
struct function_traits_t <T(U...)>
{
	using return_value = T;
	constexpr static auto params = sizeof...(U);
};



/*
	first we need a way to test, given a list of functions that
	1. it can take take all possible 

	it would be nice to have an interface making it possible 
	to find out if like the third parameter type for a function
	is a number or not.


*/

