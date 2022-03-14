export module Delta.Char;

import Delta.Convertible_to;

export 
{
	template <typename T>
	concept Char = Convertible_to <T, char>
		or Convertible_to <T, signed char>
		or Convertible_to <T, unsigned char>
		or Convertible_to <T, char16_t>
		or Convertible_to <T, char32_t>
		or Convertible_to <T, wchar_t>;
}




¯