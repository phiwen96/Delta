export module Delta.Array;

import Delta.Size;

template <typename T>
struct is_array_t 
{
	constexpr static auto value = false;
};


export template <typename T>
concept Array = is_array_t <T>::value;

export template <typename T>
concept Dynamic_array = Array <T> and is_array_t <T>::dynamic;

export template <typename T>
concept Static_array = Array <T> and not is_array_t <T>::dynamic;

export template <Static_array T>
inline auto length (T const& t) noexcept -> Size auto
{
	return is_array_t <T>::length;
} 

export template <Array T>
using element_type = typename is_array_t <T>::element_type;

template <typename T, auto N>
struct is_array_t <T [N]>
{
	using element_type = T;
	constexpr static auto value = true;
	constexpr static auto dynamic = false;
	constexpr static auto length = N;
};

template <typename T, auto N>
struct is_array_t <T (&) [N]>
{
	using element_type = T;
	constexpr static auto value = true;
	constexpr static auto dynamic = false;
	constexpr static auto length = N;
};

template <typename T, auto N>
struct is_array_t <T const [N]>
{
	using element_type = T;
	constexpr static auto value = true;
	constexpr static auto dynamic = false;
	constexpr static auto length = N;
};

template <typename T>
struct is_array_t <T *>
{
	using element_type = T;
	constexpr static auto value = true;
	constexpr static auto dynamic = true;
};
