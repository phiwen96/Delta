export module Delta.Concepts.Range.Begin;

import Delta.Concepts.Range.Policies;
import Delta.Concepts.Iterator;

template <typename T>
concept HasBegin = requires (T& t)
{
	{t.begin()} noexcept -> Iterator;
};

export constexpr auto begin (HasBegin auto&& range) noexcept -> Iterator auto 
{
	return range.begin();
}

export template <typename R, template <typename...> typename P = range_policies>
requires (RangePolicies <P <R>>)
constexpr auto begin (R && range) noexcept -> Iterator auto 
{
	return P <R> :: begin (range);
}

export template <Sentinel T>
constexpr auto begin (T&& range) noexcept -> Iterator auto 
{	
	return range;
}