export module Delta.Concepts.Pointer;
export import Delta.Types;
export import Delta.Concepts.Pointer.Traits;

export template <typename T>
concept Pointer = HasDefinedPointerTraits <T>;

export template <typename T>
using pointer_types = typelist <T*, T*&, T*&&, T const*, T const*&, T *const, T const * const>;

static_assert (AllOf <[] <typename T> {return Pointer <T>;}, pointer_types <int>>);