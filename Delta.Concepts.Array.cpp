export module Delta.Concepts.Array;

import Delta.Concepts.Size;
export import Delta.Concepts.Range;

export template <typename T>
concept ArrayTraits = requires 
{
	typename T::element_type;
	{T::length} -> Size;
};

export template <typename T>
struct array_traits;

export template <typename T, auto N>
struct array_traits <T [N]>
{
	using element_type = T;
	static constexpr auto length = N;
};

export template <typename T, auto N>
struct array_traits <T const [N]>
{
	using element_type = T;
	static constexpr auto length = N;
};

export template <typename T, auto N>
struct array_traits <T const (&) [N]>
{
	using element_type = T;
	static constexpr auto length = N;
};

export template <typename T>
concept Array = ArrayTraits <array_traits <T>>;

export template <Array T>
struct range_traits <T> : array_traits <T>
{

};


export constexpr auto length (Array auto const& range) noexcept -> auto 
{
	return array_traits <decltype (range)>::length;
}

static_assert (Array <char [10]>);
static_assert (Array <char const [10]>);
static_assert (Array <char const (&) [10]>);

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