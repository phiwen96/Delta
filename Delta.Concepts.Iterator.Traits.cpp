export module Delta.Concepts.Iterator.Traits;

export import Delta.Concepts.Iterator.Traits.Sentinel;

export template <typename T>
concept IteratorTraits = requires ()
{
	typename T::element_type;
	typename T::sentinel;
};

export template <typename T>
struct iterator_traits;

export template <typename T>
struct iterator_traits <T*>
{
	using element_type = T;
	using sentinel = sentinel_traits <T*>;
};

export template <typename T>
struct iterator_traits <T const *> //: sentinel_traits <T const*>
{
	using element_type = T;
	using sentinel = sentinel_traits <T const*>;
};

export template <typename T>
struct iterator_traits <T const * const> //: sentinel_traits <T const * const>
{
	using sentinel = sentinel_traits <T const * const>;
	using element_type = T;
};