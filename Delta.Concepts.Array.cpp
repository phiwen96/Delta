export module Delta.Concepts.Array;

template <typename T>
struct array_info
{
	static constexpr auto is_array = false;
};

export template <typename T>
concept Array = array_info <T>::is_array;

export constexpr auto length (Array auto&& range) noexcept -> auto 
{
	return array_info <decltype (range)>::length;
}

template <typename T, auto N>
struct array_info <T [N]>
{
	static constexpr auto is_array = true;
	static constexpr auto length = N;
};

template <typename T, auto N>
struct array_info <T const [N]>
{
	static constexpr auto is_array = true;
	static constexpr auto length = N;
};

template <typename T, auto N>
struct array_info <T const (&) [N]>
{
	static constexpr auto is_array = true;
	static constexpr auto length = N;
};

static_assert (Array <char [10]>);
static_assert (Array <char const [10]>);
static_assert (Array <char const (&) [10]>);