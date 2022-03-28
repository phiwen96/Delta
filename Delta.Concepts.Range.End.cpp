export module Delta.Concepts.Range.End;

import Delta.Concepts.Iterator;
import Delta.Concepts.Array;

export
{
	template <Array T>
	constexpr auto end (T& t) noexcept -> Iterator auto 
	{
		return t;
	}
	
	constexpr auto end (auto &&range) noexcept -> Iterator auto 
	requires requires
	{
		{range.end()} noexcept -> Iterator;
	}
	{
		return range.end();
	}

	// template <typename T, auto N>
	// constexpr auto end (T (t) [N]) noexcept -> Iterator auto 
	// {
	// 	return t + N;
	// }

	
}