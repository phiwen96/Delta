export module Delta.Concepts.String;


// export import Delta.Concepts.Range;
// import Delta.Concepts.Iterator;

export import Delta.Concepts.Array;
// import Delta.Concepts.Array;

// import Delta.Concepts.Char;
// // import Delta.Types;

// export template <typename T>
// requires (Iterator <T> and Char <typename iterator_traits_t <T>::element_type>)
// struct sentinel_value_traits_t <T>
// {
// 	constexpr static element_type <T> value = '\0';
// };

export template <typename T>
concept String = Range <T> and Char <element_type <T>>;





constexpr auto test_string () noexcept -> bool 
{
	
	String auto s0 = "hej";

	return true;
}

static_assert (test_string ());


// static_assert (AllOf <[] <typename T> {return Array <T>;}, array_types <int, 10>>);