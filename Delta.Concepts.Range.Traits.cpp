export module Delta.Concepts.Range.Traits;

import Delta.Concepts.Bool;
import Delta.Concepts.Size;
import Delta.Concepts.Same;
import Delta.Concepts.Iterator;
import Delta.Concepts.Range.Policies;
import Delta.Concepts.Range.Element;
import Delta.Concepts.Function;


export template <typename T>
concept RangeTraits = requires ()
{
	{T::range_type};
	// requires Iterator <typename T::iterator_type>;
};
	// HasDefinedRangePolicies <T> and 
	// Same <begin_param_type <T>, typename T::type> and
	// Same <begin_ret_type <T>, typename T::iterator_type>;

export template <typename... T>
struct get_range_traits_t;

export template <typename T>
requires RangeTraits <typename get_range_traits_t <T>::result>
using get_range_traits = typename get_range_traits_t <T>::result;

export template <typename T>
concept HasDefinedRangeTraits = RangeTraits <get_range_traits <T>>;

export template <HasDefinedRangeTraits T>
struct get_iterator_traits_t <T> : get_iterator_traits_t <typename get_range_traits <T>::iterator_type> {};

export template <typename... T>
struct range_traits_t;

export template <typename T>
requires RangeTraits <range_traits_t <T>>
struct get_range_traits_t <T>
{
	using result = range_traits_t <T>;
};

export template <HasDefinedRangePolicies T>
struct range_traits_t <T>
{
	using range_type = fun_param_type <decltype (get_range_policies <T>::begin), 0>;
	using iterator_type = fun_ret_type <decltype (get_range_policies <T>::begin)>;
};



