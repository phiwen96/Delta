export module Delta.Concepts.String;

export import Delta.Concepts.Char;
export import Delta.Concepts.Range;
export import Delta.Concepts.String.End;

// template <typename T>
// using element_type = int;




export template <typename T>
concept String = Range <T> and Char <element_type <T>>;

static_assert (String <char const [10]>);
// static_assert (ContiguousRange <char const [10]>);





// char const (&t) [N]

