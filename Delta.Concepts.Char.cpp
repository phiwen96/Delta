export module Delta.Concepts.Char;

import Delta.Concepts.Convertible;

export template <template <typename...> typename T>
using char_types = T <char, signed char, unsigned char, char16_t, char32_t, wchar_t>;

// template <typename List, typename Element>
// concept AnyOf = 




// template <typename T>
// constexpr auto is_char = 







// template <typename...>
// struct bb{};
// static_assert (Bajs <bb>);


export template <typename T>
concept Char = 
	Convertible <T, char> or 
	Convertible <T, signed char> or 
	Convertible <T, unsigned char> or 
	Convertible <T, char16_t> or 
	Convertible <T, char32_t> or 
	Convertible <T, wchar_t>; 

	/*
	concept Char = char_types | any_of (<Char> {});
	*/




