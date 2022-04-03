export module Delta.Concepts.Range.End;

import Delta.Concepts.Range.Traits;
import Delta.Concepts.Iterator;

export template <typename T>
constexpr auto end (T& t) noexcept -> Iterator auto 
requires requires 
{
	{range_traits <T>::end (t)} noexcept -> Iterator;
}
{
	return range_traits <T>::end (t);
}

export template <typename T>
constexpr auto end (T const& t) noexcept -> Iterator auto 
requires requires 
{
	{range_traits <T>::end (t)} noexcept -> Iterator;
}
{
	return range_traits <T>::end (t);
}