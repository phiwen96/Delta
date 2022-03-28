export module Delta.Concepts.Iterator.Forward;

import Delta.Concepts.Iterator.ReadOnly;
import Delta.Concepts.Iterator.WriteOnly;
import Delta.Concepts.Iterator.StepForward;

export template <typename T>
concept ForwardIterator =  
	ReadOnly <T> and 
	WriteOnly <T> and 
	StepForward <T>;