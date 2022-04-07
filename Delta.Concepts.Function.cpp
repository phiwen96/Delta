export module Delta.Concepts.Function;

template <typename T>
struct function_traits 
{
	constexpr static auto is_function = false;
};

template <typename T>
struct function_traits <T()>
{
	constexpr static auto is_function = true;
	using return_value = T;
	constexpr static auto params = 0;
};

template <typename T, typename... U>
struct function_traits <T(U...)>
{
	constexpr static auto is_function = true;
	using return_value = T;
	constexpr static auto params = sizeof...(U);
	
};

