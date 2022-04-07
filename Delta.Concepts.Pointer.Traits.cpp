export module Delta.Concepts.Pointer.Traits;

export template <typename T>
concept PointerTraits =  requires () 
{
	requires not T::is_pointer or requires
	{
		typename T::element_type;
	};
};

export template <typename T>
struct pointer_traits 
{
	constexpr static auto is_pointer = false;
};

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

