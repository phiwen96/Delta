export module Delta.Concepts.Iterator.Output;

import Delta.Concepts.Iterator.WriteOnly;
import Delta.Concepts.Iterator.StepForward;

export template <typename T>
concept OutputIterator =  WriteOnly <T> and StepForward <T>;