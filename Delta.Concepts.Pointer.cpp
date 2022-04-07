export module Delta.Concepts.Pointer;

import Delta.Concepts.Pointer.Traits;

export template <typename T>
concept Pointer = PointerTraits <pointer_traits <T>>;


// template <typename T, template <typename> typename U>
// constexpr auto for_every_pointer_value = 
// 	U <T*> and 
// 	U <T*&> and
// 	U <T *&&> and
// 	U <T const*> and 
// 	U <T const * &> and 
// 	U <T * const> and 
// 	U <const * const>;
	
export template <Pointer T>
using element_type = typename pointer_traits <T>::element_type;


// static_assert (for_every_pointer_value <int, >);