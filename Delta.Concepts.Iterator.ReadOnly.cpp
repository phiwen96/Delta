export module Delta.Concepts.Iterator.ReadOnly;

export template <typename T>
concept ReadOnly = requires (T t)
{
	*t;
};