#include <aio.h>
#include <iostream>
import Delta;

// EAT(bajsf)

template <typename...>
struct bajs_t;

template <>
struct bajs_t <int> {};




// template <template <
auto main (int, char**) -> int 
{
	TemplateSpecializationOf <bajs_t> auto b = bajs_t <int> {};
	int i;
	Stripped <int> auto* i2 = &i;
	ContainsType <int> auto i3 = typelist <char, int> {};
	std::cout << "hej" << std::endl;

	// auto ii = contains_type_

	// String auto s = "hej";
	return 0;
}