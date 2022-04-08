module Delta.Concepts.Pointer:Traits;

template <typename T>
concept PointerTraits =  requires () 
{
	typename T::element_type;
};

template <typename T>
struct pointer_traits;

template <typename T>
struct pointer_traits <T*>
{
	using element_type = T;
};

template <typename T>
struct pointer_traits <T const *>
{
	using element_type = T;
};

template <typename T>
struct pointer_traits <T const * &>
{
	using element_type = T;
};

template <typename T>
struct pointer_traits <T * const>
{
	using element_type = T;
};

template <typename T>
struct pointer_traits <T const * const>
{
	using element_type = T;
};

template <typename T>
struct pointer_traits <T*&&>
{
	using element_type = T;
};

template <typename T>
struct pointer_traits <T*&>
{
	using element_type = T;
};

