export module Delta.Concepts.Pointer.Traits;

export template <typename T>
concept PointerTraits =  requires () 
{
	typename T::element_type;
};

export template <typename T>
struct pointer_traits;

export template <typename T>
struct pointer_traits <T*>
{
	using element_type = T;
};

export template <typename T>
struct pointer_traits <T const *>
{
	using element_type = T;
};

export template <typename T>
struct pointer_traits <T const * &>
{
	using element_type = T;
};

export template <typename T>
struct pointer_traits <T * const>
{
	using element_type = T;
};

export template <typename T>
struct pointer_traits <T const * const>
{
	using element_type = T;
};

export template <typename T>
struct pointer_traits <T*&&>
{
	using element_type = T;
};

export template <typename T>
struct pointer_traits <T*&>
{
	using element_type = T;
};

