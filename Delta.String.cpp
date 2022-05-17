export module Delta.String;

export import Delta.Array;


// uncomment
static_assert (Char <element_type <char const*>>);

export template <Char T>
struct sentinel_value_t <T>
{
	static constexpr T value = '\0';
};

export template <typename T>
concept String = Range<T> and Char<defer<fun_ret_type<decltype (range_policies_t<T>::begin)>>>;

using string_array_types = product_type <array_types, char_types>;
using string_pointer_types = product_type <pointer_types, char_types>;
export using string_types = typelist <string_array_types>;

// static_assert (AllOf <[] <typename T>{return String <T>;}, array_types <char>>);
// static_assert (Range <char[10]>);
// static_assert (String <char[10]>);
// static_assert (Range <char const*>);
// static_assert (Range <char *>);
// static_assert (String <char const*>);
// static_assert (String <char *>);

