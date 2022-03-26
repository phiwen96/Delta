export module Delta.Concepts.Iterator.Jump;

export template <typename T>
concept Jump =  requires (T t)
{
	t + 10;
	t += 10;
	
	t - 10;
	t -= 10;
};