export module Delta.Concepts.Size;

export template <typename T>
concept Size = requires (T t, decltype(alignof(char)) u)
{
	u = t;
};
