export module Delta.Convertible_to;

// template <typename T, typename U>
// struct convertible_to_t 
// {
// 	constexpr static bool value = false;
// };

export template <typename T, typename U>
concept Convertible_to = requires (T t, U u)
{
	u = t;
};





// template <typename T>
// struct convertible_to_t <T, T>
// {
// 	constexpr static bool value = true;
// };