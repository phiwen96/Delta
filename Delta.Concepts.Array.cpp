export module Delta.Concepts.Array;

import Delta.Concepts.Size;
export import Delta.Concepts.Range;

export template <typename T, auto N>
struct range_traits <T [N]>
{
	using element_type = T;
	using iterator_type = T *;
	static constexpr auto is_array = true;
	static constexpr auto is_bounded = true;
	static constexpr auto length = N;
};

export template <typename T, auto N>
struct range_traits <T const [N]>
{
	using element_type = T;
	using iterator_type = T const*;
	static constexpr auto is_array = true;
	static constexpr auto is_bounded = true;
	static constexpr auto length = N;
};

export template <typename T, auto N>
struct range_traits <T const (&) [N]>
{
	using element_type = T;
	using iterator_type = T const*;
	static constexpr auto is_array = true;
	static constexpr auto is_bounded = true;
	static constexpr auto length = N;
};

export template <typename T>
concept Array = Range <T> and range_traits<T>::is_array;

export constexpr auto length (Array auto const& range) noexcept -> auto 
{
	return range_traits <decltype (range)>::length;
}

static_assert (Array <int [10]>);
static_assert (Array <int const [10]>);
static_assert (Array <int const (&) [10]>);

export constexpr auto begin (Array auto&& range) noexcept -> Iterator auto 
{
	return range;
}

export constexpr auto begin (Array auto range) noexcept -> Iterator auto 
{
	return range;
}

export constexpr auto end (Array auto&& range) noexcept -> Iterator auto 
{
	return begin (range) + length (range);
}