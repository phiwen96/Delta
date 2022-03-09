export module Delta.Same_as;

template <typename T, typename U>
struct same_as_t 
{
	constexpr static bool value = false;
};

export template <typename T, typename U>
concept Same_as = same_as_t <T, U>::value;





template <typename T>
struct same_as_t <T, T>
{
	constexpr static bool value = true;
};