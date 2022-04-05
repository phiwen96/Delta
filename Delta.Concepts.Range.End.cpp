export module Delta.Concepts.Range.End;

import Delta.Concepts.Array;
import Delta.Concepts.Iterator;
import Delta.Concepts.Range.Begin;

template <typename T>
concept HasEnd = requires (T& t)
{
	{t.end()} noexcept -> Iterator;
};

export constexpr auto end (HasEnd auto&& range) noexcept -> Iterator auto 
{
	return range.end();
}

export constexpr auto end (Array auto&& range) noexcept -> Iterator auto 
{
	return begin (range) + length (range);
}

export constexpr auto end (IteratorWithSentinel auto&& iter) noexcept -> Iterator auto 
{
	auto i = iter;
	auto e = sentinel (iter);
	while (i != e)
	{
		++i;
	}
	return i;
}