export module Delta.Concepts.String;

import Delta.Concepts.Char;
import Delta.Concepts.Range;

export template <typename T>
concept String = Range <T> and requires (T t)
{
	{*t}
};

