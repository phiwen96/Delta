export module Delta.Concepts.Iterator.Traits.Sentinel;

import Delta.Concepts.Bool;


export template <typename T>
concept SentinelValueTraits = requires
{	
	T::value;
};

export template <typename...>
struct sentinel_value_traits_t;


// import Delta.Types;



export template <typename T>
concept HasDefinedSentinelValueTraits = SentinelValueTraits <sentinel_value_traits_t <T>>;

