export module Delta.Concepts.Range;
export import Delta.Concepts.Range.Element;
export import Delta.Concepts.Iterator;
export import Delta.Concepts.Range.Traits;
export import Delta.Concepts.Range.Policies;
export import Delta.Concepts.Size;


export template <typename T>
concept Range = RangePolicies <range_policies_t <T>>;//RangeTraits <range_traits_t <T>>;

export template <Range T>
constexpr auto begin (T range) noexcept -> Iterator auto;

export template <Range T>
constexpr auto end (T range) noexcept -> Iterator auto;

