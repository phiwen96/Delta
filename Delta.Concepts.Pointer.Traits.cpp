module Delta.Concepts.Pointer:Traits;

export template <typename T>
concept PointerTraits =  requires () 
{
	typename T::element_type;
};

export template <typename... T>
struct pointer_traits_t;

export template <typename pointerType, typename elementType>
struct pointer_traits_t <pointerType, elementType>
{
	using pointer_type = pointerType;
	using element_type = elementType;
};

export template <typename T>
struct pointer_traits_t <T*>
{
	using element_type = T;
};

export template <typename T>
struct pointer_traits_t <T const *>
{
	using element_type = T;
};

export template <typename T>
struct pointer_traits_t <T const * &>
{
	using element_type = T;
};

export template <typename T>
struct pointer_traits_t <T * const>
{
	using element_type = T;
};

export template <typename T>
struct pointer_traits_t <T const * const>
{
	using element_type = T;
};

export template <typename T>
struct pointer_traits_t <T*&&>
{
	using element_type = T;
};

template <typename T>
struct pointer_traits_t <T*&>
{
	using element_type = T;
};

