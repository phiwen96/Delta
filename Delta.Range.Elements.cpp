export module Delta.Range.Elements;

import Delta.Concepts.Convertible;
import Delta.Concepts.Iterator;

export template <typename T>
struct type_inspector
{
	constexpr static auto has_elements = false;
};

export template <typename T>
constexpr auto has_elements = type_inspector <T>::has_elements;

export template <typename T>
requires has_elements <T>
using elements = type_inspector <T>::element_type; 

export template <typename T, typename U>
concept Elements = has_elements <T> and Convertible <T, U>; 

















template <typename T>
requires requires (T t)
{
	{t.begin ()} -> Iterator;
}
struct type_inspector <T>
{
	constexpr static auto has_elements = true;
	// using element_type =  t.begin ()
};