export module Delta.Pointer;
import Delta.Types;


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


export template <typename T>
concept Pointer = HasDefinedPointerTraits <T>;

export template <typename T>
using pointer_types = typelist <T*, T*&, T*&&, T const*, T const*&, T *const, T const * const>;

static_assert (AllOf <[] <typename T> {return Pointer <T>;}, pointer_types <int>>);