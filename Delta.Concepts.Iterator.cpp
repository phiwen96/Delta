export module Delta.Concepts.Iterator;

// import Delta.Concepts.Char;
export import Delta.Concepts.Iterator.Traits;


export template <typename T>
concept Iterator = HasDefinedIteratorTraits <T>;

export template <typename T>
concept SentinelValue = Iterator <T> and HasDefinedSentinelValueTraits <T>;

export template <typename T>
concept Sentinel = SentinelValue <T>;
	
export template <SentinelValue T>
constexpr auto sentinel_value = sentinel_value_traits_t <T>::value;


// static_assert (Iterator <char const*>);

// export template <Iterator T>
// using element_type = decltype (*std::declval <T> ());// decltype (*mimic <T> ());

// static_assert (Iterator <int*>);
// static_assert (AllOf <[] <typename T> {return Iterator <T>;}, pointer_types <int>>);