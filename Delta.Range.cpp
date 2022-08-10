export module Delta.Range;

template <auto r>
struct value {constexpr static decltype (r) result = r;};

template <typename r>
struct type {using result = r;};

template <typename lhs, typename rhs>
struct is_same : value <false> {};

template <typename t>
struct is_same <t, t> : value <true> {};

template <typename...>
struct maybe_first_of : type <void> {};

template <typename a, typename... b>
struct maybe_first_of <a, b...> : type <a> {};

export template <typename lhs, typename rhs>
concept Same = is_same <lhs, rhs>::result;

export template <typename iterator, typename element> 
concept Iterator = requires (iterator i) {
	*i -> Same <element>;
	++i;
	--i;
	*i;
	// requires true;
};

auto begin (auto&& range) noexcept requires requires {range.begin() -> Iterator;} {
	return range.begin();
}

auto end (auto&& range) noexcept requires requires {range.end() -> Iterator;} {
	return range.end();
}

export template <typename from, typename to>
concept Convertible_to = requires (from f, to t) {
	t = f;
};
export template <typename size>
concept Size = Convertible_to <size, unsigned int>;

// auto len (auto&& range) noexcept -> Size auto requires {
// }

export template <typename range, typename element> 
concept Range = requires (range r) {
	begin (r) -> Iterator <element>;
	end (r) -> Iterator <element>;
	len (r) -> Size;
};



// export import Delta.Iterator;

// export template <typename T>
// struct bounded_t
// {
// 	constexpr static auto value = false;
// };

// export template <typename T>
// concept Bounded = bounded_t<T>::value;


// 	// begin + end
// 	// 	or
// 	// begin + length

// export template <typename T>
// concept RangePolicies = requires(fun_param_type<decltype(T::begin), 0> &range)
// {
// 	{
// 		T::begin(range)
//     } 
// 	noexcept->Iterator;

// 	// true;

// 		{
// 			T::end (range)
// 		}
// 		noexcept->Iterator;

// };

// export template <typename... T>
// struct range_policies_t;

// export template <typename T>
// requires requires (T t) {
// 	{t.begin ()} noexcept -> Iterator;
// 	{t.end ()} noexcept -> Iterator;
// }
// struct range_policies_t <T>  {
// 	constexpr static auto begin (T& t) noexcept -> Iterator auto {
// 		return t.begin();
// 	}
// 	constexpr static auto end (T& t) noexcept -> Iterator auto {
// 		return t.end();
// 	}
// };

// export template <typename T>
// concept Range = RangePolicies <range_policies_t <T>>;

// export template <typename...>
// struct range_type_t;

// export template <RangePolicies T>
// struct range_type_t <T>
// {
// 	using result = fun_param_type<decltype(T::begin), 0>;
// };

// export template <Range T>
// struct range_type_t <T> : range_type_t <range_policies_t <T>>
// {

// };

// export template <Range T>
// requires (not Iterator <T>)
// struct element_type_t <T>
// {
// 	using result = 
// 	strip <
// 		defer <
// 			fun_ret_type <
// 				decltype (range_policies_t <T>::begin)
// 			>
// 		>
// 	>;
// };

// export template <typename T>
// using range_type = typename range_type_t <T>::result;//fun_param_type<decltype(range_policies_t <T>::begin), 0>;

// export constexpr auto begin(Range auto&& range) noexcept -> Iterator auto requires requires
// {
// 	{
// 		range_policies_t<decltype (range)>::begin(range)
// 	}
// 	noexcept->Iterator;
// }
// {
// 	return range_policies_t<decltype (range)>::begin(range);
// }

// export constexpr auto end(Range auto&& range) noexcept -> Iterator auto requires requires
// {
// 	{range_policies_t<decltype (range)>::end(range)} noexcept->Iterator;
// }
// {
// 	return range_policies_t<decltype (range)>::end(range);
// }




// {

// 	{begin(t)} noexcept->Iterator;
// 	{end(t)} noexcept->Iterator;
// };

// template <Range T>
// requires (not Iterator <T>)
// struct element_type_t <T>
// {
// 	using result = defer <fun_ret_type <decltype (range_policies_t <T>::begin)>>;
// };





