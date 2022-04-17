export module Delta.Concepts.Pointer;
import Delta.Types;
export import :Traits;

export template <typename T>
concept Pointer = HasDefinedPointerTraits <T>;

export template <typename T>
using pointer_types = typelist <T*, T*&, T*&&, T const*, T const*&, T *const, T const * const>;

static_assert (AllOf <[] <typename T> {return Pointer <T>;}, pointer_types <int>>);