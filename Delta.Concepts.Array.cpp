export module Delta.Concepts.Array;

import Delta.Concepts.Size;
import Delta.Types;
export import Delta.Concepts.Range;
export import Delta.Concepts.Array.Policies;
export import Delta.Concepts.Array.Traits;


export template <typename T>
concept Array = HasDefinedArrayPolicies <T>
// HasDefinedArrayTraits <T>;

export template <typename T, auto N>
using array_types = typelist <T [N], T (&) [N], T const (&) [N]>;


static_assert (Range <int [10]>);
// static_assert (Range <int [10]>);
// static_assert (AllOf <[] <typename T> {return Array <T>;}, array_types <int, 10>>);
// static_assert (AllOf <[] <typename T> {return Range <T>;}, array_types <int, 10>>);
// static_assert (ContiguousRange <T>);