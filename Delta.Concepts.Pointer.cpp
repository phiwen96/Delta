export module Delta.Concepts.Pointer;
import Delta.Types;
export import :Traits;

export template <typename T>
concept Pointer = HasDefinedPointerTraits <T>;

// export template <Pointer T>
// using element_type = typename pointer_traits_t <T>::element_type;


export template <typename T>
requires requires {typename element_type_t <T>::type;}
using element_type = typename element_type_t <T>::type;


export template <typename T>
using pointer_types = typelist <T*, T*&, T*&&, T const*, T const*&, T *const, T const * const>;

static_assert (AllOf <[] <typename T> {return Pointer <T>;}, pointer_types <int>>);