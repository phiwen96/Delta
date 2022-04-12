export module Delta.Concepts.Range.Policies;

import Delta.Concepts.Function;

import Delta.Concepts.Iterator;

export template <typename T>
concept RangePolicies = requires ()
{
	true;
	// {T::begin (t)} noexcept -> Iterator;
	// {T::end (t)} noexcept -> Iterator;
};

// export template <typename T>
// requires RangePolicies <typename T::range_policies>
// struct get_range_policies_t <T>
// {

// };
 
export template <typename... T>
struct range_policies_t;

template <typename T>
requires RangePolicies <range_policies_t <T>>
constexpr auto begin (T range) noexcept -> Iterator auto 
{
	return range_policies_t <T>::begin (range);
}

template <typename T>
requires RangePolicies <range_policies_t <T>>
constexpr auto end (T range) noexcept -> Iterator auto 
{
	return range_policies_t <T>::end (range);
}
