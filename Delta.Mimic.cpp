module;
#include <type_traits>

export module Delta:Mimic;

template <typename T>
struct add_r_value_t
{
	using type = T&&;
};

template <typename T>
struct add_r_value_t <T*>
{
	using type = T*;
};

template <typename T>
struct add_r_value_t <T const*>
{
	using type = T const*&&;
};

template <typename T>
struct add_r_value_t <T&>
{
	using type = T&&;
};

template <typename T>
struct add_r_value_t <T const&>
{
	using type = T const&;
};


template <typename T>
requires requires 
{
	add_r_value_t <T>::type;
}
using add_r_value = add_r_value_t <T>::type;


export template <class T>
constexpr auto mimic () noexcept -> add_r_value <T>;