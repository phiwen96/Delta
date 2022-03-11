export module Delta.String;

import Delta.Char;
import Delta.Array;
import Delta.Size;
import Delta.Range;




export 
{
	template <typename T>
	concept String = requires (T t) 
	{
		{t [1]} -> Char;
		{length (t)} -> Size;

	};

	template <Dynamic_array T>
	requires Char <element_type <T>>
	auto length (T const& s) noexcept -> auto
	{
		auto i = 0;
		while (s [i++] != '\0')
		{
			
		}
	
		return i;
	}

	
}
