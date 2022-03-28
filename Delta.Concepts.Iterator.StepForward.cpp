export module Delta.Concepts.Iterator.StepForward;

export template <typename T>
concept StepForward = requires (T t)
{
	++t;
	t++;
};