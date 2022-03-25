export module Delta.Concepts.String;

import Delta.Concepts.Char;
import Delta.Concepts.Array;
import Delta.Concepts.Size;
import Delta.Concepts.Pointer;


export template <typename T> 
constexpr auto len (T const& s) noexcept -> Size auto 
requires requires {{s [1]} -> Char;}
{
	auto i = 0;
	while (s [i++] != '\0')
	{
			
	}
	
	return i;
}

export template <typename T>
concept String = requires (T t) 
{
	{t [1]} -> Char;
	{len (t)} -> Size;
};