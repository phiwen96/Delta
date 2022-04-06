export module Delta.Concepts.Range.Traits;

import Delta.Concepts.Bool;

export template <typename T>
concept RangeTraits = requires ()
{
	typename T::element_type;
	{T::bounded} -> Bool;
};

export template <typename T>
struct range_traits;

export template <typename T>
using element_type = typename range_traits <T>::element_type;