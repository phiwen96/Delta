export module Delta:String;


// export import Delta.Concepts.Range;
// import :Char;
// import :Same;
// import Delta.Types;
// import :Iterator;
// import Delta.Concepts.Range;
// import Delta.Concepts.Array;
// import Delta.Concepts.Array;


// // import Delta.Types;


// export template <typename T>
// requires Same <typename get_iterator_traits <T>::element_type, char>
// struct sentinel_traits_t <T>
// {
// 	constexpr static auto value = '\0';
// };

// export template <typename T>
// concept String = Range <T> and Char <get_element_type <T>>;



// export template <Char T = char, Size auto N = 10>
// using string_types = typelist <array_types <T, N>>;

// static_assert (String <char const*>);


// constexpr auto test_string () noexcept -> bool 
// {
	
// 	String auto s0 = "hej";

// 	return true;
// }

// static_assert (test_string ());


// static_assert (AllOf <[] <typename T> {return String <T>;}, string_types>);