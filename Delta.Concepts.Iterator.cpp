module;
#include <utility>
export module Delta.Concepts.Iterator;

// import Delta.Concepts.Char;
// export import Delta.Concepts.Iterator.Traits;
import Delta.Concepts.Size;

import Delta.Concepts.Convertible;

import Delta.Concepts.Same;

// export import Delta.Concepts.Iterator.Traits.Sentinel;



export 
{
	struct iterator_tag 
	{
		struct INPUT {}; struct OUTPUT {}; struct FORWARD {}; struct BIDIRECTIONAL {}; struct RANDOM_ACCESS {}; struct CONTIGUOUS {};
	};
}

export template <typename T>
concept IteratorTraits = requires
{
	// requires Same <decltype (T::tag), iterator_tag>;
	// {T::tag} -> Convertible <iterator_tag>;
	// typename T::iterator_type;
	typename T::element_type;
	typename T::iterator_type;
	// typename T::iterator_tag;
	// typename T::sentinel_traits;
};

export template <typename... T>
struct get_iterator_traits_t;

export template <typename T>
// requires IteratorTraits <typename get_iterator_traits_t <T>::result>
using get_iterator_traits = typename get_iterator_traits_t <T>::result;

export template <typename T>
concept HasDefinedIteratorTraits = IteratorTraits <get_iterator_traits <T>>;

export template <typename... T>
struct iterator_traits_t;

export template <typename A, typename B, typename C>
struct iterator_traits_t <A, B, C>
{
	using iterator_tag = A; 
	using iterator_type = B;
	using element_type = C;
};

export template <typename T>
requires IteratorTraits <iterator_traits_t <T>>
struct get_iterator_traits_t <T>
{
	using result = iterator_traits_t <T>;
};

export template <typename...>
struct get_element_type_t;

export template <typename T>
// requires requires {typename get_element_type_t <T>::element_type;}
using get_element_type = typename get_element_type_t <T>::result;

export template <HasDefinedIteratorTraits T>
struct get_element_type_t <T>
{
	using result = typename get_iterator_traits <T>::element_type;
};

template <typename T>
using defer = decltype (*std::declval <T> ());

template <typename T>
concept ReadOnly = requires (T t)
{
	*t;
};

template <typename T>
concept WriteOnly = requires (T t, decltype (*t) u)
{
	*t = u;
};

template <typename T>
concept StepForward = requires (T t)
{
	++t;
	t++;
};

template <typename T>
concept StepBackward = requires (T t)
{
	--t;
	t--;
};

template <typename T>
concept Jump =  requires (T t)
{
	t + 10;
	t += 10;
	
	t - 10;
	t -= 10;
};

export template <typename T>
requires (ReadOnly <T> and StepForward <T>)
struct iterator_traits_t <T> : iterator_traits_t <iterator_tag::INPUT, T, defer <T>> {};

export template <typename T>
requires (WriteOnly <T> and StepForward <T>)
struct iterator_traits_t <T> : iterator_traits_t <iterator_tag::OUTPUT, T, defer <T>> {};

export template <typename T>
requires (ReadOnly <T> and WriteOnly <T> and StepForward <T>)
struct iterator_traits_t <T> : iterator_traits_t <iterator_tag::FORWARD, T, defer <T>> {};

export template <typename T>
requires (ReadOnly <T> and
	WriteOnly <T> and 
	StepForward <T> and 
	StepBackward <T>)
struct iterator_traits_t <T> : iterator_traits_t <iterator_tag::BIDIRECTIONAL, T, defer <T>> {};

export template <typename T>
requires (ReadOnly <T> and
	WriteOnly <T> and 
	StepForward <T> and 
	StepBackward <T> and 
	Jump <T>)
struct iterator_traits_t <T> : iterator_traits_t <iterator_tag::RANDOM_ACCESS, T, defer <T>> {};


export template <typename T>
concept SentinelTraits = requires
{	
	T::value;
};

export template <typename...>
struct get_sentinel_traits_t;

export template <typename T>
using get_sentinel_traits = typename get_sentinel_traits_t <T>::result;

export template <typename...>
struct sentinel_traits_t;

export template <typename T>
requires SentinelTraits <sentinel_traits_t <T>>
struct get_sentinel_traits_t <T>
{
	using result = sentinel_traits_t <T>;
};


// import Delta.Types;



export template <typename T>
concept HasDefinedSentinelTraits = SentinelTraits <get_sentinel_traits <T>>;




export template <typename T>
concept Iterator = HasDefinedIteratorTraits <T>;

// export template <typename T>
// concept Sentinel = Iterator <T> and HasDefinedSentinelTraits <T>;
	
// export template <Sentinel T>
// constexpr auto sentinel_value = sentinel_traits_t <T>::value;


// static_assert (Iterator <char const*>);

// export template <Iterator T>
// using element_type = decltype (*std::declval <T> ());// decltype (*mimic <T> ());

// static_assert (Iterator <int*>);
// static_assert (AllOf <[] <typename T> {return Iterator <T>;}, pointer_types <int>>);