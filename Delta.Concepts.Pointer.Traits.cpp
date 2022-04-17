export module Delta.Concepts.Pointer:Traits;

export template <typename T>
concept PointerTraits =  requires () 
{
	typename T::pointer_type;
};

export template <typename... T>
struct pointer_traits_t;

export template <typename T>
concept HasDefinedPointerTraits = PointerTraits <pointer_traits_t <T>>;

export template <typename T>
struct pointer_traits_t <T*>
{
	using pointer_type = T *;
};

export template <typename T>
struct pointer_traits_t <T const *>
{
	using pointer_type = T const *;
};

export template <typename T>
struct pointer_traits_t <T const * &>
{
	using pointer_type = T const * &;
};

export template <typename T>
struct pointer_traits_t <T * const>
{
	using pointer_type = T * const;
};

export template <typename T>
struct pointer_traits_t <T const * const>
{
	using pointer_type = T const * const;
};

export template <typename T>
struct pointer_traits_t <T*&&>
{
	using pointer_type = T*&&;
};

export template <typename T>
struct pointer_traits_t <T*&>
{
	using pointer_type = T*&;
};