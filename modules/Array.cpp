export module Delta.Array;


template <typename T>
struct is_array_t 
{
	constexpr static bool value = false;
};

template <typename T>
constexpr auto is_array = is_array_t <T>::value;

export template <typename T>
concept Array = is_array <T>;




template <typename T, auto N>
struct is_array_t <T [N]>
{
	constexpr static bool value = true;
};