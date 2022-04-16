// module;
// #include <utility>
export module Delta.Concepts.Iterator;

// import Delta.Mimic;
// import Delta.Types;
// import Delta.Concepts.Convertible;
// export import Delta.Concepts.Pointer;
import Delta.Concepts.Char;
export import Delta.Concepts.Iterator.Traits;
// export import Delta.Concepts.Iterator.Sentinel;
// export import Delta.Concepts.Iterator.Input;
// export import Delta.Concepts.Iterator.Output;
// export import Delta.Concepts.Iterator.Forward;
// export import Delta.Concepts.Iterator.Bidirectional;
// export import Delta.Concepts.Iterator.RandomAccess;
// export import Delta.Concepts.Iterator.Contiguous;

export template <typename T>
concept Iterator = HasDefinedIteratorTraits <T>;

export template <typename T>
requires requires {typename iterator_type_t <T>::type;}
using iterator_type = typename iterator_type_t <T>::type;

export template <typename T>
concept SentinelValue = Iterator <T> and HasDefinedSentinelValueTraits <T>;

export template <typename T>
concept Sentinel = SentinelValue <T>;
	


	

// template <typename T>
// concept HasValue = Sentinel <T> and sentinel_traits<T>::has_sentinel and requires {sentinel_traits<T>::value;};

export template <SentinelValue T>
constexpr auto sentinel_value = sentinel_value_traits_t <T>::value;

// export template <typename T>
// requires (Iterator <T> and Char <element_type <>>)
// struct sentinel_value_traits_t <T>
// {
// 	constexpr static element_type <T> value = '\0';
// };

// export template <typename T>
// struct sentinel_value_t;


// export template <typename T>
// constexpr auto sentinel_value = sentinel_value_t <T>::value;

// export template <typename T>
// concept IteratorWithSentinelValue = Iterator <T> and requires (T t)
// {
// 	sentinel_value <T>;  
// 	// sentinel_value <T>;
// };


// static_assert (Iterator <char const*>);

// export template <Iterator T>
// using element_type = decltype (*std::declval <T> ());// decltype (*mimic <T> ());

static_assert (Iterator <int*>);
// static_assert (AllOf <[] <typename T> {return Iterator <T>;}, pointer_types <int>>);