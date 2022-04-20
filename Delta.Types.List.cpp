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



template <typename... T>
struct node;

template <typename T>
struct node <T>
{
	using type = T;
	// constexpr static auto length = 
};





export template <typename T, typename... U>
struct typelist <T, U...> //: indexed_element <0, T>
{
	template <template <typename...> typename V>
	using transform = V <T, U...>;

	constexpr static auto reversed_index = sizeof... (U);

	template <auto i> 
	using get = numbered_typelist <0, T, U...>::template get <i>;

	// using params = numbered_typelist <0, T, U...>;
};

// static_assert ()










// import Delta.Types.List.Traits;

// export template <typename T>
// concept Typelist = TypelistTraits <typelist_traits <T>>;