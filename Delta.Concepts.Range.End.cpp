export module Delta.Concepts.Range.End;

import Delta.Concepts.Iterator;
import Delta.Concepts.Array;

export
{	
	template <auto N>
	constexpr auto end (auto (&t) [N]) noexcept -> Iterator auto 
	{
		return t + N;
	}

	template <auto N>
	constexpr auto end (auto const (&t) [N]) noexcept -> Iterator auto 
	{
		return t + N;
	}

	constexpr auto end (auto& t) noexcept -> Iterator auto 
	requires requires 
	{
		{end (t)} noexcept -> Iterator;
	}
	{
		return end (t);
	}

	constexpr auto end (auto const& t) noexcept -> Iterator auto 
	requires requires 
	{
		{end (t)} noexcept -> Iterator;
	}
	{
		return end (t);
	}
	
}