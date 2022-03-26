export module Delta.Concepts.Iterator.Input;

import Delta.Concepts.Iterator.ReadOnly;
import Delta.Concepts.Iterator.StepForward;

export template <typename T>
concept InputIterator =  
	ReadOnly <T> and 
	StepForward <T>;