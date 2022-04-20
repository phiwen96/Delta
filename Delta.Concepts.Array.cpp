export module Delta.Concepts.Array;

// import Delta.Concepts.Size;
// import Delta.Types;
export import Delta.Concepts.Range;

export template <typename T>
concept ArrayPolicies = RangePolicies <T>;

export template <typename... T>
struct get_array_policies_t;

export template <typename T>
// requires ArrayPolicies <typename get_array_policies_t <T>::result>
using get_array_policies = typename get_array_policies_t <T>::result;

export template <typename T>
concept HasDefinedArrayPolicies = ArrayPolicies <get_array_policies <T>>;

export template <typename...>
struct array_policies_t;

export template <typename T>
requires ArrayPolicies <array_policies_t <T>>
struct get_array_policies_t <T>
{
	using result = array_policies_t <T>;
};

export template <HasDefinedArrayPolicies T>
struct get_range_policies_t <T>
{
	using result = get_array_policies <T>;
};

export template <typename T>
concept Array = HasDefinedArrayPolicies <T>;

export template <typename T, auto N>
using array_types = typelist <T [N], T (&) [N], T const (&) [N]>;

export template <typename T, auto N>
struct array_policies_t <T [N]> 
{
	constexpr static auto begin (T (range) [N]) noexcept -> auto*
	{
		return range;
	}

	constexpr static auto length (T (range) [N]) noexcept -> Size auto
	{
		return N;
	}	
};

export template <typename T, auto N>
struct array_policies_t <T (&) [N]> 
{
	constexpr static auto begin (T (&range) [N]) noexcept -> auto*
	{
		return range;
	}

	constexpr static auto length (T (&range) [N]) noexcept -> Size auto
	{
		return N;
	}	
};

static_assert (AllOf <[] <typename T> {return Array <T>;}, array_types <int, 10>>);
static_assert (AllOf <[] <typename T> {return Range <T>;}, array_types <int, 10>>);

consteval auto test_array () noexcept -> bool 
{
	
	return true;
}

