export module Delta.Concepts.String;

import Delta.Concepts.Char;
import Delta.Concepts.Range;
// import Delta.Concepts.Iterator;
import Delta.Concepts.Array;
import Delta.Types;



export template <typename T>
concept String = Range <T> and Char <element_type <T>>;

// export template <Iterator T>



constexpr auto test_string () noexcept -> bool 
{
	
	// String auto s0 = "hej";

	return true;
}

static_assert (test_string ());


// static_assert (AllOf <[] <typename T> {return Array <T>;}, array_types <int, 10>>);