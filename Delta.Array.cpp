export module Delta.Array;

export import Delta.Range;


export template <typename T>
concept ArrayPolicies = RangePolicies<T>;

export template <typename...>
struct array_policies_t;

export template <typename T>
concept HasDefinedArrayPolicies = ArrayPolicies<array_policies_t<T>>;

export template <HasDefinedArrayPolicies T>
struct range_policies_t<T> : array_policies_t<T>
{
};

export template <typename T>
concept Array = HasDefinedArrayPolicies<T>;

export template <Array T>
struct bounded_t<T>
{
	constexpr static auto value = true;
};

export template <typename U, auto N>
using array_types = typelist<U[N], U (&)[N], U const (&)[N]>;

export template <typename T, auto N>
struct array_policies_t<T[N]>
{
	constexpr static auto begin(T(range)[N]) noexcept -> Iterator auto
	{
		return range;
	}

	constexpr static auto end(T(range)[N]) noexcept -> Iterator auto
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

// static_assert (AllOf <[]<typename T>{return Array <T>;}, array_types <int, 2>>);
// static_assert (AllOf <[]<typename T>{return Range <T>;}, array_types <int, 2>>);
// static_assert (AllOf <[]<typename T>{return Range <T>;}, array_types <char, 2>>);
// static_assert (AllOf <[]<typename T>{return Array <T>;}, array_types <char, 2>>);
// static_assert (Range <char[10]>);