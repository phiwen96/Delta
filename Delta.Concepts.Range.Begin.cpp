export module Delta.Concepts.Range.Begin;

import Delta.Concepts.Iterator;
import Delta.Concepts.Size;
import Delta.Concepts.Array;

export
{
	template <typename T, auto N>
	constexpr auto begin (T (&t) [N]) noexcept -> Iterator auto 
	{
		return t + N;
	}

	template <auto N>
	constexpr auto begin (auto const (&t) [N]) noexcept -> Iterator auto 
	{
		return t + N;
	}

	constexpr auto begin (auto& range) noexcept -> Iterator auto 
	requires requires
	{
		{range.begin()} noexcept -> Iterator;
	}
	{
		return range.begin();
	}

	constexpr auto begin (auto const& range) noexcept -> Iterator auto 
	requires requires
	{
		{range.begin()} noexcept -> Iterator;
	}
	{
		return range.begin();
	}
}