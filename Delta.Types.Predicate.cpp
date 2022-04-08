export module Delta.Types.Predicate;

import Delta.Concepts.Same;

/*
	A type predicate is a lambda that, when called 
	with types, should return a boolean value.
*/
export template <auto predicate, typename... T>
concept TypePredicate = requires ()
{
	{predicate.template operator() <T...> ()} -> Same <bool>;
};
