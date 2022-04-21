export module Delta:Function;

import :Types;


constexpr auto is_function (auto&&...) noexcept -> bool
{
	return false;
}

template <typename T, typename... U>
constexpr auto is_function (T (*) (U...)) noexcept -> bool
{
	return true;
}



export template <typename T>
concept FunctionTraits = requires ()
{
	typename T::return_type;

	// {T::params} -> Signed;
};

export template <typename T>
struct function_traits_t;


export template <typename T>
struct function_traits_t <T()>
{
	using return_type = T;
	constexpr static auto nr_of_params = 0;
};

export template <typename T>
struct function_traits_t <T() noexcept>
{
	using return_type = T;
	constexpr static auto nr_of_params = 0;
};

export template <typename T, typename... U>
struct function_traits_t <T(U...)>
{
	using return_type = T;
	constexpr static auto nr_of_params = sizeof...(U);
	using params = typelist <U...>;
};

export template <typename T, typename... U>
struct function_traits_t <T(U...) noexcept>
{
	using return_type = T;
	constexpr static auto nr_of_params = sizeof...(U);
	using params = typelist <U...>;
};


export template <typename T>
concept Function = FunctionTraits <function_traits_t <T>>;

export template <Function T, auto I>
using fun_param_type = typename function_traits_t <T>::params::get <I>;

export template <Function T>
using fun_ret_type = typename function_traits_t <T>::return_type;




/*
	first we need a way to test, given a list of functions that
	1. it can take take all possible 

	it would be nice to have an interface making it possible 
	to find out if like the third parameter type for a function
	is a number or not.


*/

