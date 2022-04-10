export module Delta.Concepts.Range.Policies;

import Delta.Concepts.Iterator;

export template <typename T>
concept RangePolicies = requires (T& t)
{
	{T::begin (t)} noexcept -> Iterator;
	{T::end (t)} noexcept -> Iterator;
};
 
export template <typename... T>
struct range_policies_t;