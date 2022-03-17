export module Delta.String;

import Delta.Char;
import Delta.Array;
import Delta.Size;


export template <Dynamic_array T> requires (not Static_array <T>) and Char <element_type <T>>
auto len (T const& s) noexcept -> Size auto
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