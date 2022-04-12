export module Delta.Concepts.Array;

import Delta.Concepts.Size;
import Delta.Types;

export import :Traits;

// export template <typename T>
// concept Array = ArrayTraits <array_traits_t <T>>;
// ArrayTraits <array_traits_t <range_traits_t <T>>>;

export template <typename T, auto N>
using array_types = typelist <T [N], T (&) [N], T const (&) [N]>;

// static_assert (Array <int[10]>);
// static_assert (AllOf <[] <typename T> {return Array <T>;}, array_types <int, 10>>);
