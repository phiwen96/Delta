export module Delta.String;

import Delta.Char;
export import Delta.Array;
import Delta.Size;
import Delta.Range;




export 
{
	template <Dynamic_array T>
	requires (not Static_array <T>) and Char <element_type <T>>
	inline auto length (T const& s) noexcept -> Size auto
	{
		auto i = 0;
		while (s [i++] != '\0')
		{
			
		}
	
		return i;
	}

	template <typename T>
	concept String = requires (T t) 
	{
		{t [1]} -> Char;
		{length (t)} -> Size;

	};

	


	
}
