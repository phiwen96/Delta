export module Delta.Concepts.Iterator.StepBackward;

export template <typename T>
concept StepBackward = requires (T t)
{
	--t;
	t--;
};