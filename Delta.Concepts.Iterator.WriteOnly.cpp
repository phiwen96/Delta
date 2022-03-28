export module Delta.Concepts.Iterator.WriteOnly;

export template <typename T>
concept WriteOnly = requires (T t, decltype (*t) u)
{
	*t = u;
};