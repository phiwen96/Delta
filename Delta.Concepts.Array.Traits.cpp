export module Delta.Concepts.Array:Traits;

import Delta.Concepts.Range.Traits;

template <typename T>
concept ArrayTraits = requires ()
{
	requires RangeTraits <typename T::range_traits>;
};

template <typename T>
struct array_traits_t;
// template <typename T>
// struct array_traits;

template <typename T, auto N>
struct array_traits_t <T [N]>
{
	using element_type = T;
	using iterator_type = T*; //return_type_of <>;
	// static constexpr auto is_array = true;
	// static constexpr auto is_bounded = true;
};

template <typename T, auto N>
struct array_traits_t <T (&) [N]>
{
	using element_type = T;
	using iterator_type = T*; //return_type_of <>;
	// static constexpr auto is_array = true;
	// static constexpr auto is_bounded = true;
};

// template <typename T, auto N>
// struct array_traits_t <T const [N]>
// {
// 	using element_type = T;
// 	using iterator_type = T const*;
// 	// using
// 	// static constexpr auto is_array = true;
// 	// static constexpr auto is_bounded = true;
// };

// template <typename T, auto N>
// struct array_traits_t <T const (&) [N]>
// {
// 	using element_type = T;
// 	using iterator_type = T const*;

// 	// static constexpr auto is_array = true;
// 	// static constexpr auto is_bounded = true;
// };

// export template <Array T>
// constexpr auto begin ()

// export template <typename T>
// requires ArrayTraits <array_traits_t <T>>
// struct range_traits_t <T>
// {

// };

// export template <typename T>
// requires ArrayTraits <array_traits <T>>
// struct range_traits <T>
// {
// 	using element_type = typename array_traits <T>::element_type;
// 	using iterator_type = typename array_traits <T>::iterator_type;
// };