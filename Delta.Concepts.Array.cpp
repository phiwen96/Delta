export module Delta.Concepts.Array;

import Delta.Concepts.Size;

template <typename T>
struct type_info 
{
	constexpr static auto is_array = false;
};

export template <typename T>
concept Array = type_info <T>::is_array;

export template <Array T>
auto len (T const& t) noexcept -> Size auto
{
	return type_info <T>::len;
} 

export template <Array T>
using element_type = typename type_info <T>::element_type;

template <typename T, auto N>
struct type_info <T [N]>
{
	using element_type = T;
	constexpr static auto is_array = true;
	constexpr static auto is_dynamic = false;
	constexpr static auto len = N;
};

template <typename T, auto N>
struct type_info <T (&) [N]>
{
	using element_type = T;
	constexpr static auto is_array = true;
	constexpr static auto is_dynamic = false;
	constexpr static auto len = N;
};

template <typename T, auto N>
struct type_info <T (&&) [N]>
{
	using element_type = T;
	constexpr static auto is_array = true;
	constexpr static auto is_dynamic = false;
	constexpr static auto len = N;
};

template <typename T, auto N>
struct type_info <T const [N]>
{
	using element_type = T;
	constexpr static auto is_array = true;
	constexpr static auto is_dynamic = false;
	constexpr static auto len = N;
};

