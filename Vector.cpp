module;
#include <cstdlib>
// #include <cstdio>
#include <type_traits>
#include <iostream>
#include <span>
export module Vector;

export template <typename T>
struct vector {	
	friend auto swap (vector& lhs, vector& rhs) noexcept -> void {
		using std::swap;
		std::swap (lhs.buffer, rhs.buffer);
		std::swap (lhs.next, rhs.next);
		std::swap (lhs.max, rhs.max);
	}
	vector (unsigned int s) noexcept : buffer {(T*) std::malloc (s * sizeof (T))}, next {s}, max {s} {
		
	}
	vector (auto&&... t) noexcept : buffer {(T*) std::malloc (sizeof (T) * sizeof... (t))}, next {sizeof... (t)}, max {next} {
		auto i = 0;
		((construct_element (i, std::forward <decltype (t)> (t))), ...);
		// (construct_element (i, std::forward <decltype (t)> (t)))...;
		// std::cout << "hej" << std::endl;
		// printf ("hej");
		// printf ("%i\n", sizeof (t));
	}
	vector () noexcept : buffer {(T*) std::malloc (sizeof (T))}, next {0}, max {1} {
		
	}
	vector (vector && o) noexcept : buffer {nullptr}, next {0}, max {0} {
		swap (*this, o);
	}
	
	~vector () {
		std::free (buffer);
	}
	auto resize (int s) noexcept -> void {
		buffer = (T*) std::realloc (buffer, sizeof (T) * s);
		next = s - 1;
		max = s;
	}
	auto operator += (auto&& v) noexcept -> vector & {
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
	friend auto operator << (std::ostream & os, vector const& v) noexcept -> std::ostream& {
		for (auto* i = v.begin(); i < v.end(); ++i) {
			os << *i << " ";
		}
		return os;
	}
	// constexpr auto operator += (std::span s) noexcept -> vector & {
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
vector (T&&...) -> vector <T>;

export template <typename T>
struct bajs {
	auto kiss () -> int;
};
// module;
// #include <cstdlib>
// // #include <cstdio>
// #include <type_traits>
// #include <iostream>
// export module Vector;

// export template <typename T>
// struct vector {	
// 	vector (auto&&... t) noexcept : buffer {(T*) std::malloc (sizeof (T) * sizeof... (t))}, next {sizeof... (t)}, max {next} {
// 		auto i = 0;
// 		((construct_element (i, std::forward <decltype (t)> (t))), ...);
// 		// (construct_element (i, std::forward <decltype (t)> (t)))...;
// 		// std::cout << "hej" << std::endl;
// 		// printf ("hej");
// 		// printf ("%i\n", sizeof (t));
// 	}
// 	vector () noexcept : buffer {(T*) std::malloc (sizeof (T))}, next {0}, max {1} {
		
// 	}
// 	~vector () {
// 		std::free (buffer);
// 	}
// 	auto operator += (auto&& v) noexcept -> vector & {
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
// vector (T&&...) -> vector <T>;