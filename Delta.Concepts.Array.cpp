export module Delta.Concepts.Array;

import Delta.Concepts.Size;
import Delta.Types;
import Delta.Concepts.Range;


// export import :Traits;

// export template <typename T>
// concept Array = ArrayTraits <array_traits_t <T>>;
// ArrayTraits <array_traits_t <range_traits_t <T>>>;

export template <typename T, auto N>
struct range_policies_t <T [N]> 
{
	constexpr static auto begin (T (range) [N]) noexcept -> Iterator auto
	{
		return range;
	}

	constexpr static auto length (T (range) [N]) noexcept -> Size auto
	{
		return N;
	}
};

export template <typename T, auto N>
struct range_policies_t <T (&) [N]> 
{
	constexpr static auto begin (T (&range) [N]) noexcept -> Iterator auto
	{
		return range;
	}

	constexpr static auto length (T (&range) [N]) noexcept -> Size auto
	{
		return N;
	}
};

export template <typename T, auto N>
using array_types = typelist <T [N], T (&) [N], T const (&) [N]>;

static_assert (Range <int const(&)[10]>);
// static_assert (AllOf <[] <typename T> {return Range <T>;}, array_types <int, 10>>);
