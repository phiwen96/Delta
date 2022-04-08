export module Delta.Types:AnyOf;

import Delta.Types.Predicate;

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

export template <typename T, auto predicate, typename U, typename... V>
requires TypePredicate <>
concept AnyOf = any_of <T, predicate, U, V...>;
// export template <typename T>
// concept TypelistTraits = requires ()
// {
// 	true;
// };