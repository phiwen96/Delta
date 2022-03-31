module;
#include <stdlib.h>
export module Delta.Async.Out;

import Delta.Async.Write;
import Delta.Concepts.String;

export namespace async
{
	struct out_t 
	{
		constexpr auto operator << (Range auto const& str) -> out_t&
		{
			printf ("hej");
			// std::cout << ":" << str;
			async::write (1, str);
			return *this;
		}
	};

	auto out = out_t {};
}