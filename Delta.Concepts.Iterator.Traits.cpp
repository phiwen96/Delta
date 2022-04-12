export module Delta.Concepts.Iterator.Traits;

import Delta.Concepts.Pointer;

export import Delta.Concepts.Iterator.Traits.Sentinel;

export template <typename T>
concept IteratorTraits = requires()
{
	typename T::iterator_type;
	typename T::element_type;
	// typename T::sentinel_traits;
};

export template <typename... T>
struct iterator_traits_t;

export template <typename iteratorType, typename elementType>
struct iterator_traits_t <iteratorType, elementType>
{
	using iterator_type = iteratorType;
	using element_type = elementType;
};


export template <Pointer T>
struct iterator_traits_t <T> : iterator_traits_t <typename pointer_traits_t <T>::iterator_type, typename pointer_traits_t <T>::element_type> {};




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

