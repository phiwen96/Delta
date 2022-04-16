export module Delta.Concepts.Array.Policies;

import Delta.Concepts.Range.Policies;
import Delta.Concepts.Iterator;
import Delta.Concepts.Size;

export template <typename T>
concept ArrayPolicies = RangePolicies <T>;

export template <typename... T>
struct array_policies_t;

export template <typename T>
concept HasDefinedArrayPolicies = ArrayPolicies <array_policies_t <T>>;



export template <typename T, auto N>
struct array_policies_t <T [N]> 
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
struct array_policies_t <T (&) [N]> 
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
export template <HasDefinedArrayPolicies T>
struct get_range_policies_t <T> //: array_policies_t <T>
{
	using type = array_policies_t <T>;
};

