export module Delta.Concepts.Range.Policies;

import Delta.Concepts.Function;

import Delta.Concepts.Iterator;
import Delta.Concepts.Size;
/*
	begin + end 
		or 
	begin + length
*/
export template <typename T>
concept RangePolicies = requires (typename function_traits_t <decltype (T::begin)>::params::get <0> range)
{
	requires requires ()
	{
		{T::end (range)} noexcept -> Iterator;

	} or requires ()
	{
		{T::length (range)} noexcept -> Size;
	};
	
	
	// true;
	// {T::begin (t)} noexcept -> Iterator;
	// {T::end (t)} noexcept -> Iterator;
};

// export template <typename T>
// requires RangePolicies <typename T::range_policies>
// struct get_range_policies_t <T>
// {

// };
 
export template <typename... T>
struct range_policies_t;

template <typename T>
requires RangePolicies <range_policies_t <T>>
constexpr auto begin (T range) noexcept -> Iterator auto 
{
	return range_policies_t <T>::begin (range);
}

template <typename T>
requires RangePolicies <range_policies_t <T>>
constexpr auto end (T range) noexcept -> Iterator auto 
{
	if constexpr (not requires {typename T::end;})
	{
		return range_policies_t <T>::begin (range) + range_policies_t <T>::length (range);

	} else 
	{
		return range_policies_t <T>::end (range);
	}
}
