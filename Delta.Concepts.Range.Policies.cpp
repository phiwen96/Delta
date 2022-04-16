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
concept RangePolicies = requires (fun_param_type <decltype (T::begin), 0>& range)
{
	{T::begin (range)} noexcept -> Iterator;

	requires requires 
	{
		{T::end (range)} noexcept -> Iterator;

	} or requires 
	{
		{T::length (range)} noexcept -> Size;
	};
	
	
	// true;
	// {T::begin (t)} noexcept -> Iterator;
	// {T::end (t)} noexcept -> Iterator;
};

export template <typename... T>
struct range_policies_t;

export template <typename T>
concept HasDefinedRangePolicies = RangePolicies <range_policies_t <T>>;

// export template <typename T>
// requires RangePolicies <typename T::range_policies>
// struct get_range_policies_t <T>
// {

// };
 

// export template <SentinelValue T>
// struct range_policies_t <T>
// {
// 	constexpr static auto begin (T t) noexcept -> Iterator auto 
// 	{
// 		return t;
// 	}

// 	constexpr static auto end (T t) noexcept -> Iterator auto 
// 	{
// 		auto i = t;

// 		while (i != sentinel_value <T>)
// 		{
// 			++i;
// 		}
		
// 		return i;
// 	}
// };

// export template <SentinelValue T>
// constexpr auto begin (T range) noexcept -> Iterator auto 
// {
// 	return range;
// }

// export template <SentinelValue T>
// constexpr auto end (T range) noexcept -> Iterator auto 
// {
// 	auto i = range;

// 		while (i != sentinel_value <T>)
// 		{
// 			++i;
// 		}
		
// 		return i;
// }


export template <HasDefinedRangePolicies T>
constexpr auto begin (T range) noexcept -> Iterator auto 
{
	return range_policies_t <T>::begin (range);
}

export template <HasDefinedRangePolicies T>
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