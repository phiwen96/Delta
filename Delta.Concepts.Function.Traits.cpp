export module Delta.Concepts.Function.Traits;

import Delta.Types.List;
import Delta.Concepts.Number.Signed;

constexpr auto is_function (auto&&...) noexcept -> bool
{
	return false;
}

template <typename T, typename... U>
constexpr auto is_function (T (*) (U...)) noexcept -> bool
{
	return true;
}



export template <typename T>
concept FunctionTraits = requires ()
{
	typename T::return_type;

	// {T::params} -> Signed;
};

export template <typename T>
struct function_traits_t;


export template <typename T>
struct function_traits_t <T()>
{
	using return_type = T;
	constexpr static auto nr_of_params = 0;
};

export template <typename T>
struct function_traits_t <T() noexcept>
{
	using return_type = T;
	constexpr static auto nr_of_params = 0;
};

export template <typename T, typename... U>
struct function_traits_t <T(U...)>
{
	using return_type = T;
	constexpr static auto nr_of_params = sizeof...(U);
	using params = typelist <U...>;
};

export template <typename T, typename... U>
struct function_traits_t <T(U...) noexcept>
{
	using return_type = T;
	constexpr static auto nr_of_params = sizeof...(U);
	using params = typelist <U...>;
};




