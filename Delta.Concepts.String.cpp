export module Delta.Concepts.String;

export import Delta.Concepts.Char;
export import Delta.Concepts.Range;
export import Delta.Concepts.String.Begin;
export import Delta.Concepts.String.End;

// template <typename T>
// using element_type = int;




export template <typename T>
concept String = Range <T> and Char <element_type <T>>;
// requires (T& t)
// {
// 	{begin (t)} -> Iterator;
// 	{end (t)} -> Iterator;
// };


// static_assert (ContiguousRange <char const [10]>);





// char const (&t) [N]

export template <Char T>
struct range_traits <T const*>
{
	using element_type = T;

	static constexpr auto begin (T const* t) noexcept -> Iterator auto 
	{
		return t;
	}

	static constexpr auto end (T const* t) noexcept -> Iterator auto 
	{
		T const* i = t;

		while (*i != '\0')
		{
			++i;
		}
		
		return i;
	}
};



static_assert (String <char const *>);