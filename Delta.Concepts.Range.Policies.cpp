export module Delta.Concepts.Range.Policies;

import Delta.Concepts.Iterator;



template <typename T>
struct function_param_t 
{

};

export template <typename T>
concept RangePolicies = requires (T& t)
{
	{T::begin (t)} noexcept -> Iterator;
	{T::end (t)} noexcept -> Iterator;
};
 
export template <typename T>
struct range_policies;