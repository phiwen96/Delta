module;
#include <stdio.h>
// #include <iostream>
export module Delta.Async.Out;

import Delta.Async.Write;
import Delta.Concepts.String;


export namespace async
{
	struct out_t 
	{
		constexpr out_t () noexcept 
		{
			
		}

		template <Range T>
 		constexpr auto operator << (T && str) -> auto&
		{
			// printf (str);
			async::write (1, (T&&) str);
			return *this;
		}
	};

	// auto out = out_t {};
}