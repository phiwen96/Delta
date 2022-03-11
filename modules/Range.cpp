export module Delta.Range;

import Delta.Range.Iterator;

export 
{
	template <typename T>
	concept Range = requires (T t)
	{
		{begin (t)} -> Iterator;
		{end (t)} -> Iterator;
	};
}