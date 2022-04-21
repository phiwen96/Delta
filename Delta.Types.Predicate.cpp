export module Delta:Types.Predicate;

import :Same;

/*
	A type predicate is a lambda that, when called 
	with a type, should return a boolean value.
*/
export template <typename T, typename U>
concept TypePredicate = requires (T& t)
{
	{t.template operator() <U> ()} -> Same <bool>;
};
