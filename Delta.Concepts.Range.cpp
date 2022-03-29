export module Delta.Concepts.Range;

import Delta.Concepts.Convertible;
import Delta.Mimic;
import Delta.Concepts.Iterator;
export import Delta.Concepts.Range.Begin;
export import Delta.Concepts.Range.End;
export import Delta.Concepts.Range.Contiguous;



export template <typename T>
concept Range = 
requires (T& t)
{
	{begin (t)} noexcept -> Iterator;
	{end (t)} noexcept -> Iterator;
};

export template <Range T>
using element_type = decltype (*(begin (mimic <T> ())));

export template <typename T, typename U>
concept Element = Range <T> and Convertible <element_type <T>, U>;