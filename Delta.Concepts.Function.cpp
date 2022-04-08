export module Delta.Concepts.Function;

import :Traits;



export template <typename T>
concept Function = FunctionTraits <function_traits_t <T>>;




/*
	first we need a way to test, given a list of functions that
	1. it can take take all possible 

	it would be nice to have an interface making it possible 
	to find out if like the third parameter type for a function
	is a number or not.


*/

