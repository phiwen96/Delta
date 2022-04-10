export module Delta.Concepts.Array;

import Delta.Concepts.Size;
import Delta.Types;
// import Delta.Concepts.Range;
export import :Traits;

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

export template <typename T>
concept Array = ArrayTraits <array_traits_t <T>>; //Range <T> and range_traits<T>::is_array;

export template <typename T, auto N>
using array_types = typelist <T [N], T (&) [N], T const (&) [N]>;
static_assert (Array <int[10]>);
// static_assert (AllOf <[] <typename T> {return Array <T>;}, array_types <int, 10>>);

// export constexpr auto length (Array auto const& range) noexcept -> auto 
// {
// 	return range_traits <decltype (range)>::length;
// }

// static_assert (Array <int [10]>);
// static_assert (Array <int const [10]>);
// static_assert (Array <int const (&) [10]>);
