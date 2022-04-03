export module Delta.Concepts.Range.Begin;

import Delta.Concepts.Range.Traits;
import Delta.Concepts.Iterator;
import Delta.Concepts.Char;

export template <typename T>
constexpr auto begin (T& t) noexcept -> Iterator auto 
requires requires 
{
	{range_traits <T>::begin (t)} noexcept -> Iterator;
}
{
	return range_traits <T>::begin (t);
}

export template <typename T>
constexpr auto begin (T const& t) noexcept -> Iterator auto 
requires requires 
{
	{range_traits <T>::begin (t)} noexcept -> Iterator;
}
{
	return range_traits <T>::begin (t);
}



