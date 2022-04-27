export module Delta.String;

export import Delta.Array;



// static_assert (Char <element_type <char const*>>);

export template <Iterator T>
requires Char <element_type <T>>
struct sentinel_value_t <T>
{
	static constexpr element_type <T> value = '\0';
};

export template <typename T>
concept String = Range<T> and Char<defer<fun_ret_type<decltype (range_policies_t<T>::begin)>>>;
