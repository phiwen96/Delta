export module Delta.Concepts.Iterator;

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
