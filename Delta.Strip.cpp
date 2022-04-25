export module Delta.Strip;

import Delta.Same;

template <typename...>
struct strip_t;

export template <typename T>
using strip = typename strip_t <T>::result;

/*
	auto t (Strip <int>)
*/
export template <typename T, typename U>
concept Strip = Same <strip <T>, U>;

template <typename T>
struct strip_t <T>
{
	using result = T;
};

template <typename T>
struct strip_t <T*>
{
	using result = typename strip_t <T>::result;
};

template <typename T>
struct strip_t <T&>
{
	using result = typename strip_t <T>::result;
};

template <typename T>
struct strip_t <T&&>
{
	using result = typename strip_t <T>::result;
};

template <typename T>
struct strip_t <T const>
{
	using result = typename strip_t <T>::result;
};

static_assert (Same <strip <char const*>, char>);