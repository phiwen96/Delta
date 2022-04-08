module Delta.Concepts.Range:End;

import Delta.Concepts.Iterator;
import Delta.Concepts.Range.Begin;
import :Traits;


// template <typename T>
// concept HasEnd = requires (T& t)
// {
// 	{t.end()} noexcept -> Iterator;
// };

// export constexpr auto end (HasEnd auto&& range) noexcept -> Iterator auto 
// {
// 	return range.end();
// }

// export template <Sentinel T>
// constexpr auto end (T&& t) noexcept -> Iterator auto 
// {
// 	auto i = t;

// 	while (*i != sentinel_value <T>)
// 	{
// 		++i;
// 	}
// 	return i;
// }

template <Range T>
constexpr auto end (T& range) noexcept -> Iterator auto 
{
	return range_traits <T>::end (range);
}