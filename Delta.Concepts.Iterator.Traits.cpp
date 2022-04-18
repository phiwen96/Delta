module;
#include <utility>
export module Delta.Concepts.Iterator.Traits;

export import Delta.Concepts.Pointer;
import Delta.Concepts.Convertible;
import Delta.Concepts.Iterator.ReadOnly;
import Delta.Concepts.Iterator.WriteOnly;
import Delta.Concepts.Iterator.StepForward;
import Delta.Concepts.Iterator.StepBackward;
import Delta.Concepts.Iterator.Jump;
import Delta.Concepts.Same;

export import Delta.Concepts.Iterator.Traits.Sentinel;

export enum struct iterator_tag 
{
	INPUT, OUTPUT, FORWARD, BIDIRECTIONAL, RANDOM_ACCESS, CONTIGUOUS
};

export template <typename T>
concept IteratorTraits = requires()
{
	// requires Same <decltype (T::tag), iterator_tag>;
	{T::tag} -> Convertible <iterator_tag>;
	typename T::iterator_type;
	typename T::element_type;
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
using get_element_type = typename get_element_type_t <T>::element_type;

export template <HasDefinedIteratorTraits T>
struct get_element_type_t <T>
{
	using result = typename get_iterator_traits <T>::element_type;
};

export template <typename T>
requires (ReadOnly <T> and StepForward <T>)
struct iterator_traits_t <T>
{
	constexpr static auto tag = iterator_tag::INPUT;
	using iterator_type = T;
	using element_type = decltype (*std::declval <T> ());//typename pointer_traits_t <T>::element_type;
};

export template <typename T>
requires (WriteOnly <T> and StepForward <T>)
struct iterator_traits_t <T>
{
	constexpr static auto tag = iterator_tag::OUTPUT;
	using iterator_type = T;
	using element_type = decltype (*std::declval <T> ());//typename pointer_traits_t <T>::element_type;
};

export template <typename T>
requires (ReadOnly <T> and WriteOnly <T> and StepForward <T>)
struct iterator_traits_t <T>
{
	constexpr static auto tag = iterator_tag::FORWARD;
	using iterator_type = T;
	using element_type = decltype (*std::declval <T> ());//typename pointer_traits_t <T>::element_type;
};

export template <typename T>
requires (ReadOnly <T> and
	WriteOnly <T> and 
	StepForward <T> and 
	StepBackward <T>)
struct iterator_traits_t <T>
{
	constexpr static auto tag = iterator_tag::BIDIRECTIONAL;
	using iterator_type = T;
	using element_type = decltype (*std::declval <T> ());//typename pointer_traits_t <T>::element_type;
};

export template <typename T>
requires (ReadOnly <T> and
	WriteOnly <T> and 
	StepForward <T> and 
	StepBackward <T> and 
	Jump <T>)
struct iterator_traits_t <T>
{
	constexpr static auto tag = iterator_tag::RANDOM_ACCESS;
	using iterator_type = T;
	using element_type = decltype (*std::declval <T> ());//typename pointer_traits_t <T>::element_type;
};