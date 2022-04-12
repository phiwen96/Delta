export module Delta.Concepts.Array:Traits;

import Delta.Concepts.Range.Traits;
import Delta.Concepts.Iterator.Traits;

template <typename T>
concept ArrayTraits = requires ()
{
	requires RangeTraits <typename T::range_traits>;
};

export template <typename... T>
struct array_traits_t;

export template <RangeTraits rangeTraits>
struct array_traits_t <rangeTraits>
{
	using range_traits = rangeTraits;
};

export template <typename T, auto N>
struct array_traits_t <T [N]>
{
	using range_traits = range_traits_t <T [N], iterator_traits_t <T*, T>>;
	// using range_type = T [N];
	// using iterator_traits = iterator_traits_t <T*, T>;
};


// export template <typename T, auto N>
// struct range_traits_t <T [N]> : array_traits_t <range_traits_t <T [N]>>
// {
// 	// using range_traits = range_traits_t <T [N]>;
// 	using range_type = T [N];
// 	using iterator_traits = iterator_traits_t <T*, T>;
// };