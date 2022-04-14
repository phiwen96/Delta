module;
#include <utility>
export module Delta.Concepts.Iterator.Traits;

// import Delta.Concepts.Pointer;
import Delta.Concepts.Iterator.ReadOnly;
import Delta.Concepts.Iterator.WriteOnly;
import Delta.Concepts.Iterator.StepForward;
import Delta.Concepts.Iterator.StepBackward;
import Delta.Concepts.Iterator.Jump;
import Delta.Concepts.Same;

export import Delta.Concepts.Iterator.Traits.Sentinel;

enum struct iterator_tag 
{
	INPUT, OUTPUT, FORWARD, RANDOM_ACCESS, CONTIGUOUS
};

export template <typename T>
concept IteratorTraits = requires()
{
	requires Same <decltype (T::tag), iterator_tag>;
	typename T::type;
	typename T::element_type;
	// typename T::sentinel_traits;
};

export template <typename... T>
struct iterator_traits_t;

export template <typename T>
requires (ReadOnly <T> and StepForward <T>)
struct iterator_traits_t <T>
{
	constexpr static auto tag = iterator_tag::INPUT;
	using type = T;
	using element_type = decltype (*std::declval <T> ());//typename pointer_traits_t <T>::element_type;
};

export template <typename T>
requires (WriteOnly <T> and StepForward <T>)
struct iterator_traits_t <T>
{
	constexpr static auto tag = iterator_tag::OUTPUT;
	using type = T;
	using element_type = decltype (*std::declval <T> ());//typename pointer_traits_t <T>::element_type;
};

export template <typename T>
requires (ReadOnly <T> and WriteOnly <T> and StepForward <T>)
struct iterator_traits_t <T>
{
	constexpr static auto tag = iterator_tag::FORWARD;
	using type = T;
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
	using type = T;
	using element_type = decltype (*std::declval <T> ());//typename pointer_traits_t <T>::element_type;
};

// export template <typename iteratorType, typename elementType>
// struct iterator_traits_t <iteratorType, elementType>
// {
// 	using iterator_type = iteratorType;
// 	using element_type = elementType;
// };


// export template <Pointer T>
// struct iterator_traits_t <T> : iterator_traits_t <typename pointer_traits_t <T>::iterator_type, typename pointer_traits_t <T>::element_type> {};




// export template <typename T>
// struct iterator_traits_t <T *>
// {
// 	using element_type = T;
// 	using sentinel = sentinel_traits<T *>;
// };

// export template <typename T>
// struct iterator_traits_t <T const *> //: sentinel_traits <T const*>
// {
// 	using element_type = T;
// 	using sentinel = sentinel_traits<T const *>;
// };

// export template <typename T>
// struct iterator_traits_t <T const *const> //: sentinel_traits <T const * const>
// {
// 	using sentinel = sentinel_traits<T const *const>;
// 	using element_type = T;
// };

