export module Delta.Array;

export import Delta.Range;


export template <typename T>
concept ArrayPolicies = RangePolicies <T>;// and Bounded <range_type <T>>;

export template <typename...>
struct array_policies_t;

export template <typename T>
concept Array = ArrayPolicies <array_policies_t<T>>;

export template <Array T>
struct bounded_t<T> {
	constexpr static auto value = true;
};

template <typename T>
using m_array_types = int;

export template <typename, auto...>
concept Kuk = true;

// template <Kuk...>
// using kuki = int;

export template <
	auto N = 1, // length
	template <typename...> typename T = typelist, // type container
	typename... U> // types
requires (
	sizeof...(U) > 0 and
	(Array <U[N]> and ...) and
	(Array <U(&)[N]> and ...) and
	(Array <U const(&)[N]> and ...) and 
	requires {typename T <U[N]..., U(&)[N]..., U const(&)[N]...>;}
)
using make_array_types = T <U[N]..., U(&)[N]..., U const(&)[N]...>;

export template <
	typename T,
	auto N = 1,
	typename tp = typelist <>>
requires (Array <T[N]> and Array <T(&)[N]> and Array <T const(&)[N]>)
using push_array_types = push_back <tp, T[N], T(&)[N], T const(&)[N]>;



template <typename...>
using bbb = int;

// using t0 = push_array_types <int>;
// using t1 = push_array_types <int, typelist <>, 3, bbb>;
// using t1 = push_array_types <int>;

// export template <typename U, auto N, Typelist T = typelist <>>
// using push_array_type = add_types <T, U[N], U(&)[N], U const(&)[N]>; 

// export template <Typelist T = typelist<>, typename... U>
// using push_array_types = add_types <T, U [1]..., U (&)[1]..., U const (&) [1]...>;  //typename T::template push_back <U [1]..., U (&) [1]..., U const (&) [1]...>;


// export template <Typelist T>
// using make_array_types = 

constinit static auto count = 0;

template <typename>
struct push_array_t;






// template <Typelist T, typename... U>
// concept ContainsType = contains_type_t <> ;
// template <Typelist T>
// requires (not Contains <T, >)
// struct push_array_t <T>
// {

// };

export template <typename T, auto N>
struct array_policies_t <T[N]> {
	constexpr static auto begin(T(&range)[N]) noexcept -> Iterator auto {
		return range;
	}
	constexpr static auto end(T(&range)[N]) noexcept -> Iterator auto {
		return range + N;
	}
};

export template <typename T, auto N>
struct array_policies_t<T const(&)[N]> {
	constexpr static auto begin(T const(&range)[N]) noexcept -> Iterator auto {
		return range;
	}
	constexpr static auto end(T const(&range)[N]) noexcept -> Iterator auto {
		return range + N;
	}
};

export template <typename T, auto N>
struct array_policies_t<T(&)[N]> {
	constexpr static auto begin(T (&range)[N]) noexcept -> Iterator auto {
		return range;
	}
	constexpr static auto end(T (&range)[N]) noexcept -> Iterator auto {
		return range + N;
	}
};

export template <Array T>
struct range_policies_t <T> : array_policies_t <T> {};

// static_assert (AllOf <[]<typename T>{return Array <T>;}, array_types <int, 2>>);
// static_assert (AllOf <[]<typename T>{return Range <T>;}, array_types <int, 2>>);
// static_assert (AllOf <[]<typename T>{return Range <T>;}, array_types <char, 2>>);
// static_assert (AllOf <[]<typename T>{return Array <T>;}, array_types <char, 2>>);
// static_assert (Range <char[10]>);