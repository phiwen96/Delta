export module Delta.Concepts.Iterator.Bidirectional;

import Delta.Concepts.Iterator.ReadOnly;
import Delta.Concepts.Iterator.WriteOnly;
import Delta.Concepts.Iterator.StepForward;
import Delta.Concepts.Iterator.StepBackward;

export template <typename T>
concept BidirectionalIterator =  ReadOnly <T> and WriteOnly <T> and StepForward <T> and StepBackward <T>;