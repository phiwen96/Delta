export module Delta.Concepts.Range.Traits;

import Delta.Concepts.Bool;
import Delta.Concepts.Size;

import Delta.Concepts.Iterator;

export template <typename T>
concept RangeTraits = requires ()
{

	// typename T::element_type;
	requires IteratorTraits <typename T::iterator_traits>;
	// requires Iterator <typename T::iterator_type>;
	
	// {T::is_array} -> Bool;
	// {T::is_bounded} -> Bool;

	// requires not T::is_bounded or requires 
	// {
	// 	{T::length} -> Size;
	// };
};

export template <typename T>
struct range_traits;

export template <typename T>
requires RangeTraits <range_traits <T>>
using element_type = typename range_traits <T>::element_type;

// export template <Iterator T>
// requires (Sentinel <T>)
// struct range_traits <>

template <typename T>
requires RangeTraits <range_traits <T>>
constexpr auto begin (T& range) noexcept -> Iterator auto 
{
	return range_traits <T>::begin (range);
}

template <typename T>
requires RangeTraits <range_traits <T>>
constexpr auto end (T& range) noexcept -> Iterator auto 
{
	return range_traits <T>::end (range);
}
