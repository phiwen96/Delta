module;
#include <utility>
export module Delta.Concepts.Iterator;

import Delta.Mimic;
import Delta.Concepts.Convertible;
export import Delta.Concepts.Iterator.Sentinel;
export import Delta.Concepts.Iterator.Input;
export import Delta.Concepts.Iterator.Output;
export import Delta.Concepts.Iterator.Forward;
export import Delta.Concepts.Iterator.Bidirectional;
export import Delta.Concepts.Iterator.RandomAccess;
export import Delta.Concepts.Iterator.Contiguous;


export template <typename T>
concept Iterator =
	InputIterator<T> or
	OutputIterator<T> or 
	ForwardIterator<T> or 
	BidirectionalIterator<T> or 
	RandomAccessIterator<T> or
	ContiguousIterator<T>;

export template <typename T>
concept IteratorWithSentinel = Iterator <T> and requires (T& t)
{
	{t == sentinel_value <T>} noexcept -> Convertible <bool>;  
	{t != sentinel_value <T>} noexcept -> Convertible <bool>;  
};

export template <Iterator T>
using element_type = decltype (*std::declval <T> ());// decltype (*mimic <T> ());
