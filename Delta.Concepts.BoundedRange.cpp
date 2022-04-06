export module Delta.Concepts.BoundedRange;

// import Delta.Concepts.Size;
// import Delta.Concepts.Range;

// export template <typename T>
// concept BoundedRange = Range <T> and range_traits <T>::bounded;

// export template <typename T, auto N>
// struct range_traits <T [N]>
// {
// 	using element_type = T;
// 	static constexpr auto bounded = true;
// 	static constexpr auto length = N;
// };

// export template <typename T, auto N>
// struct range_policies <T [N]>
// {
// 	static constexpr auto begin (T (range) [N]) noexcept -> Iterator auto 
// 	{
// 		return range;
// 	}

// 	static constexpr auto end (T (range) [N]) noexcept -> Iterator auto 
// 	{
// 		return range + N;
// 	}
// };

// export template <typename T, auto N>
// struct range_traits <T const [N]>
// {
// 	using element_type = T;
// 	static constexpr auto bounded = true;
// 	static constexpr auto length = N;
// };

// export template <typename T, auto N>
// struct range_traits <T const (&) [N]>
// {
// 	using element_type = T;
// 	static constexpr auto bounded = true;
// 	static constexpr auto length = N;
// };