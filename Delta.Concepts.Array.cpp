export module Delta.Concepts.Array;

import Delta.Concepts.Size;

template <typename T>
struct array_traits 
{
	constexpr static auto is_array = false;
};

export template <typename T>
concept Array = array_traits <T>::is_array;

export template <Array T>
auto len (T const& t) noexcept -> Size auto
{
	return array_traits <T>::len;
} 

export template <Array T>
using element_type = typename array_traits <T>::element_type;

template <typename T, auto N>
struct array_traits <T [N]>
{
	using element_type = T;
	constexpr static auto is_array = true;
	constexpr static auto is_dynamic = false;
	constexpr static auto len = N;
};

template <typename T, auto N>
struct array_traits <T (&) [N]>
{
	using element_type = T;
	constexpr static auto is_array = true;
	constexpr static auto is_dynamic = false;
	constexpr static auto len = N;
};

template <typename T, auto N>
struct array_traits <T (&&) [N]>
{
	using element_type = T;
	constexpr static auto is_array = true;
	constexpr static auto is_dynamic = false;
	constexpr static auto len = N;
};

template <typename T, auto N>
struct array_traits <T const [N]>
{
	using element_type = T;
	constexpr static auto is_array = true;
	constexpr static auto is_dynamic = false;
	constexpr static auto len = N;
};


template <typename T>
struct array_traits <T []>
{
	using element_type = T;
	constexpr static auto is_array = true;
	constexpr static auto is_dynamic = true;
};

template <typename T>
struct array_traits <T (&) []>
{
	using element_type = T;
	constexpr static auto is_array = true;
	constexpr static auto is_dynamic = true;
};

template <typename T>
struct array_traits <T (&&) []>
{
	using element_type = T;
	constexpr static auto is_array = true;
	constexpr static auto is_dynamic = true;
};

template <typename T>
struct array_traits <T const []>
{
	using element_type = T;
	constexpr static auto is_array = true;
	constexpr static auto is_dynamic = true;
};




consteval auto test_array () noexcept -> bool
{
	int t [10];

	static_assert (Array <decltype (t)>);

	return true;
}

static_assert (test_array ());