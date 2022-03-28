export module Delta.Concepts.Range.Begin;

import Delta.Concepts.Iterator;
import Delta.Concepts.Size;
import Delta.Concepts.Array;

export
{
	constexpr auto begin (auto &&range) noexcept -> Iterator auto 
	requires requires
	{
		{range.begin()} noexcept -> Iterator;
	}
	{
		return range.begin();
	}

	constexpr auto begin (auto* t) noexcept -> Iterator auto 
	{
		return t;
	}
}