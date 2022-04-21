export module Delta:Same;

template <typename T, typename U>
struct same_t 
{
	constexpr static bool value = false;
};

export template <typename T, typename U>
concept Same = same_t <T, U>::value;





template <typename T>
struct same_t <T, T>
{
	constexpr static bool value = true;
};