export module Delta.Concepts.Range.Begin;

import Delta.Concepts.Range.Policies;
import Delta.Concepts.Iterator;
// import Delta.Concepts.Range.Traits;

template <typename T>
concept HasBegin = requires (T& t)
{
	{t.begin()} noexcept -> Iterator;
};

export constexpr auto begin (HasBegin auto&& range) noexcept -> Iterator auto 
{
	return range.begin();
}

export template <typename T>
requires RangePolicies <range_policies <T>>
constexpr auto begin (T range) noexcept -> Iterator auto 
{
	return range_policies <decltype (range)> :: begin (range);
}

export template <Sentinel T>
constexpr auto begin (T&& range) noexcept -> Iterator auto 
{	
	return range;
}