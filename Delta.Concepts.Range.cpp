export module Delta.Concepts.Range;

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

export template <typename T>
struct range_traits
{

};

export template <typename T, auto N>
struct range_traits <T const [N]>
{
	using element_type = T;
};

export template <typename T, auto N>
struct range_traits <T const (&) [N]>
{
	using element_type = T;
};





export template <Range T>
using element_type = typename range_traits <T>::element_type;//decltype (*(begin (mimic <T> ())));

export template <typename T, typename U>
concept Element = Range <T> and Convertible <element_type <T>, U>;

static_assert (Same <element_type <char const [10]>, char>);