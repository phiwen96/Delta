export module Delta.String;

import Delta.Types;
import Delta.Same;
import Delta.Char;
import Delta.Size;
import Delta.Iterator;
import Delta.Range;
import Delta.Array;
// import :Range;
// import :Range;
// import :Array;



 template <Iterator T>
requires Char <get_element_type <T>>
struct sentinel_traits_t <T>
{
	constexpr static get_element_type <T> value = '\0';
};

export template <typename T>
concept String = Range <T> and Char <get_element_type <T>>;



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