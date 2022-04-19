export module Delta.Concepts.Range;

export import Delta.Concepts.Range.Policies;

export template <typename T>
concept Range = HasDefinedRangePolicies <T>;// and HasDefinedRangeTraits <T>;


// export template <typename...>
// struct get_range_type_t;

// export template <HasDefinedRangeTraits T>
// struct get_range_type_t <T>
// {
// 	using result = typename get_range_traits <T>::range_type;
// };

// export template <typename T>
// requires Range <typename get_range_type_t <T>::result>
// using get_range_type = typename get_range_type_t <T>::result;
