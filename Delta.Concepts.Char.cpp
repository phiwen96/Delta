export module Delta.Concepts.Char;

import Delta.Concepts.Convertible;

export 
{
	template <typename T>
	concept Char = 
		Convertible <T, char> or 
		Convertible <T, signed char> or 
		Convertible <T, unsigned char> or 
		Convertible <T, char16_t> or 
		Convertible <T, char32_t> or 
		Convertible <T, wchar_t>; 
}



