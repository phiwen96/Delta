export module Delta.Concepts.Range.End;

import Delta.Concepts.Iterator;

export
{
	constexpr auto end (auto &&range) noexcept -> Iterator auto 
	requires requires
	{
		{range.end()} noexcept -> Iterator;
	}
	{
		return range.end();
	}
}