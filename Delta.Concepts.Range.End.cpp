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

export template <Iterator T>
requires requires (T t)
{
	*t == sentinel_value <T>;
}
constexpr auto end (T t) noexcept -> Iterator auto 
{
	auto i = t;

	while (*i != sentinel_value <T>)
	{
		++i;
	}
	return i;
}