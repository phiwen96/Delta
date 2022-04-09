export module Delta.Concepts.Range;

export import Delta.Concepts.Iterator;
export import Delta.Concepts.Range.Traits;

export template <typename T>
concept Range = RangeTraits <range_traits <T>>;

export template <Range T>
constexpr auto begin (T& range) noexcept -> Iterator auto;


export template <Range T>
constexpr auto end (T& range) noexcept -> Iterator auto;



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
