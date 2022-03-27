export module Delta.Concepts.Range.Contiguous;

import Delta.Concepts.Iterator;
export import Delta.Concepts.Range.Begin;
export import Delta.Concepts.Range.End;

export template <typename T>
concept ContiguousRange = requires (T t)
{
	{begin (t)} -> Iterator;
	{end (t)} -> Iterator;
};

