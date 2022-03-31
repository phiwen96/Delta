export module Delta.Concepts.Range.Begin;

import Delta.Concepts.Iterator;

export
{
	// template <typename T>
	// struct begin_t
	// {
		
	// };

	
	// template <typename T>
	// template <auto N>
	
	// template <typename T, auto N>
	// struct begin_t <T const [N]>
	// {
	// 	static constexpr auto begin (auto const (t) [N]) noexcept -> Iterator auto 
	// 	{
	// 		return t + N;
	// 	}
	// };

	template <auto N>
	constexpr auto begin (auto (t) [N]) noexcept -> Iterator auto 
	{
		return t;
	}

	template <auto N>
	constexpr auto begin (auto (&t) [N]) noexcept -> Iterator auto 
	{
		return t;
	}

	template <auto N>
	constexpr auto begin (auto const (&t) [N]) noexcept -> Iterator auto 
	{
		return t;
	}

	constexpr auto begin (auto& range) noexcept -> Iterator auto 
	requires requires
	{
		{range.begin()} noexcept -> Iterator;
	}
	{
		return range.begin();
	}

	constexpr auto begin (auto const& range) noexcept -> Iterator auto 
	requires requires
	{
		{range.begin()} noexcept -> Iterator;
	}
	{
		return range.begin();
	}
}