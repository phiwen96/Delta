export module Delta.Char;

import Delta.Same_as;

export 
{
	template <typename T>
	concept Char = Same_as <T, char>
		or Same_as <T, signed char>
		or Same_as <T, unsigned char>
		or Same_as <T, char16_t>
		or Same_as <T, char32_t>
		or Same_as <T, wchar_t>;
}