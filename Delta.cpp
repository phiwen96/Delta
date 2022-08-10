export module Delta;

// export import Delta.String;
export import Delta.Range;
export import Delta.Graphics;

export import std;
#ifdef MACOS
export import Darwin;
#endif

export template <typename T>
auto operator << (std::ostream& os, std::vector <T> const& v) noexcept -> std::ostream& requires requires (T const & t) {
	std::cout << t;
} {
	for (auto const& i : v)
		std::cout << i << std::endl;
	return os;
}

// static_assert (Array <char[10]>);
// static_assert (Range <char[10]>);
// static_assert (AllOf <>);
// export auto runk () -> int{return 5;}

// export template <Sentinel T>
// requires(not Array<T>)
// struct range_policies_t<T>
// {
// 	constexpr static auto begin(T t) noexcept -> Iterator auto
// 	{
// 		return t;
// 	}

// 	constexpr static auto end(T t) noexcept -> Iterator auto
// 	{
// 		auto i = t;

// 		while (*i != sentinel_value_t<T>::value)
// 		{
// 			++i;
// 		}

// 		return i;
// 	}
// };
// static_assert (not Char <int>);
// // generates weird bugs
// // static_assert (AllOf <[]<typename T>{return Range <T>;}, make_array_types <int>>);
// // static_assert (not Sentinel <int[10]>);
// // static_assert (Sentinel <int[10]>);
// // static_assert (Iterator <int[10]>);
// // static_assert (Range <int[10]>);
// // static_assert (AllOf <[]<typename T> {return String <T>;}, string_types>);
// // static_assert (AllOf <[]<typename T>{return Sentinel <T>;}, product_type <non_pointer_const_types, char_types>>);

// // Char<defer<fun_ret_type<decltype (range_policies_t<T>::begin)>>>
// // static_assert (AllOf <[]<typename T>{return Bounded <T>;}, make_array_types <int>>);

// // static_assert (Same <range_type <int[10]>, int>);

// static_assert (Array <int[10]>);
// // static_assert (Same <fun_param_type<decltype(array_policies_t <int(&)[10]>::begin), 0>, int(&)[10]>);
// // static_assert (Same <fun_param_type<decltype(array_policies_t <int const(&)[10]>::begin), 0>, int const(&)[10]>);
// // static_assert (Same <fun_param_type<decltype(array_policies_t <int[10]>::begin), 0>, int(&)[10]>);
// // static_assert (Same <range_type <int[10]>, int>);
// // static_assert (Range <int[10]>);
// // using test_make_array_types = product_type <make_array_types, > 
// // using array_test_types = unnested_tp <make_array_types >                                 
// // static_assert (AllTypesOf <[]<typename T>{return Array <T>;}, push_array_types <10, int>>);
// // static_assert (AllTypesOf <[]<typename T>{return Array <T>;}, push_array_types <10, char>>);
// // static_assert (AllTypesOf <[]<typename T> {return Array <T>;}, make_array_types <char_types <typelist>>>);
// /*
// 	accumulate types with policies as paramters to accumulate all true boolean values both for all or for one
// 	if possible, 
// Motivation:


// */
// static_assert (Range <int[10]>);
// static_assert (Range <int(&)[10]>);
// static_assert (Range <int const(&)[10]>);
// static_assert (not Iterator <int(&)[10]>);
// static_assert (not Iterator <int const(&)[10]>);
// // static_assert (not Iterator <int[10]>);
// static_assert (Same <element_type <int[10]>, int>);
// static_assert (Same <element_type <int(&)[10]>, int>);
// // export using bajw = element_type <int[10]>;
// // static_assert (Range <int[10]>);
// // static_assert (AllOf <[]<typename T> {return Bounded <T>;}, make_array_types <char_types>>);

// // static_assert (Same <make_array_types <>>)
// // static_assert (AllOf <[]<typename T>{return Range <T>;}, make_array_types <char, 2>>);

// // T *, T *&, T *&&, T const *, T const *&, T *const, T *const&, T const *const
// // static_assert (String <char*>);

// // static_assert (Sentinel <char* const>);
// // static_assert (Sentinel <char* const>&);
// // static_assert (Sentinel <char const * const>);
// // static_assert (AllOf <[]<typename T>{return Sentinel <T>;}, product_type <pointer_types, char_types>>);
// // static_assert (not Array <char*>);


// export auto operator += (auto & v, auto && t) noexcept -> auto& requires requires {
// 	v.push_back (std::forward <decltype (t)> (t));
// } {
// 	v.push_back (std::move (t));
// 	return v;
// }

