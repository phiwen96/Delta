
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

template <typename T>
constexpr auto mimic() noexcept -> add_r_value<T>;

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

static_assert(Same<strip<char const *>, char>);

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

static_assert(not Same<char, typelist<char, int>::get<1>>);
static_assert(Same<typelist<int, double>, typelist<char, typelist<int, double>>::get<1>>);
static_assert(Same<char, typelist<char, int>::get<0>>);
static_assert(Same<int, typelist<char, int>::get<1>>);
static_assert(Same<int, typelist<char, int, bool>::get<1>>);

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
using pointer_types = typelist<T *, T *&, T *&&, T const *, T const *&, T *const, T const *const>;

static_assert(AllOf<[]<typename T>
					{ return Pointer<T>; },
					pointer_types<int>>);

using char_types = typelist<char, signed char, unsigned char, char16_t, char32_t, wchar_t>;

template <typename T>
concept Char = AnyOf<[]<typename C>
					 { return Strip<T, C>; },
					 char_types>;

static_assert(AllOf<[]<typename T>
					{ return Char<T>; },
					char_types>);

template <typename T>
concept Size = requires(T t, decltype(alignof(char)) u)
{
	u = t;
};

struct iterator_tag
{
	struct INPUT
	{
	};
	struct OUTPUT
	{
	};
	struct FORWARD
	{
	};
	struct BIDIRECTIONAL
	{
	};
	struct RANDOM_ACCESS
	{
	};
	struct CONTIGUOUS
	{
	};
};

template <typename T>
concept IteratorTraits = requires
{
	typename T::element_type;
	typename T::iterator_type;
};

template <typename... T>
struct iterator_traits_t;

template <typename T>
concept HasDefinedIteratorTraits = IteratorTraits<iterator_traits_t<T>>;

template <typename A, typename B, typename C>
struct iterator_traits_t<A, B, C>
{
	using iterator_tag = A;
	using iterator_type = B;
	using element_type = C;
};

template <typename T>
using defer = decltype(*mimic<T>());

template <typename T>
concept ReadOnly = requires(T t)
{
	*t;
};

template <typename T>
concept WriteOnly = requires(T t, defer<T> u)
{
	*t = u;
};

template <typename T>
concept StepForward = requires(T t)
{
	++t;
	t++;
};

template <typename T>
concept StepBackward = requires(T t)
{
	--t;
	t--;
};

template <typename T>
concept Jump = requires(T t)
{
	t + 10;
	t += 10;

	t - 10;
	t -= 10;
};

template <typename T>
concept InputIterator = ReadOnly<T> and StepForward<T>;

template <typename T>
concept OutputIterator = WriteOnly<T> and StepForward<T>;

template <typename T>
concept ForwardIterator = InputIterator<T> and WriteOnly<T>;

template <typename T>
concept BidirectionalIterator = ForwardIterator<T> and StepBackward<T>;

template <typename T>
concept RandomAccessIterator = BidirectionalIterator<T> and Jump<T>;

template <typename T>
struct is_contiguous_iterator_t
{
	constexpr static auto result = false;
};

template <typename T>
constexpr auto is_contiguous_iterator = is_contiguous_iterator_t<T>::result;

template <typename T>
concept ContiguousIterator = RandomAccessIterator<T> and is_contiguous_iterator<T>;

template <typename T>
concept Iterator = InputIterator<T> or OutputIterator<T> or ForwardIterator<T> or BidirectionalIterator<T> or RandomAccessIterator<T> or ContiguousIterator<T>;

template <typename...>
struct element_type_t;

template <Iterator T>
struct element_type_t<T>
{
	using result = defer<T>;
};

template <typename T>
using element_type = typename element_type_t<T>::result;

template <typename...>
struct sentinel_traits_t;

// template <typename T>
// concept HasDefinedSentinelTraits = SentinelTraits <sentinel_traits_t <T>>;

// static_assert (HasDefinedSentinelTraits <char const*>);

template <typename T>
concept Sentinel = Iterator<T> and requires {sentinel_traits_t <T>::value;};

// static_assert(requires {typename sentinel_traits_t <char const*>;});
// static_assert(requires {{sentinel_traits_t <char const*>::value()};});

// static_assert (Sentinel <char const*>);

// template <Sentinel T>
// constexpr auto sentinel_value = sentinel_traits_t <T>::value;

template <typename T>
struct bounded_t
{
	constexpr static auto value = false;
};

template <typename T>
concept Bounded = bounded_t<T>::value;


	// begin + end
	// 	or
	// begin + length

template <typename T>
concept RangePolicies = requires(fun_param_type<decltype(T::begin), 0> &range)
{
	{
		T::begin(range)
	}
	noexcept->Iterator;
	// true;

	requires requires
	{
		{
			T::end(range)
		}
		noexcept->Iterator;

	} or requires
	{
		{
			T::length(range)
		}
		noexcept->Size;
	};
};

template <typename... T>
struct range_policies_t;

// template <typename... T>
// struct get_range_policies;

// template <typename T>
// requires RangePolicies <range_policies_t <T>>
// struct get_range_policies <T> : range_policies_t <T> {};

template <typename T>
constexpr auto begin(T range) noexcept -> Iterator auto requires requires
{
	{
		range_policies_t<T>::begin(range)
	}
	noexcept->Iterator;
}
{
	return range_policies_t<T>::begin(range);
}

template <typename T>
constexpr auto end(T range) noexcept -> Iterator auto requires requires
{
	{
		range_policies_t<T>::end(range)
	}
	noexcept->Iterator;
}
{
	return range_policies_t<T>::end(range);
}

template <typename T>
concept Range = requires(T &t)
{
	{
		begin(t)
	}
	noexcept->Iterator;
	{
		end(t)
	}
	noexcept->Iterator;
};

// template <Range T>
// requires (not Iterator <T>)
// struct element_type_t <T>
// {
// 	using result = defer <fun_ret_type <decltype (range_policies_t <T>::begin)>>;
// };

template <Sentinel T>
requires(not Bounded<T>) struct range_policies_t<T>
{
	constexpr static auto begin(T t) noexcept -> Iterator auto
	{
		return t;
	}

	constexpr static auto end(T t) noexcept -> Iterator auto
	{
		auto i = t;

		while (*i != sentinel_traits_t<T>::value)
		{
			++i;
		}

		return i;
	}
};

template <typename T>
concept ArrayPolicies = RangePolicies<T>;

template <typename...>
struct array_policies_t;

template <typename T>
concept HasDefinedArrayPolicies = ArrayPolicies<array_policies_t<T>>;

template <HasDefinedArrayPolicies T>
struct range_policies_t<T> : array_policies_t<T>
{
};

template <typename T>
concept Array = HasDefinedArrayPolicies<T>;

template <Array T>
struct bounded_t<T>
{
	constexpr static auto value = true;
};

template <typename U, auto N>
using array_types = typelist<U[N], U (&)[N], U const (&)[N]>;

template <typename T, auto N>
struct array_policies_t<T[N]>
{
	constexpr static auto begin(T(range)[N]) noexcept -> auto *
	{
		return range;
	}

	constexpr static auto length(T(range)[N]) noexcept -> Size auto
	{
		return N;
	}
};

template <typename T, auto N>
struct array_policies_t<T (&)[N]>
{
	constexpr static auto begin(T (&range)[N]) noexcept -> auto *
	{
		return range;
	}

	constexpr static auto length(T (&range)[N]) noexcept -> Size auto
	{
		return N;
	}
};

static_assert(AllOf<[]<typename T>
					{ return Array<T>; },
					array_types<int, 10>>);
// static_assert (AllOf <[]<typename T>{return Range <T>;}, array_types <int, 10>>);
// static_assert(Range<int[10]>);

// template <typename T>
// using get_sentinel_traits_t <>

// static_assert (Char <get_element_type <char const*>>);

// static_assert (Char <element_type <char const*>>);

// static_assert (sentinel_value <char const*> == '\0');
// static_assert (Char <element_type <char const*>>);
template <Iterator T>
requires Char<defer<T>>
struct sentinel_traits_t<T>
{
	static constexpr auto value = '\0';
};
static_assert (Iterator <char const*>);
static_assert (Char <defer <char const*>>);

// template <>
// struct sentinel_traits_t<char const*>
// {
// 	static constexpr auto value = '\0';
// };

template <typename T>
concept String = Range<T> and Char<defer<fun_ret_type<decltype (range_policies_t<T>::begin)>>>;

// static_assert(Iterator<char const *>);
// static_assert (HasDefinedIteratorTraits <char const*>);

// // static_assert (sentinel_traits_t <char const*>::value == '\0');
// static_assert(String<char const *>);
// static_assert (Char <element_type <char[10]>>);
// // static_assert (Same <get_element_type <int[10]>, int>);
// static_assert (String <char [10]>);
// static_assert (Strip <get_element_type <char const*>, char>);
// static_assert (Strip <get_element_type <char[10]>, char>);
// static_assert (Same <get_element_type <char[10]>, char>);
// static_assert ()


// template <typename...>
// struct sentinel_value_t;

// template <typename T>
// concept Sentinel = requires {sentinel_value_t <T>::value;};

// template <Sentinel T>
// constexpr auto sentinel_value = sentinel_value_t <T>::value;

// template <Sentinel T>
// auto test (T t)
// {
// 	auto i = t;
// 	while (*i != sentinel_value <T>)
// 	{
// 		++i;
// 	}
	
// }

// template <>
// struct sentinel_value_t <char const*>
// {
// 	constexpr static auto value = '\0'; 
// };

// static_assert (Sentinel <char const*>);
static_assert (String <char const*>);
auto main(int, char **) -> int
{
	// test ("hej");
	// String auto s = "hej";
	return 0;
}