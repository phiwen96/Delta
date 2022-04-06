export module Delta.Concepts.String;

export import Delta.Concepts.Char;
export import Delta.Concepts.Array;


export template <Iterator T>
requires Char <element_type <T>>
struct sentinel_value_t <T>
{
	static constexpr auto value = '\0';
};

export template <Iterator T>
requires Char <element_type <T>>
struct sentinel_value_t <T const>
{
	static constexpr auto value = '\0';
};

export template <typename T>
concept String = Range <T> and Char <element_type <T>>;

static_assert (Iterator <char const *>);
static_assert (Char <element_type <char const *>>);
// static_assert (String <char const *>);



// static_assert (ContiguousRange <char const [10]>);





// char const (&t) [N]

// export template <Char T>
// struct range_traits <T const*>
// {
// 	using element_type = T;

// 	static constexpr auto begin (T const* t) noexcept -> Iterator auto 
// 	{
// 		return t;
// 	}

// 	static constexpr auto end (T const* t) noexcept -> Iterator auto 
// 	{
// 		T const* i = t;

// 		while (*i != '\0')
// 		{
// 			++i;
// 		}
		
// 		return i;
// 	}
// };



