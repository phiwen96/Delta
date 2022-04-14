export module Delta.Concepts.Iterator.Traits.Sentinel;

import Delta.Concepts.Bool;

export template <typename T>
concept SentinelValueTraits = requires
{	
	T::value;
};

export template <typename T>
struct sentinel_value_traits_t;

