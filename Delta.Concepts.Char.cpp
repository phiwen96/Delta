export module Delta.Concepts.Char;

import Delta.Concepts.Convertible;
import Delta.Types;

using char_types = typelist <char, signed char, unsigned char, char16_t, char32_t, wchar_t>;

// template <typename List, typename Element>
// concept AnyOf = 




// template <typename T>
// constexpr auto is_char = 







// template <typename...>
// struct bb{};
// static_assert (Bajs <bb>);



export template <typename T>
concept Char = AnyOf <[] <typename U> {return Convertible <T, U>;}, char_types>;
	// Convertible <T, char> or 
	// Convertible <T, signed char> or 
	// Convertible <T, unsigned char> or 
	// Convertible <T, char16_t> or 
	// Convertible <T, char32_t> or 
	// Convertible <T, wchar_t>; 

	/*
	concept Char = char_types | any_of (<Char> {});
	*/

static_assert (AllOf <[] <typename T> {return Char <T>;}, char_types>);




