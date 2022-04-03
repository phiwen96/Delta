export module Delta.Concepts.Range.Traits;

import Delta.Concepts.Iterator;

export template <typename T>
struct range_traits 
{
	static constexpr auto begin (T& t) noexcept -> Iterator auto 
	requires requires
	{
		{t.begin ()} noexcept -> Iterator;
	}
	{
		return t.begin ();
	}

	static constexpr auto begin (T const& t) noexcept -> Iterator auto 
	requires requires
	{
		{t.begin ()} noexcept -> Iterator;
	}
	{
		return t.begin ();
	}

	static constexpr auto end (T& t) noexcept -> Iterator auto 
	requires requires
	{
		{t.end ()} noexcept -> Iterator;
	}
	{
		return t.end ();
	}

	static constexpr auto end (T const& t) noexcept -> Iterator auto 
	requires requires
	{
		{t.end ()} noexcept -> Iterator;
	}
	{
		return t.end ();
	}
};

export template <typename T>
requires requires {typename range_traits <T>::element_type;}
using element_type = typename range_traits <T>::element_type;


export template <typename T, auto N>
struct range_traits <T [N]>
{	
	using element_type = T;

	static constexpr auto begin (T (t) [N]) noexcept -> Iterator auto 
	{
		return t;
	}

	static constexpr auto end (T (t) [N]) noexcept -> Iterator auto 
	{
		return t;
	}

	static constexpr auto begin (T const (&t) [N]) noexcept -> Iterator auto 
	{
		return t;
	}

	static constexpr auto end (T const (&t) [N]) noexcept -> Iterator auto 
	{
		return t;
	}
};


