export module Delta.Concepts.Iterator.RandomAccess;

import Delta.Concepts.Iterator.ReadOnly;
import Delta.Concepts.Iterator.WriteOnly;
import Delta.Concepts.Iterator.StepForward;
import Delta.Concepts.Iterator.StepBackward;
import Delta.Concepts.Iterator.Jump;

export template <typename T>
concept RandomAccessIterator =  
	ReadOnly <T> and
	WriteOnly <T> and 
	StepForward <T> and 
	StepBackward <T> and 
	Jump <T>;