export module Delta.Concepts.Range.Traits;

import Delta.Concepts.Bool;
import Delta.Concepts.Size;
import Delta.Concepts.Same;
import Delta.Concepts.Iterator;
import Delta.Concepts.Range.Policies;
import Delta.Concepts.Range.Element;
import Delta.Concepts.Function;


template <typename T>
requires RangePolicies <range_policies_t <T>>
using begin_param_type = fun_param_type <decltype (range_policies_t <T>::begin), 0>;

template <typename T>
requires RangePolicies <range_policies_t <T>>
using begin_ret_type = fun_ret_type <decltype (range_policies_t <T>::begin)>;


export template <typename T>
concept RangeTraits = 
	Same <begin_param_type <T>, typename T::range_type> and
	Same <begin_ret_type <T>, typename T::iterator_type>;

export template <typename... T>
struct range_traits_t;

export template <typename T>
concept HasRangeTraits = RangeTraits <range_traits_t <T>>;

export template <typename T>
requires RangePolicies <range_policies_t <T>>
struct range_traits_t <T>
{
	using range_type = fun_param_type <decltype (T::begin), 0>;
	using iterator_type = fun_ret_type <decltype (T::begin)>;
};

// template <typename T>
// requires RangeTraits <range_traits_t <T>>
// constexpr auto begin (T range) noexcept -> Iterator auto 
// {
// 	return range_traits_t <T>::begin (range);
// }

// template <typename T>
// requires RangeTraits <range_traits_t <T>>
// constexpr auto end (T range) noexcept -> Iterator auto 
// {
// 	return range_traits_t <T>::end (range);
// }
