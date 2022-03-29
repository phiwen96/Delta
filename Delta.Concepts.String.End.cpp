export module Delta.Concepts.String.End;

import Delta.Concepts.Char;
import Delta.Concepts.Iterator;

export template <InputIterator T>
constexpr auto end (T&& t) noexcept -> Iterator auto 
{
	while (*t != '\0')
	{
		++t;
	}

	return t;
}
