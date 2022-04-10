export module Delta.Concepts.Range;

import Delta.Concepts.Iterator;
import Delta.Concepts.Range.Traits;

export template <typename T>
concept Range = RangeTraits <range_traits_t <T>>;

export template <Range T>
constexpr auto begin (T& range) noexcept -> Iterator auto;

export template <Range T>
constexpr auto end (T& range) noexcept -> Iterator auto;