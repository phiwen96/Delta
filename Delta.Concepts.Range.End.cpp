export module Delta.Concepts.Range.End;

import Delta.Concepts.Iterator;
import Delta.Concepts.Array;

export
{	
	template <typename T, auto N>
	constexpr auto end (T (&t) [N]) noexcept -> Iterator auto 
	{
		return t;
	}

	template <typename T>
	constexpr auto end (T& t) noexcept -> Iterator auto 
	requires requires 
	{
		{end (t)} -> Iterator;
	}
	{
		return end (t);
	}
	
}