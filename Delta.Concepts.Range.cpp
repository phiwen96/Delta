export module Delta.Concepts.Range;
export import Delta.Concepts.Range.Element;
export import Delta.Concepts.Iterator;
export import Delta.Concepts.Range.Traits;
export import Delta.Concepts.Range.Policies;
export import Delta.Concepts.Size;


export template <typename T>
concept Range = HasDefinedRangePolicies <T> and HasDefinedRangeTraits <T>;


export template <typename T>
requires Range <typename range_type_t <T>::type>
using range_type = typename range_type_t <T>::type;
