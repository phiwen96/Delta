export module Delta.Concepts.Range.Policies;

import Delta.Concepts.Iterator;

// export template <typename T>
// concept RangePolicies = requires ()
// {
// 	typename T::element_type;
// };
 
export template <typename T>
requires requires (T t)
{
	{t.begin()} noexcept -> Iterator;
	{t.end()} noexcept -> Iterator;
}
struct range_policies
{
	static constexpr auto begin (T t) noexcept -> Iterator auto 
	{
		return t.begin();
	}

	static constexpr auto end (T t) noexcept -> Iterator auto 
	{
		return t.end();
	}
};


