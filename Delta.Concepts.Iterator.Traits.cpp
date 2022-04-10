export module Delta.Concepts.Iterator.Traits;

import Delta.Concepts.Pointer;

export import Delta.Concepts.Iterator.Traits.Sentinel;

export template <typename T>
concept IteratorTraits = requires()
{
	typename T::element_type;
	typename T::sentinel_traits;
};

export template <typename... T>
struct iterator_traits_t ;

export template <Pointer T>
struct iterator_traits_t <T>
{
	using iterator_type = T;
	using element_type = 
};

// export template <typename T>
// struct iterator_traits_t <T *>
// {
// 	using element_type = T;
// 	using sentinel = sentinel_traits<T *>;
// };

// export template <typename T>
// struct iterator_traits_t <T const *> //: sentinel_traits <T const*>
// {
// 	using element_type = T;
// 	using sentinel = sentinel_traits<T const *>;
// };

// export template <typename T>
// struct iterator_traits_t <T const *const> //: sentinel_traits <T const * const>
// {
// 	using sentinel = sentinel_traits<T const *const>;
// 	using element_type = T;
// };