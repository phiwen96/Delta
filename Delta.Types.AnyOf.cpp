export module Delta.Types.AnyOf;

// import Delta.Types.Predicate;
import Delta.Types.Predicate;

template <auto predicate, typename T, typename... U>
struct any_of_t
{
	constexpr static auto value = predicate.template operator() <T> () ? true : any_of_t <predicate, U...>::value;
};

template <auto predicate, typename T>
struct any_of_t <predicate, T>
{
	constexpr static auto value = predicate.template operator() <T> () ? true : false;
};

// typelist
template <auto predicate, template <typename...> typename T, typename... U>
struct any_of_t <predicate, T <U...>> : any_of_t <predicate, U...> {};


template <auto predicate, typename... T>
constexpr auto any_of = any_of_t <predicate, T...>::value;

export template <auto predicate, typename... T>
concept AnyOf = (TypePredicate <decltype (predicate), T> and ... ) and any_of <predicate, T...>;
// export template <typename T>
// concept TypelistTraits = requires ()
// {
// 	true;
// };