export module Delta.Concepts.String;

import Delta.Concepts.Char;
import Delta.Concepts.Range;

// template <typename T>
// using element_type = int;

export import Delta.Concepts.String.End;


export template <typename T>
concept String = Range <T> and Char <element_type <T>>;





// char const (&t) [N]

