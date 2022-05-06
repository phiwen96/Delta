export module Delta;

export import Delta.String;

// static_assert (Array <char[10]>);
// static_assert (Range <char[10]>);
// static_assert (AllOf <>);

export template <Sentinel T>
requires(not Array<T>) 
struct range_policies_t<T>
{
	constexpr static auto begin(T t) noexcept -> Iterator auto
	{
		return t;
	}

	constexpr static auto end(T t) noexcept -> Iterator auto
	{
		auto i = t;

		while (*i != sentinel_value_t<T>::value)
		{
			++i;
		}

		return i;
	}
};
static_assert (not Char <int>);
// generates weird bugs
// static_assert (AllOf <[]<typename T>{return Range <T>;}, array_types <int>>);
// static_assert (not Sentinel <int[10]>);
// static_assert (Sentinel <int[10]>);
// static_assert (Iterator <int[10]>);
// static_assert (Range <int[10]>);
// static_assert (AllOf <[]<typename T> {return String <T>;}, string_types>);
// static_assert (AllOf <[]<typename T>{return Sentinel <T>;}, product_type <non_pointer_const_types, char_types>>);


// Char<defer<fun_ret_type<decltype (range_policies_t<T>::begin)>>>
// static_assert (AllOf <[]<typename T>{return Bounded <T>;}, array_types <int>>);

// static_assert (Same <range_type <int[10]>, int>);

static_assert (Array <int[10]>);
static_assert (Same <fun_param_type<decltype(array_policies_t <int(&)[10]>::begin), 0>, int(&)[10]>);
static_assert (Same <fun_param_type<decltype(array_policies_t <int const(&)[10]>::begin), 0>, int const(&)[10]>);
static_assert (Same <fun_param_type<decltype(array_policies_t <int[10]>::begin), 0>, int(&)[10]>);
// static_assert (Same <range_type <int[10]>, int>);
// static_assert (Range <int[10]>);
static_assert (AllOf <[]<typename T>{return Array <T>;}, array_types <int>>);
static_assert (AllOf <[]<typename T>{return Array <T>;}, array_types <char>>);
static_assert (AllOf <[]<typename T> {return Array <T>;}, array_types <char_types>>);

static_assert (Range <int(&)[10]>);
static_assert (Range <int const(&)[10]>);
static_assert (not Iterator <int(&)[10]>);
static_assert (not Iterator <int const(&)[10]>);
// static_assert (not Iterator <int[10]>);
static_assert (Same <element_type <int[10]>, int>);
static_assert (Same <element_type <int(&)[10]>, int>);
// export using bajw = element_type <int[10]>;
// static_assert (Range <int[10]>);
// static_assert (AllOf <[]<typename T> {return Bounded <T>;}, array_types <char_types>>);

// static_assert (Same <array_types <>>)
// static_assert (AllOf <[]<typename T>{return Range <T>;}, array_types <char, 2>>);

// T *, T *&, T *&&, T const *, T const *&, T *const, T *const&, T const *const
// static_assert (String <char*>);

// static_assert (Sentinel <char* const>);
// static_assert (Sentinel <char* const>&);
// static_assert (Sentinel <char const * const>);
// static_assert (AllOf <[]<typename T>{return Sentinel <T>;}, product_type <pointer_types, char_types>>);
// static_assert (not Array <char*>);
