export module Delta.Concepts.Function.Traits;

export import Delta.Concepts.Function.Traits.Params;
import Delta.Concepts.Number.Signed;

export template <typename T>
concept FunctionTraits = requires ()
{
	typename T::return_value;

	{T::params} -> Signed;
};

export template <typename T>
struct function_traits_t;


export template <typename T>
struct function_traits_t <T()>
{
	using return_value = T;
	constexpr static auto params = 0;
};

export template <typename T, typename... U>
struct function_traits_t <T(U...)>
{
	using return_value = T;
	constexpr static auto params = sizeof...(U);
	// template <auto n>
	// using param = 
};


