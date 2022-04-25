export module Delta.String;

import Delta.Types;
// import Delta.Same;
import Delta.Char;
import Delta.Iterator;
import Delta.Range;
import Delta.Array;
// import :Range;
// import :Range;
// import :Array;



export template <Iterator T>
requires Char <get_element_type <T>>
struct sentinel_traits_t <T>
{
	static constexpr get_element_type <T> value = '\0';
};

// export template <typename T>
// using get_sentinel_traits_t <>

export template <typename T>
concept String = Range <T> and Char <get_element_type <T>>;
static_assert (Array <char[10]>);
// static_assert (Array <char [10]>);
// static_assert (String <char const*>);

// export template <Char T = char, Size auto N = 10>
// using string_types = typelist <array_types <T, N>>;

// static_assert (Iterator <char const*>);
// static_assert (HasDefinedSentinelTraits <char const*>);
// static_assert (SentinelTraits <sentinel_traits_t <char const*>>);
// static_assert (Sentinel <char const*>);

// static_assert (AllOf <[]<typename T>{return String <T>;}, string_types>);

// static_assert (String <char const*>);


// constexpr auto test_string () noexcept -> bool 
// {
	
// 	String auto s0 = "hej";

// 	return true;
// }

// static_assert (test_string ());


// static_assert (AllOf <[] <typename T> {return String <T>;}, string_types>);