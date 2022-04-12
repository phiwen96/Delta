export module Delta.Concepts.String;

import Delta.Concepts.Char;
import Delta.Concepts.Range;
// import Delta.Concepts.Iterator;
import Delta.Concepts.Array;
import Delta.Types;

export template <typename T>
concept String = Range <T> and Char <typename range_traits_t <T>::element_type>;

// static_assert (AllOf <[] <typename T> {return Array <T>;}, array_types <int, 10>>);