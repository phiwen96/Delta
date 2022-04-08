module;
#include <utility>
export module Delta.Concepts.Range;


// import Delta.Concepts.Same;
// import Delta.Concepts.Convertible;
// import Delta.Mimic;
// export import Delta.Concepts.Array;
export import Delta.Concepts.Iterator;
export import Delta.Concepts.Range.Begin;
export import Delta.Concepts.Range.End;
export import Delta.Concepts.Range.Policies;
export import Delta.Concepts.Range.Traits;

// export import Delta.Concepts.Range.Contiguous;

// export template <typename T>
// requires Array <T>
// struct range_traits <T>// : array_traits <T>
// {

// };



export template <typename T, 
	template <typename> typename traits = range_traits, 
	template <typename> typename policies = range_policies>
concept Range = 
	RangeTraits <traits <T>> and 
	RangePolicies <policies <T>>;
// requires (T& t)
// {
// 	{begin (t)} noexcept -> Iterator;
// 	{end (t)} noexcept -> Iterator;
// };

// export template <>

// export template <typename T>
// struct range_traits
// {

// };






// export template <Array T>
// struct range_traits <T> : array_traits <T>
// {

// };

// export template <typename T>
// requires IteratorWithSentinelValue <T*>
// struct range_traits <T*>
// {
// 	using element_type = T;
// };

// export template <typename T>
// requires IteratorWithSentinelValue <T const*>
// struct range_traits <T const*>
// {
// 	using element_type = T;
// };








// static_assert (Range <char [10]>);
// static_assert (Range <char const [10]>);
// static_assert (Range <char const (&) [10]>);
