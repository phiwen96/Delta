export module Delta.Types.AllOf;

// import Types.Predicate;
import Delta.Types.Predicate;
import Delta.Types.List;

export template <auto predicate, typename T, typename... U>
struct all_of_t
{
	constexpr static auto value = predicate.template operator() <T> () ? all_of_t <predicate, U...>::value : false;
};

export template <auto predicate, typename T>
struct all_of_t <predicate, T>
{
	constexpr static auto value = predicate.template operator() <T> () ? true : false;
};

// expand nested typelist, but what if vector

export template <auto predicate, typename... U>
struct all_of_t <predicate, typelist <U...>> : all_of_t <predicate, U...> {};

export template <auto predicate, typename... U, typename... V>
struct all_of_t <predicate, typelist <U...>, V...> : all_of_t <predicate, U..., V...> {};


export template <auto predicate, typename... T>
constexpr auto all_of = all_of_t <predicate, T...>::value;

export template <auto predicate, typename... T>
concept AllOf = (TypePredicate <decltype (predicate), T> and ... ) and all_of <predicate, T...>;
// export template <typename T>
// concept TypelistTraits = requires ()
// {
// 	true;
// };

// static_assert (not AllOf <[]<typename T>{return }>)