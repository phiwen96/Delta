export module Delta.Concepts.Function;

import Delta.Concepts.Function.Traits;



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

