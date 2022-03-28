export module Delta.Concepts.Range;

import Delta.Concepts.Iterator;
export import Delta.Concepts.Range.Begin;
export import Delta.Concepts.Range.End;
export import Delta.Concepts.Range.Contiguous;

export template <typename T>
concept Range = requires (T t)
{
	{begin (t)} -> Iterator;
	{end (t)} -> Iterator;
};

