// module;
// #include <utility>
export module Delta.Common;

export {
	

template <typename T>
struct add_r_value_t
{
	using type = T &&;
};

template <typename T>
struct add_r_value_t<T *>
{
	using type = T *;
};

template <typename T>
struct add_r_value_t<T const *>
{
	using type = T const *&&;
};

template <typename T>
struct add_r_value_t<T &>
{
	using type = T &&;
};

template <typename T>
struct add_r_value_t<T const &>
{
	using type = T const &;
};

template <typename T>
using add_r_value = typename add_r_value_t<T>::type;

template<typename _Tp, typename _Up = _Tp&&>
    constexpr _Up
    __mimic(int) noexcept;

  template<typename _Tp>
    constexpr _Tp
    __mimic(long) noexcept;

  template<typename _Tp>
    constexpr auto mimic() noexcept -> decltype(__mimic<_Tp>(0));
// template <typename T>
// constexpr auto mimic() noexcept -> add_r_value<T>;

template <typename T, typename U>
struct same_t
{
	constexpr static bool value = false;
};

template <typename T, typename U>
concept Same = same_t<T, U>::value;

template <typename T>
struct same_t<T, T>
{
	constexpr static bool value = true;
};

template <typename...>
struct strip_t;

template <typename T>
using strip = typename strip_t<T>::result;

template <typename T, typename U>
concept Strip = Same<strip<T>, U>;

template <typename T>
struct strip_t<T>
{
	using result = T;
};

template <typename T>
struct strip_t<T *>
{
	using result = strip<T>;
};

template <typename T>
struct strip_t<T &>
{
	using result = strip<T>;
};

template <typename T>
struct strip_t<T &&>
{
	using result = strip<T>;
};

template <typename T>
struct strip_t<T const>
{
	using result = strip<T>;
};



template <typename T, typename U>
concept Convertible = requires(T t, U u)
{
	u = t;
};

template <bool, typename T, typename U>
struct if_else_t;

template <typename T, typename U>
struct if_else_t<true, T, U>
{
	using type = T;
};

template <typename T, typename U>
struct if_else_t<false, T, U>
{
	using type = U;
};

template <bool B, typename T, typename U>
using if_else = typename if_else_t<B, T, U>::type;


	// A type predicate is a lambda that, when called
	// with a type, should return a boolean value.

template <typename T, typename U>
concept TypePredicate = requires(T &t)
{
	{
		t.template operator()<U>()
		} -> Same<bool>;
};

template <auto, typename...>
struct numbered_typelist;

template <auto N, typename T, typename... U>
struct numbered_typelist<N, T, U...>
{
	using type = T;
	constexpr static auto index = N;

	template <auto i>
	requires(i >= N and i <= N + sizeof...(U)) using get = if_else<i == index, type, typename numbered_typelist<N + 1, U...>::type>;
};

template <auto N, typename T>
struct numbered_typelist<N, T>
{
	using type = T;
	constexpr static auto index = N;

	template <auto i>
	requires(i == index) using get = type;
};

template <typename... T>
struct typelist;

template <typename T, typename... U>
struct typelist<T, U...> //: indexed_element <0, T>
{
	template <template <typename...> typename V>
	using transform = V<T, U...>;

	constexpr static auto reversed_index = sizeof...(U);

	template <auto i>
	using get = typename numbered_typelist<0, T, U...>::template get<i>;
};

template <auto predicate, typename T, typename... U>
struct any_of_t
{
	constexpr static auto value = predicate.template operator()<T>() ? true : any_of_t<predicate, U...>::value;
};

template <auto predicate, typename T>
struct any_of_t<predicate, T>
{
	constexpr static auto value = predicate.template operator()<T>() ? true : false;
};

// typelist

template <auto predicate, typename... U>
struct any_of_t<predicate, typelist<U...>> : any_of_t<predicate, U...>
{
};

template <auto predicate, typename... U, typename... V>
struct any_of_t<predicate, typelist<U...>, V...> : any_of_t<predicate, U..., V...>
{
};

template <auto predicate, typename... T>
constexpr auto any_of = any_of_t<predicate, T...>::value;

template <auto predicate, typename... T>
concept AnyOf = (TypePredicate<decltype(predicate), T> and ...) and any_of<predicate, T...>;

template <auto predicate, typename T, typename... U>
struct all_of_t
{
	constexpr static auto value = predicate.template operator()<T>() ? all_of_t<predicate, U...>::value : false;
};

template <auto predicate, typename T>
struct all_of_t<predicate, T>
{
	constexpr static auto value = predicate.template operator()<T>() ? true : false;
};

// expand nested typelist, but what if vector

template <auto predicate, typename... U>
struct all_of_t<predicate, typelist<U...>> : all_of_t<predicate, U...>
{
};

template <auto predicate, typename... U, typename... V>
struct all_of_t<predicate, typelist<U...>, V...> : all_of_t<predicate, U..., V...>
{
};

template <auto predicate, typename... T>
constexpr auto all_of = all_of_t<predicate, T...>::value;

template <auto predicate, typename... T>
concept AllOf = (TypePredicate<decltype(predicate), T> and ...) and all_of<predicate, T...>;



// constexpr auto is_function (auto&&...) noexcept -> bool
// {
// 	return false;
// }

// template <typename T, typename... U>
// constexpr auto is_function (T (*) (U...)) noexcept -> bool
// {
// 	return true;
// }

template <typename T>
concept FunctionTraits = requires
{
	typename T::return_type;
};

template <typename... T>
struct function_traits_t;

template <typename T>
struct function_traits_t<T()>
{
	using return_type = T;
	constexpr static auto nr_of_params = 0;
};

template <typename T>
struct function_traits_t<T() noexcept>
{
	using return_type = T;
	constexpr static auto nr_of_params = 0;
};

template <typename T, typename... U>
struct function_traits_t<T(U...)>
{
	using return_type = T;
	constexpr static auto nr_of_params = sizeof...(U);
	using params = typelist<U...>;
};

template <typename T, typename... U>
struct function_traits_t<T(U...) noexcept>
{
	using return_type = T;
	constexpr static auto nr_of_params = sizeof...(U);
	using params = typelist<U...>;
};

template <typename T>
concept Function = FunctionTraits<function_traits_t<T>>;

template <Function T, auto I>
using fun_param_type = typename function_traits_t<T>::params::template get<I>;

template <Function T>
using fun_ret_type = typename function_traits_t<T>::return_type;

template <typename T>
concept PointerTraits = requires()
{
	typename T::pointer_type;
};

template <typename... T>
struct pointer_traits_t;

template <typename T>
concept HasDefinedPointerTraits = PointerTraits<pointer_traits_t<T>>;

template <typename T>
struct pointer_traits_t<T *>
{
	using pointer_type = T *;
};

template <typename T>
struct pointer_traits_t<T const *>
{
	using pointer_type = T const *;
};

template <typename T>
struct pointer_traits_t<T const *&>
{
	using pointer_type = T const *&;
};

template <typename T>
struct pointer_traits_t<T *const>
{
	using pointer_type = T *const;
};

template <typename T>
struct pointer_traits_t<T *const&>
{
	using pointer_type = T *const&;
};

template <typename T>
struct pointer_traits_t<T const *const>
{
	using pointer_type = T const *const;
};

template <typename T>
struct pointer_traits_t<T *&&>
{
	using pointer_type = T *&&;
};

template <typename T>
struct pointer_traits_t<T *&>
{
	using pointer_type = T *&;
};

template <typename T>
concept Pointer = HasDefinedPointerTraits<T>;

template <typename T>
using pointer_types = typelist<T *, T *&, T *&&, T const *, T const *&, T *const, T *const&, T const *const>;


using char_types = typelist<char, signed char, unsigned char, char16_t, char32_t, wchar_t>;

template <typename T>
concept Char = AnyOf<[]<typename C>
					 { return Strip<T, C>; },
					 char_types>;



template <typename T>
concept Size = requires(T t, decltype(alignof(char)) u)
{
	u = t;
};


template <template <typename...> typename T, typename...>
struct product_type_t;


template <template <typename...> typename TypeTransformer, template <typename...> typename Typelist, typename... Element>
struct product_type_t <TypeTransformer , Typelist <Element...>>
{
	using result = Typelist <TypeTransformer <Element>...>;
};

template <template <typename...> typename TypeTransformer, typename... Typelists>
using product_type = typename product_type_t <TypeTransformer, Typelists...>::result;

}

template <typename T>
using _p = typelist <T*, T*&>;

using _c = typelist <char16_t, char32_t>;
using _r = product_type <_p, _c>;
static_assert (Same <_r, typelist <typelist <char16_t*, char16_t*&>, typelist <char32_t*, char32_t*&>>>);
static_assert (AllOf <[]<typename T>{return Pointer <T>;}, _r>);
// static_assert (Same <_r, typelist <char16_t*, char16_t*&, char32_t*, char32_t*&>>);

// static_assert(not Same<char, typelist<char, int>::get<1>>);
// static_assert(Same<typelist<int, double>, typelist<char, typelist<int, double>>::get<1>>);
// static_assert(Same<char, typelist<char, int>::get<0>>);
// static_assert(Same<int, typelist<char, int>::get<1>>);
// static_assert(Same<int, typelist<char, int, bool>::get<1>>);

// static_assert(Same<strip<char const *>, char>);

// static_assert(AllOf<[]<typename T>{ return Pointer<T>; },pointer_types<int>>);
// static_assert(AllOf<[]<typename T>{ return Strip<T, int>; },pointer_types<int>>);
// static_assert(AllOf<[]<typename T>
// 					{ return Char<T>; },
// 					char_types>);