export module Delta.Common;

#define EAT(...)

export template <auto T>
struct value_t {constexpr static auto value = T;};

template <template <typename...> typename typeList, typename type>
struct is_template_specialization_of_t : value_t <false> {};

template <template <typename...> typename typeList, typename... type>
struct is_template_specialization_of_t<typeList, typeList<type...>> : value_t <true> {};

template <template <typename...> typename typeList, typename type>
constexpr auto is_template_specialization_of = is_template_specialization_of_t<typeList, type>::value;

export template <typename T, template <typename...> typename typeList>
concept TemplateSpecializationOf = is_template_specialization_of <typeList, T>;

// template <typename T>
// struct get_

export template <typename T>
concept HasType = requires {typename T::type;};

export template <typename T>
struct type_t {using type = T;};

export template <HasType T>
using get_type = typename T::type;

template <typename T>
struct add_r_value_t : type_t <T&&> {};

template <typename T>
struct add_r_value_t <T*>  : type_t <T*> {};

template <typename T>
struct add_r_value_t <T const*> : type_t <T const*&&> {};

template <typename T>
struct add_r_value_t <T&> : type_t <T&&> {};

template <typename T>
struct add_r_value_t <T const&> : type_t <T const&> {};

export template <typename T>
using add_r_value = get_type <add_r_value_t <T>>;

template <typename _Tp, typename _Up = _Tp &&>
constexpr _Up __mimic(int) noexcept;

template <typename _Tp>
constexpr _Tp __mimic(long) noexcept;

export template <typename _Tp>
constexpr auto mimic() noexcept -> decltype(__mimic<_Tp>(0));

template <typename T, typename U>
struct same_t : value_t <false> {};

template <typename T>
struct same_t <T, T> : value_t <true> {};

export template <typename T, typename U>
concept Same = same_t<T, U>::value;

template <typename T>
struct strip_t : type_t <T> {};

template <typename T>
struct strip_t <T *> : strip_t <T> {};

template <typename T>
struct strip_t <T &> : strip_t <T> {};

template <typename T>
struct strip_t <T &&> : strip_t <T> {};

template <typename T>
struct strip_t <T const> : strip_t <T> {};

export template <typename T>
requires HasType <strip_t <T>>
using strip = get_type <strip_t <T>>;

//Strips T and check if same as U
export template <typename T, typename U>
concept Stripped = Same <strip <T>, U>;

static_assert (Stripped <char const, char>);
static_assert (!Stripped <char, char const>);
// static_assert (Same <strip <char const>, strip_t<char const>>);

export template <typename T, typename U>
concept Convertible = requires (T t, U u) {u = t;};

template <bool, typename T, typename U>
struct if_else_type_t;

template <typename T, typename U>
struct if_else_type_t <true, T, U> : type_t <T> {};

template <typename T, typename U>
struct if_else_type_t <false, T, U> : type_t <U> {};

export template <bool B, typename T, typename U>
using if_else_type = get_type <if_else_type_t <B, T, U>>;

// A type predicate is a lambda that, when called
// with a type, should return a boolean value.

export template <typename T, typename U>
concept TypePredicate = requires(T &t) {{t.template operator() <U> ()} -> Same <bool>;};

template <auto, typename...>
struct type_index_t;

struct null {};


template <>
struct type_index_t <0>
{
	constexpr static auto index = 0;
	using type = null;

	template <auto i>
	requires(i == 0) using get = type;
};

template <auto N, typename T, typename... U>
struct type_index_t <N, T, U...>
{
	using type = T;
	constexpr static auto index = N;

	template <auto i>
	requires(i >= N and i <= N + sizeof...(U)) using get = if_else_type<i == index, type, typename type_index_t <N + 1, U...>::type>;
};

template <auto N, typename T>
struct type_index_t <N, T>
{
	using type = T;
	constexpr static auto index = N;

	template <auto i>
	requires(i == index) using get = type;
};

export template <typename T>
concept Typelist = requires 
{
	typename T::template push_back <int>;
	typename T::template push_back <int, char, double>;

	typename T::template make_new <int>;
	typename T::template make_new <int, char, double>;

	// typename T::template switch_container <std::tuple>
	// typename T::template switch_containter <get_template <T>>;
};

export template <typename... T>
struct typelist
{
	template <typename... U>
	using push_back = typelist <T..., U...>;

	template <typename... U>
	using push_front = typelist <U..., T...>;

	template <template <typename...> typename U>
	using switch_to = U <T...>;
	
	template <typename... U>
	using make_new = typelist <U...>;

	template <template <typename...> typename U, typename... V>
	using transform = U <T..., V...>;

	constexpr static auto reversed_index = sizeof... (T);

	template <auto i>
	using get = typename type_index_t <0, T...>::template get<i>;
};

export template <Typelist T, typename... U>
using add_types = typename T::template push_back <U...>;

template <typename...>
struct get_t;

// template <typename... T>
// struct get_t <>

// template <typename... T>
// using unnested_typelist =

template <auto predicate, typename T, typename... U>
struct any_type_of_t
{
	constexpr static auto value = predicate.template operator()<T>() ? true : any_type_of_t<predicate, U...>::value;
};

template <auto predicate, typename T>
struct any_type_of_t<predicate, T>
{
	constexpr static auto value = predicate.template operator()<T>() ? true : false;
};

// typelist

template <auto predicate, typename... U>
struct any_type_of_t<predicate, typelist<U...>> : any_type_of_t <predicate, U...> {};

template <auto predicate, typename... U, typename... V>
struct any_type_of_t<predicate, typelist<U...>, V...> : any_type_of_t<predicate, U..., V...> {};

template <auto predicate, typename... T>
constexpr auto any_type_of = any_type_of_t <predicate, T...>::value;

export template <auto predicate, typename... T>
concept AnyOf = (TypePredicate<decltype(predicate), T> and ...) and any_type_of<predicate, T...>;

template <auto predicate, typename T, typename... U>
struct all_types_of_t : value_t <predicate.template operator()<T>() ? all_types_of_t<predicate, U...>::value : false> {};

template <auto predicate, typename T>
struct all_types_of_t<predicate, T> : value_t <predicate.template operator()<T>() ? true : false> {};

template <auto predicate, typename... U>
struct all_types_of_t<predicate, typelist<U...>> : all_types_of_t<predicate, U...> {};

template <auto predicate, typename... U, typename... V>
struct all_types_of_t <predicate, typelist<U...>, V...> : all_types_of_t<predicate, U..., V...> {};

template <auto predicate, typename... T>
constexpr auto all_types_of = all_types_of_t <predicate, T...>::value;

export template <auto predicate, typename... T>
concept AllTypesOf = all_types_of_t<predicate, T...>::value; //(TypePredicate<decltype(predicate), T> and ...) and all_types_of<predicate, T...>;

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

export template <typename T>
concept Function = FunctionTraits <function_traits_t<T>>;

export template <Function T, auto I>
using fun_param_type = typename function_traits_t<T>::params::template get<I>;

export template <Function T>
using fun_ret_type = typename function_traits_t<T>::return_type;

export template <typename T>
struct pointer_type_t {using pointer_type = T;};

template <typename T>
concept PointerTraits = requires {typename T::pointer_type;};

template <typename... T>
struct pointer_traits_t;

template <typename T>
concept HasDefinedPointerTraits = PointerTraits <pointer_traits_t<T>>;

template <typename T>
struct pointer_traits_t<T *> : pointer_type_t <T*> {};

template <typename T>
struct pointer_traits_t<T const *> : pointer_type_t <T const *> {};

template <typename T>
struct pointer_traits_t<T const *&> : pointer_type_t <T const *&> {};

template <typename T>
struct pointer_traits_t<T *const> : pointer_type_t <T *const> {};

template <typename T>
struct pointer_traits_t<T *const &> : pointer_type_t <T *const &> {};

template <typename T>
struct pointer_traits_t<T const *const> : pointer_type_t <T const *const> {};

template <typename T>
struct pointer_traits_t<T *&&> : pointer_type_t <T *&&> {};

template <typename T>
struct pointer_traits_t<T *&> : pointer_type_t <T *&> {};

export template <typename T>
concept Pointer = HasDefinedPointerTraits <T>;

export template <typename T, template <typename...> typename typeList = typelist>
using pointer_non_const_types = typeList <T *, T *&, T *&&, T const *, T const *&>;

export template <typename T>
using pointer_const_types = typelist<T *const, T *const &, T const *const>;

export template <typename T>
using pointer_types = typelist <pointer_non_const_types <T>, pointer_const_types<T>>;

export template <template <typename...> typename T>
using char_types = T <char, signed char, unsigned char, char16_t, char32_t, wchar_t>;

export template <typename T>
concept Char = AnyOf<[]<typename C>
					 { return Stripped <C, T>; },
					 char_types <typelist>>;

export template <typename T>
concept Size = requires(T t, decltype(alignof(char)) u)
{
	u = t;
};

export using floating_types = typelist<float, double, long double>;

export template <typename T>
concept Floating = AnyOf<[]<typename C>
						 { return Stripped <C, T>; },
						 floating_types>;

export using signed_types = typelist<short, short int, signed short, signed short int, int, signed, signed int, long, long int, signed long, signed long int, long long, long long int, signed long long, signed long long int>;

export template <typename T>
concept Signed = AnyOf<[]<typename U>
					   { return Stripped <U, T>; },
					   signed_types>;

export using unsigned_types = typelist<unsigned short, unsigned short int, unsigned, unsigned int, unsigned long, unsigned long int, unsigned long long, unsigned long long int>;

export template <typename T>
concept Unsigned = AnyOf<[]<typename U>
						 { return Stripped <U, T>; },
						 unsigned_types>;

template <typename T, typename... U>
struct type_iterator_t
{
	using type = T;
	constexpr static auto last = false;
	using next = type_iterator_t<U...>;
};

template <typename T>
struct type_iterator_t<T>
{
	using type = T;
	constexpr static auto last = true;
};

template <typename... types>
struct denested_type_insert_iterator_t
{
	// template <template <typename...> typename target, >
	// using type = push_type_back<typeList, if_else_type<is_template_specialization_of<target, T>, >>;
	// template <typename typeList>
	// using insert = push_type_back <typeList, if_else_type <is_template_specialization_of <>, >>
};







// template <typename... typeList>
// using denest = denest_t <>

template <typename... T>
struct find_suitable_typelist_t;

// template <typename... T>
// using denest = denest_t <>

// using integer_types = denest <signed_types, unsigned_types>;
// using integer_types = merge <unsigned_types, signed_types>;//typelist <short, short int, signed short, signed short int, int, signed, signed int, long, long int, signed long, signed long int, long long, long long int, signed long long, signed long long int, unsigned short, unsigned short int, unsigned, unsigned int, unsigned long, unsigned long int, unsigned long long, unsigned long long int>;

// template <typename T>
// concept Integer = AnyOf <[]<typename U>{return Strip <T, U>;}, integer_types>;

// // using number_types =
// template <typename T>
// concept Number = Signed <T> or Unsigned <T>;

// using number_types = typelist <>

template <typename... T>
struct for_all_types_t;

template <typename T>
struct for_all_types_t<T>
{
};

template <typename T, typename... U>
struct for_all_types_t<T, U...>
{
};

template <template <typename...> typename T, typename...>
struct product_type_t;

template <template <typename...> typename TypeTransformer, template <typename...> typename Typelist, typename... Element>
struct product_type_t <TypeTransformer, Typelist<Element...>> : type_t<Typelist<TypeTransformer<Element>...>> {};

/*
	product_type takes a transformation function and applies it to all types in a typelist
*/

export template <template <typename...> typename TypeTransformer, typename... Typelists>
using product_type = typename product_type_t<TypeTransformer, Typelists...>::type;

template <typename...>
struct type_node;

template <typename A>
struct type_node <A>
{
	constexpr static auto null = true;
	using type = A;

	// template <typename typeList>
	// using denest = push_type_back <typeList, T>;
};

template <typename A, typename... B>
struct type_node<A, B...>
{
	constexpr static auto null = false;
	using type = A;
	using next = type_node <B...>;

	// using next = type_node <U...>;
	// template <template <typename...> typename U, typename... V>
	// using type = U <>;
	// template <typename C>
	// using denest = typename type_node <typeList, B...>::template denest <tp_push_back <C, B>>;
};

// template <template <typename...> typename typeList, typename... types>
// using denest = for_all_types_t <types...>:: // typeList <if_else_type <contains_typelist <typeList, types>, int, types>...>;    //typename type_node <typelist, T...>::template denest <typelist <>>;

/*
	product_tp takes a transformation function and applies it to all types in a typelist
*/
template <template <typename...> typename typeFunction, typename... args>
struct product_tp_t
{
	// using result = type_node <>
	// using result = denested_tp <args...>
};

// template <template <typename...> typename type_transformer, typ

/*
	Testing contains_typelist
*/

using unnested_testing_types = typelist <char, double>;
using nested_testing_types = typelist <int, unnested_testing_types>;

// static_assert (contains_typelist <typelist, unnested_testing_types, nested_testing_types>);
// static_assert (contains_typelist <typelist, nested_testing_types>);

using t3 = typelist <int, char>;
// using t4 = denest <typelist, t3>;

// static_assert (Same <t3, t4>);
// static_assert (AllTypesOf <[]<typename T>{return Number <T>;}, integer_types>);

template <typename T>
using _p = typelist<T *, T *&>;

using _c = typelist<char16_t, char32_t>;
using _r = product_type<_p, _c>;
static_assert(Same<_r, typelist<typelist<char16_t *, char16_t *&>, typelist<char32_t *, char32_t *&>>>);
static_assert(AllTypesOf<[]<typename T>
					{ return Pointer<T>; },
					_r>);
// static_assert (Same <_r, typelist <char16_t*, char16_t*&, char32_t*, char32_t*&>>);

using t0 = typelist<int, char>;
using t1 = typelist<double, int, typelist<>>;
using t2 = typelist<t0, t1, typelist<t0, t1>>;

// static_assert (Same <denested_tp <typelist, t2>, typelist <int, char, double, int, int, char, double, int>>);
// static_assert (Same <denested_tp <typelist, int, t2, char>, typelist <int, int, char, double, int, int, char, double, int, char>>);