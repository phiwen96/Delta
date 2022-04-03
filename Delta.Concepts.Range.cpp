module;
#include <utility>
export module Delta.Concepts.Range;

export import Delta.Concepts.Range.Traits;
import Delta.Concepts.Same;
import Delta.Concepts.Convertible;
import Delta.Mimic;
export import Delta.Concepts.Iterator;
export import Delta.Concepts.Range.Begin;
export import Delta.Concepts.Range.End;
// export import Delta.Concepts.Range.Contiguous;

export template <typename T>
concept Range = 
requires (T& t)
{
	{begin (t)} noexcept -> Iterator;
	{end (t)} noexcept -> Iterator;
};

static_assert (Range <char [10]>);
static_assert (Range <char const [10]>);
static_assert (Range <char const (&) [10]>);


// export template <typename T, auto N>
// struct range_traits <T const (&) [N]>
// {	
// 	using element_type = T;

// 	static constexpr auto begin (T const (&t) [N]) noexcept -> Iterator auto 
// 	{
// 		return t;
// 	}

// 	static constexpr auto end (T const (&t) [N]) noexcept -> Iterator auto 
// 	{
// 		return t;
// 	}
// };






// export template <Range T>
// using element_type = typename range_traits <T>::element_type;//decltype (*(begin (mimic <T> ())));

// export template <typename T, typename U>
// concept Element = Range <T> and Convertible <element_type <T>, U>;

// static_assert (Same <element_type <char const [10]>, char>);