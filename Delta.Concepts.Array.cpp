export module Delta.Concepts.Array;

import Delta.Concepts.Size;
import Delta.Types;
export import Delta.Concepts.Range;

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


// static_assert (Range <int [10]>);
static_assert (AllOf <[] <typename T> {return Range <T>;}, array_types <int, 10>>);
// static_assert (ContiguousRange <T>);