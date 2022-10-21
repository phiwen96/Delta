module;
#include <cstdlib>
// #include <cstdio>
#include <type_traits>
#include <iostream>
export module Vector;

export template <typename T>
struct vector {	
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
	~vector () {
		std::free (buffer);
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
	constexpr auto size () noexcept {
		return next;
	}
	constexpr auto begin () noexcept -> T * {
		return buffer;
	}
	constexpr auto end () noexcept -> T * {
		return buffer + next;
	}

private:
	auto construct_element (int& i, auto&& v) noexcept -> void {
		::new (buffer + i) T (std::forward <decltype (v)> (v));
		++i;
	}
	T* buffer;
	int next;
	int max;
};

export template <typename T>
vector (T&&...) -> vector <T>;