export module Delta.Concepts.Array;

import Delta.Concepts.Size;
// import Delta.Concepts.Range;
import :Traits;

// template <typename T, auto N>
// struct range_policies <T [N]>
// {
// 	static constexpr auto begin (T (t) [N]) noexcept -> Iterator auto 
// 	{
// 		return t;
// 	}

// 	static constexpr auto end (T (t) [N]) noexcept -> Iterator auto 
// 	{
// 		return t + N;
// 	}

// 	static constexpr auto length (T (t) [N]) noexcept -> Size auto 
// 	{
// 		return N;
// 	}
// };

// export template <typename T, auto N>
// struct range_traits <T [N]>
// {
// 	using element_type = T;
// 	using iterator_type = T*; //return_type_of <>;
// 	static constexpr auto is_array = true;
// 	static constexpr auto is_bounded = true;
// };

// export template <typename T, auto N>
// struct range_traits <T const [N]>
// {
// 	using element_type = T;
// 	using iterator_type = T const*;
// 	static constexpr auto is_array = true;
// 	static constexpr auto is_bounded = true;
// };

// export template <typename T, auto N>
// struct range_traits <T const (&) [N]>
// {
// 	using element_type = T;
// 	using iterator_type = T const*;
// 	static constexpr auto is_array = true;
// 	static constexpr auto is_bounded = true;
// };

export template <typename T>
concept Array = true; //Range <T> and range_traits<T>::is_array;

// export constexpr auto length (Array auto const& range) noexcept -> auto 
// {
// 	return range_traits <decltype (range)>::length;
// }

// static_assert (Array <int [10]>);
// static_assert (Array <int const [10]>);
// static_assert (Array <int const (&) [10]>);
