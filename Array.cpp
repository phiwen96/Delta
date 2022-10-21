export module Array;

export template <typename T, auto N>
struct array {
	T buffer [N];
	// constexpr array (T...) noexcept {

	// }
	constexpr auto data () noexcept -> T * {
		return buffer;
	}
	constexpr auto size () noexcept -> int {
		return N;
	}
};

export array (auto a, auto... b) -> array <decltype (a), sizeof... (b) + 1>;