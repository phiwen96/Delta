export module Delta.Concepts.Range;

import Delta.Concepts.Range.Iterator;

export 
{
	template <typename T>
	concept Range = requires (T t)
	{
		{begin (t)} -> Iterator;
		{end (t)} -> Iterator;
	};
}