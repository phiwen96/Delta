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
 		constexpr auto operator << (Range auto const& str) -> auto&
		{
			// printf ("length: %ld\n", end (str) - begin (str));
			// printf ("tjo");
			// std::cout << ":" << str;
			async::write (1, str);
			return *this;
		}
	};

	// auto out = out_t {};
}