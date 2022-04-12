export module Delta.Concepts.Range.Traits;

import Delta.Concepts.Bool;
import Delta.Concepts.Size;
import Delta.Concepts.Same;
import Delta.Concepts.Iterator;
import Delta.Concepts.Range.Policies;

export template <typename T>
concept RangeTraits = requires (typename T::range_type& range)
{
	requires IteratorTraits <typename T::iterator_traits>;
	

	// requires Same <typename T::iterator_traits::iterator_type, decltype (T::begin (range))>;

	// {T::begin (r)} noexcept -> Iterator;
	// {T::end (r)} noexcept -> Iterator;
	
	// requires IteratorTraits <typename T::iterator_traits>;

	// requires Iterator <typename T::iterator_type>;
	
	// {T::is_array} -> Bool;
	// {T::is_bounded} -> Bool;

	// requires not T::is_bounded or requires 
	// {
	// 	{T::length} -> Size;
	// };
};

export template <typename T>
concept HasRangeTraits = RangeTraits <typename T::range_traits>;

export template <typename... T>
struct range_traits_t;

export template <typename rangeType, IteratorTraits iteratorTraits>
struct range_traits_t <rangeType, iteratorTraits>
{
	using range_type = rangeType;
	using iterator_traits = iteratorTraits;
};

// template <typename T>
// requires RangeTraits <range_traits_t <T>>
// constexpr auto begin (T range) noexcept -> Iterator auto 
// {
// 	return range_traits_t <T>::begin (range);
// }

// template <typename T>
// requires RangeTraits <range_traits_t <T>>
// constexpr auto end (T range) noexcept -> Iterator auto 
// {
// 	return range_traits_t <T>::end (range);
// }
