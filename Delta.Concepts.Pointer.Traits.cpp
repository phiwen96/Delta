export module Delta.Concepts.Pointer:Traits;

export template <typename T>
concept PointerTraits =  requires () 
{
	typename T::pointer_type;
	typename T::element_type;
};

export template <typename... T>
struct pointer_traits_t;

export template <typename T>
concept HasDefinedPointerTraits = PointerTraits <pointer_traits_t <T>>;

export template <typename pointerType, typename elementType>
struct pointer_traits_t <pointerType, elementType>
{
	using pointer_type = pointerType;
	using element_type = elementType;
};

export template <typename T>
struct pointer_traits_t <T*> : pointer_traits_t <T*, T> {};

export template <typename T>
struct pointer_traits_t <T const *> : pointer_traits_t <T const *, T> {};

export template <typename T>
struct pointer_traits_t <T const * &> : pointer_traits_t <T const * &, T> {};

export template <typename T>
struct pointer_traits_t <T * const> : pointer_traits_t <T * const, T> {};

export template <typename T>
struct pointer_traits_t <T const * const> : pointer_traits_t <T const * const, T> {};

export template <typename T>
struct pointer_traits_t <T*&&> : pointer_traits_t <T*&&, T> {};

export template <typename T>
struct pointer_traits_t <T*&> : pointer_traits_t <T*&, T> {};

export template <typename...>
struct element_type_t;

export template <HasDefinedPointerTraits T>
struct element_type_t <T>
{
	using type = typename pointer_traits_t <T>::pointer_type;
};
