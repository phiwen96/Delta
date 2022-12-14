module;
#include <cstdlib>
// #include <cstdio>
#include <type_traits>
#include <iostream>
#include <span>
#include <utility>
export module Mector;

export template <typename T>
struct mector {	
	// friend auto swap (mector& lhs, mector& rhs) noexcept -> void {
	// 	using std::swap;
	// 	std::swap (lhs.buffer, rhs.buffer);
	// 	std::swap (lhs.next, rhs.next);
	// 	std::swap (lhs.max, rhs.max);
	// }
	mector (unsigned int s) noexcept : buffer {(T*) std::malloc (s * sizeof (T))}, next {s}, max {s} {
		
	}
	mector (auto&&... t) noexcept : buffer {(T*) std::malloc (sizeof (T) * sizeof... (t))}, next {sizeof... (t)}, max {next} {
		auto i = 0;
		((construct_element (i, std::forward <decltype (t)> (t))), ...);
		// (construct_element (i, std::forward <decltype (t)> (t)))...;
		// std::cout << "hej" << std::endl;
		// printf ("hej");
		// printf ("%i\n", sizeof (t));
	}
	mector () noexcept : buffer {(T*) std::malloc (sizeof (T))}, next {0}, max {1} {
		
	}
	mector (mector && o) noexcept : buffer {nullptr}, next {0}, max {0} {
		// swap (*this, o);
		std::swap (buffer, o.buffer);
		std::swap (next, o.next);
		std::swap (max, o.max);
	}
	
	~mector () {
		for (auto i = 0; i < next; ++i) {
			buffer [i].~T();
		}
		std::free (buffer);
	}
	auto resize (int s) noexcept -> void {
		buffer = (T*) std::realloc (buffer, sizeof (T) * s);
		next = s - 1;
		max = s;
	}
	auto operator += (auto&& v) noexcept -> mector & {
		if (next == max) {
			max *= 2;
			buffer = (T*) std::realloc (buffer, sizeof (T) * max);
		}
		::new (buffer + next) T (std::forward <decltype (v)> (v));
		++next;
		return *this;
	}
	auto operator [] (int i) noexcept -> T & {
		return buffer [i];
	}

	constexpr auto data () noexcept -> T* {
		return buffer;
	}
	constexpr auto size () const noexcept {
		return next;
	}
	constexpr auto back () noexcept -> T & {
		return buffer [next - 1];
	}
	constexpr auto begin () noexcept -> T * {
		return buffer;
	}
	constexpr auto end () noexcept -> T * {
		return buffer + next;
	}
	constexpr auto begin () const noexcept -> T * const {
		return buffer;
	}
	constexpr auto end () const noexcept -> T * const {
		return buffer + next;
	}
	friend auto operator << (std::ostream & os, mector const& v) noexcept -> std::ostream& {
		for (auto* i = v.begin(); i < v.end(); ++i) {
			os << *i << " ";
		}
		return os;
	}
	// constexpr auto operator += (std::span s) noexcept -> mector & {
	// 	if (s.size ())
	// }

	constexpr auto space () const noexcept -> unsigned int {
		return max - next;
	}

private:
	auto construct_element (int& i, auto&& v) noexcept -> void {
		::new (buffer + i) T {std::forward <decltype (v)> (v)};
		++i;
	}
	T* buffer;
	unsigned int next;
	unsigned int max;
};

export template <typename T>
mector (T&&...) -> mector <T>;


// module;
// #include <cstdlib>
// // #include <cstdio>
// #include <type_traits>
// #include <iostream>
// export module mector;

// export template <typename T>
// struct mector {	
// 	mector (auto&&... t) noexcept : buffer {(T*) std::malloc (sizeof (T) * sizeof... (t))}, next {sizeof... (t)}, max {next} {
// 		auto i = 0;
// 		((construct_element (i, std::forward <decltype (t)> (t))), ...);
// 		// (construct_element (i, std::forward <decltype (t)> (t)))...;
// 		// std::cout << "hej" << std::endl;
// 		// printf ("hej");
// 		// printf ("%i\n", sizeof (t));
// 	}
// 	mector () noexcept : buffer {(T*) std::malloc (sizeof (T))}, next {0}, max {1} {
		
// 	}
// 	~mector () {
// 		std::free (buffer);
// 	}
// 	auto operator += (auto&& v) noexcept -> mector & {
// 		if (next == max) {
// 			max *= 2;
// 			buffer = (T*) std::realloc (buffer, sizeof (T) * max);
// 		}
// 		::new (buffer + next) T (std::forward <decltype (v)> (v));
// 		++next;
// 		return *this;
// 	}
// 	auto operator [] (int i) noexcept -> T & {
// 		return buffer [i];
// 	}

// 	constexpr auto data () noexcept -> T* {
// 		return buffer;
// 	}
// 	constexpr auto size () noexcept {
// 		return next;
// 	}
// 	constexpr auto begin () noexcept -> T * {
// 		return buffer;
// 	}
// 	constexpr auto end () noexcept -> T * {
// 		return buffer + next;
// 	}

// private:
// 	auto construct_element (int& i, auto&& v) noexcept -> void {
// 		::new (buffer + i) T (std::forward <decltype (v)> (v));
// 		++i;
// 	}
// 	T* buffer;
// 	int next;
// 	int max;
// };

// export template <typename T>
// mector (T&&...) -> mector <T>;