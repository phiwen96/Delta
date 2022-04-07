export module Delta.Concepts.Iterator.Traits.Sentinel;

import Delta.Concepts.Bool;

export template <typename T>
concept SentinelTraits = requires ()
{
	{T::has_sentinal} -> Bool;
	
	requires not T::has_sentinal or requires 
	{
		T::value;
	};
};

export template <typename T>
struct sentinel_traits
{
	constexpr static auto has_sentinal = false;
};

