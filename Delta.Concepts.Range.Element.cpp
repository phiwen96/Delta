export module Delta.Concepts.Range.Element;

import Delta.Concepts.Same;

export template <typename T>
struct element_traits_t;

export template <typename T>
concept ElementTraits = requires ()
{
	typename T::type;
};

export template <typename T>
requires ElementTraits <element_traits_t <T>>
using element_type = typename element_traits_t <T>::type;

export template <typename T, typename U>
concept Element = ElementTraits <element_traits_t <T>> and Same <element_type <T>, U>;


