export module Delta.Types.List;

import Delta.Types.IfElse;

template <auto, typename...>
struct numbered_typelist;

// template <auto N, auto M, typename... U, typename... V>
// struct numbered_typelist <N, numbered_typelist <M, U...>, V...> : numbered_typelist <N, U..., V...>
// {

// };	

template <auto N, typename T, typename... U>
struct numbered_typelist <N, T, U...>
{
	using type = T;
	constexpr static auto index = N;

	template <auto i>
	requires (i >= N and i <= N + sizeof... (U))
	using get = if_else <i == index, type, typename numbered_typelist <N + 1, U...>::type>;
};

// template <auto N, auto M, typename... T>
// struct numbered_typelist <N, numbered_typelist <M, T...>> : numbered_typelist <N, T...>
// {

// };

template <auto N, typename T>
struct numbered_typelist <N, T>
{
	using type = T;
	constexpr static auto index = N;

	template <auto i>
	requires (i == index)
	using get = type;
};



export template <typename... T>
struct typelist;

export template <typename T, typename... U>
struct typelist <T, U...> : numbered_typelist <0, T, U...>
{
	template <template <typename...> typename V>
	using transform = V <T, U...>;

	// using params = numbered_typelist <0, T, U...>;
};

// static_assert ()










// import Delta.Types.List.Traits;

// export template <typename T>
// concept Typelist = TypelistTraits <typelist_traits <T>>;