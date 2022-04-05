export module Delta.Concepts.Range.Begin;

import Delta.Concepts.Array;
import Delta.Concepts.Iterator;

template <typename T>
concept HasBegin = requires (T& t)
{
	{t.begin()} noexcept -> Iterator;
};

export constexpr auto begin (HasBegin auto&& range) noexcept -> Iterator auto 
{
	return range.begin();
}

export constexpr auto begin (Array auto&& range) noexcept -> Iterator auto 
{
	return range;
}

export constexpr auto begin (Array auto range) noexcept -> Iterator auto 
{
	return range;
}

export constexpr auto begin (IteratorWithSentinel auto&& range) noexcept -> Iterator auto 
{
	return range;
}