export module Delta.Range;

export import Delta.Iterator;

export template <typename T>
struct bounded_t
{
	constexpr static auto value = false;
};

export template <typename T>
concept Bounded = bounded_t<T>::value;


	// begin + end
	// 	or
	// begin + length

export template <typename T>
concept RangePolicies = requires(fun_param_type<decltype(T::begin), 0> &range)
{
	{
		T::begin(range)
	}
	noexcept->Iterator;
	// true;

		{
			T::end(range)
		}
		noexcept->Iterator;

};

export template <typename... T>
struct range_policies_t;

export template <typename T>
concept Range = RangePolicies <range_policies_t <T>>;

export template <typename...>
struct range_type_t;

export template <RangePolicies T>
struct range_type_t <T>
{
	using result = fun_param_type<decltype(T::begin), 0>;
};

export template <Range T>
struct range_type_t <T> : range_type_t <range_policies_t <T>>
{

};

export template <Range T>
requires (not Iterator <T>)
struct element_type_t <T>
{
	using result = strip <defer <fun_ret_type <decltype(range_policies_t <T>::begin)>>>;
};

export template <typename T>
using range_type = typename range_type_t <T>::result;//fun_param_type<decltype(range_policies_t <T>::begin), 0>;

export constexpr auto begin(Range auto&& range) noexcept -> Iterator auto requires requires
{
	{
		range_policies_t<decltype (range)>::begin(range)
	}
	noexcept->Iterator;
}
{
	return range_policies_t<decltype (range)>::begin(range);
}

export constexpr auto end(Range auto&& range) noexcept -> Iterator auto requires requires
{
	{range_policies_t<decltype (range)>::end(range)} noexcept->Iterator;
}
{
	return range_policies_t<decltype (range)>::end(range);
}




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





