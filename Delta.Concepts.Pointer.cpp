export module Delta.Concepts.Pointer;

template <typename T>
struct type_info 
{
	constexpr static auto is_pointer = false;
};


export template <typename T>
concept Pointer = type_info <T>::is_pointer;


export template <Pointer T>
using element_type = typename type_info <T>::element_type;

template <typename T>
struct type_info <T*>
{
	using element_type = T;
	constexpr static auto is_pointer = true;
};

template <typename T>
struct type_info <T const*>
{
	using element_type = T;
	constexpr static auto is_pointer = true;
};

template <typename T>
struct type_info <T*&&>
{
	using element_type = T;
	constexpr static auto is_pointer = true;
};

template <typename T>
struct type_info <T*&>
{
	using element_type = T;
	constexpr static auto is_pointer = true;
};
