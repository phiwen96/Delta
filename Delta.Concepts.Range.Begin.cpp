export module Delta.Concepts.Range.Begin;

import Delta.Concepts.Iterator;

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
}