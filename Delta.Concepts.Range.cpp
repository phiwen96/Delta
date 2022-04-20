export module Delta.Concepts.Range;



export import Delta.Concepts.Iterator;
export import Delta.Concepts.Function;
export import Delta.Types;


/*
	begin + end 
		or 
	begin + length
*/
export template <typename T>
concept RangePolicies = requires (fun_param_type <decltype (T::begin), 0>& range)
{
	{T::begin (range)} noexcept -> Iterator;
	// true;

	requires requires 
	{
		{T::end (range)} noexcept -> Iterator;

	} or requires 
	{
		{T::length (range)} noexcept -> Size;
	};
};

export template <typename...>
struct get_range_policies_t;

export template <typename T>
// requires RangePolicies <typename get_range_policies_t <T>::result>
using get_range_policies = typename get_range_policies_t <T>::result;

export template <typename T>
concept HasDefinedRangePolicies = RangePolicies <get_range_policies <T>>;

export template <typename... T>
struct range_policies_t;

export template <typename T>
requires RangePolicies <range_policies_t <T>>
struct get_range_policies_t <T>
{
	using result = range_policies_t <T>;
};

// export template <typename...>
// struct begin_param_type_t;

// export template <typename T>
// using begin_param_type = typename begin_param_type_t <T>::result;

// export template <RangePolicies T>
// struct begin_param_type_t <T> {using result = fun_param_type <decltype (T::begin), 0>;};

// export template <typename...>
// struct begin_ret_type_t;

// export template <typename T>
// using begin_ret_type = typename begin_ret_type_t <T>::result;

// export template <RangePolicies T>
// struct begin_ret_type_t <T> {using result = fun_ret_type <decltype (T::begin)>;};

// export template <HasDefinedRangePolicies T>
// struct begin_param_type_t <T> : begin_param_type_t <get_range_policies <T>> {};

// export template <HasDefinedRangePolicies T>
// struct begin_ret_type_t <T> : begin_ret_type_t <get_range_policies <T>> {};


export template <SentinelValue T>
struct range_policies_t <T>
{
	constexpr static auto begin (T t) noexcept -> Iterator auto 
	{
		return t;
	}

	constexpr static auto end (T t) noexcept -> Iterator auto 
	{
		auto i = t;

		while (i != sentinel_value <T>)
		{
			++i;
		}
		
		return i;
	}
};

export template <HasDefinedRangePolicies T>
constexpr auto begin (T range) noexcept -> Iterator auto 
{
	return get_range_policies <T>::begin (range);
}

export template <HasDefinedRangePolicies T>
constexpr auto end (T range) noexcept -> Iterator auto 
{
	if constexpr (not requires {typename T::end;})
	{
		return get_range_policies <T>::begin (range) + get_range_policies <T>::length (range);

	} else 
	{
		return get_range_policies <T>::end (range);
	}
}

export template <typename T>
concept RangeTraits = IteratorTraits <typename T::iterator_traits> and requires ()
{
	typename T::range_type;
};

export template <typename T>
concept Range = HasDefinedRangePolicies <T>;// and HasDefinedRangeTraits <T>;

