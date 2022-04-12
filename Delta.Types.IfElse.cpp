export module Delta.Types.IfElse;

template <bool, typename T, typename U>
struct if_else_t;

template <typename T, typename U>
struct if_else_t <true, T, U>
{
	using type = T;
};

template <typename T, typename U>
struct if_else_t <false, T, U>
{
	using type = U;
};

export template <bool B, typename T, typename U>
using if_else = typename if_else_t <B, T, U>::type;

