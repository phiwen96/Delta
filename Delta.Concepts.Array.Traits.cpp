export module Delta.Concepts.Array.Traits;
// import Delta.Concepts.Function;
// import Delta.Concepts.Size;
// import Delta.Concepts.Range;
// import Delta.Concepts.Array.Policies;


// template <typename T>
// concept ArrayTraits = RangeTraits <T>;

// export template <typename... T>
// struct get_array_traits_t;

// export template <typename T>
// // requires ArrayTraits <typename get_array_traits_t <T>::result>
// using get_array_traits = typename get_array_traits_t <T>::result;

// export template <typename T>
// concept HasDefinedArrayTraits = ArrayTraits <get_array_traits <T>>;

// export template <typename... T>
// struct array_traits_t;

// export template <typename T>
// requires ArrayTraits <array_traits_t <T>>
// struct get_array_traits_t <T>
// {
// 	using result = array_traits_t <T>;
// };

// export template <typename T>
// requires (RangeTraits <array_traits_t <T>> and not RangePolicies <get_range_policies <T>>)
// struct get_range_traits_t <T>
// {
// 	using result = array_traits_t <T>;
// };

// export template <HasDefinedArrayPolicies T>
// struct array_traits_t <T> 
// {
// 	using range_type = fun_param_type <decltype (get_array_policies <T>::begin), 0>;
// 	using iterator_traits = get_iterator_traits <fun_ret_type <decltype (get_array_policies <T>::begin)>>;
// };

// // export template <HasDefinedArrayTraits T>
// // struct get_range_traits_t <T> 
// // {
// // 	using result = get__traits <T>;
// // };