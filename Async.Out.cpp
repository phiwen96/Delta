module;

export module Delta.Async.Out;

import Delta.Async.Write;

export namespace async
{
	struct out_t 
	{
		auto operator << (char const* str) -> out_t&
		{
			async::write (1, str);
			return *this;
		}
	};

	auto out = out_t {};
}