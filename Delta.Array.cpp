export module Delta.Array;

export import Delta.Range;


export template <typename T>
concept ArrayPolicies = RangePolicies<T>;// and Bounded <range_type <T>>;

export template <typename...>
struct array_policies_t;

export template <typename T>
concept Array = ArrayPolicies<array_policies_t<T>>;

export template <Array T>
struct bounded_t<T>
{
	constexpr static auto value = true;
};

// template <typename T>
// using array_types_t = typelist<T[1], T (&)[1], T const (&)[1]>;

// export template <typename... U>
// using array_types = typelist<array_types_t <U>...>;

export template <typename T>
using array_types = typelist<T[1], T (&)[1], T const (&)[1]>;

export template <typename T, auto N>
struct array_policies_t<T[N]>
{
	constexpr static auto begin(T(&range)[N]) noexcept -> Iterator auto
	{
		return range;
	}

	constexpr static auto end(T(&range)[N]) noexcept -> Iterator auto
	{
		return range + N;
	}
};

export template <typename T, auto N>
struct array_policies_t<T const(&)[N]>
{
	constexpr static auto begin(T const(&range)[N]) noexcept -> Iterator auto
	{
		return range;
	}

	constexpr static auto end(T const(&range)[N]) noexcept -> Iterator auto
	{
		return range + N;
	}
};

export template <typename T, auto N>
struct array_policies_t<T (&)[N]>
{
	constexpr static auto begin(T (&range)[N]) noexcept -> Iterator auto
	{
		return range;
	}

	constexpr static auto end(T (&range)[N]) noexcept -> Iterator auto
	{
		return range + N;
	}
};

export template <Array T>
struct range_policies_t <T> : array_policies_t <T>
{

};

// static_assert (AllOf <[]<typename T>{return Array <T>;}, array_types <int, 2>>);
// static_assert (AllOf <[]<typename T>{return Range <T>;}, array_types <int, 2>>);
// static_assert (AllOf <[]<typename T>{return Range <T>;}, array_types <char, 2>>);
// static_assert (AllOf <[]<typename T>{return Array <T>;}, array_types <char, 2>>);
// static_assert (Range <char[10]>);