#include <iostream>
#include <utility>
#include <type_traits>
import Delta;

/*
	make a new array type that satisfies concept Array explicitly 
	by defining a template specialization for array_policies_t 
*/

template <typename T, auto N>
struct super_array1 {
	T data [N];
};

template <typename T, auto N>
struct array_policies_t <super_array1 <T, N>> {
	using type = super_array1 <T, N>;
	static auto* begin (type& s) noexcept {return s.data;}
	static auto* end (type& s) noexcept {return s.data + N;}
};

static_assert (Array <super_array1 <int, 10>>);

/*
	make a new array type that satisfies concept Array implicitly 
	by defining the right functions
*/

template <typename T, auto N>
struct super_array2 {
	T data [N];

	constexpr auto* begin () noexcept {return data;}
	constexpr auto* end () noexcept {return data + N;}
};

static_assert (Range <super_array2 <int, 10>>);
// static_assert (Array <super_array2 <int, 10>>);

constexpr bool is_constant_evaluated() noexcept
{
    if consteval {
        return true;
    }
    else {
        return false;
    }
}

constexpr auto bajs (auto&& b) noexcept -> auto 
requires (is_constant_evaluated ())
{

}

template <typename T>
concept Bajs = requires (T t) {
	{t.begin ()};
};

auto bb1 (auto) {
	auto* i = new int;
	return i;
}

constexpr auto bb2 (auto g) {
	return g;
}

consteval auto bb3 (auto g) {
	return g;
}

#define CONSTEXPR_EVAL(...) \
  std::integral_constant< \
    std::decay_t<decltype(__VA_ARGS__)>, \
    __VA_ARGS__ \
  >::value

#define KKU(...) requires (std::integral_constant< \
    std::decay_t<decltype(__VA_ARGS__)>, \
    __VA_ARGS__ \
  >){true;}



// static_assert (not CONSTEXPR_EVAL (bb1 (4)));
static_assert (CONSTEXPR_EVAL (bb2 (4)));
static_assert (CONSTEXPR_EVAL (bb3 (4)));


// static_assert ( KKU(bb1 (4)));
static_assert (KKU(bb2 (4)));
static_assert (KKU (bb3 (4)));

#define IS_CONSTEXPR(expr) \
  __builtin_types_compatible_p(__typeof__((1 ? (void*) ((__INTPTR_TYPE__) ((expr) * 0)) : (int*) 0)), int*)

// static_assert ( not IS_CONSTEXPR(bb1 (4)));
// static_assert (IS_CONSTEXPR(bb2 (4)));
static_assert (__builtin_constant_p (bb2 (4)));
static_assert (__builtin_constant_p (bb3 (4)));
static_assert (not __builtin_constant_p (bb1 (4)));

template <typename T>
concept dslmd = requires (T& t){(bool) __builtin_constant_p (t.begin());};
static_assert (dslmd <super_array2 <int, 10>>);

struct notArray {
	auto* begin () noexcept {
		return new int;
	}
	auto* end () noexcept {
		return new int;
	}
};

auto main (int, char**) -> int 
{	
	// B0 auto b0 = super_array2 <int, 10> {};
	return 0;
} 