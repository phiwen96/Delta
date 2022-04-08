module Delta.Concepts.Function:Traits;

import Delta.Concepts.Number.Signed;

template <typename T>
concept FunctionTraits = requires ()
{
	typename T::return_value;

	{T::params} -> Signed;
};

template <typename T>
struct function_traits_t;


template <typename T>
struct function_traits_t <T()>
{
	using return_value = T;
	constexpr static auto params = 0;
};

template <typename T, typename... U>
struct function_traits_t <T(U...)>
{
	using return_value = T;
	constexpr static auto params = sizeof...(U);
};


