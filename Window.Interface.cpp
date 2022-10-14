module;
#include <GLFW/glfw3.h>
#include <coroutine>
#include <utility>
// #include <iostream>
#include <stdio.h> // printf
#include <cstdlib> // exit
export module Window:Interface;

import Concepts.Bool;
// import Coro;

export template <typename T>
concept Window = requires (T window) {
	{window.show ()} noexcept -> Bool;
};


export struct window_coro {
	auto await_ready () noexcept -> bool {
		return false;
	}
	auto await_suspend (std::coroutine_handle <> h) noexcept -> void {
		// h.resume();
	}
	auto await_resume () noexcept -> void {

	}
	// auto yield_value () noexcept -> std::suspend_always {
	// 	return {};
	// }
};

export struct /*[[nodiscard]]*/ future_io {
	struct promise_type {
		std::coroutine_handle <> continuation;
		// char* txt;
		auto get_return_object () -> future_io {
			return future_io {*this};
		}
		auto initial_suspend () -> std::suspend_never {
			return {};
		}
		auto final_suspend () noexcept -> auto {
			// std::cout << "final_suspend" << std::endl;
			struct awaitable {
				auto await_ready () noexcept -> bool {
					return false;
				}
				auto await_suspend (std::coroutine_handle <promise_type> h) noexcept -> std::coroutine_handle <> {
					// std::cout << "future_io::final_suspend::await_suspend" << std::endl;
					return h.promise().continuation ? h.promise().continuation : std::noop_coroutine ();
				}
				auto await_resume () noexcept -> void {
					// std::cout << "future_io::final_suspend::await_resume" << std::endl;
				}
			};
			return awaitable {};
			// return std::suspend_never {};
			// return std::suspend_always {};
		}
		void unhandled_exception() {exit (-1);}
		
		auto return_void () noexcept -> void {

		}
	
		// auto yield_value (void*& p) noexcept -> std::suspend_never {
		// 	p = std::coroutine_handle <promise_type>::from_promise (*this).address();
		// 	return {};
		// }

		auto yield_value (GLFWwindow* handle) noexcept -> std::suspend_always {
			printf ("setting user pointer\n");
			// Weird ?
			glfwSetWindowUserPointer (handle, continuation.address());
			return {};
		}
	};

	auto done () const noexcept -> bool {
		return handle.done ();
	}
	auto await_ready () noexcept -> bool {
		return false;
	}
	auto await_suspend (std::coroutine_handle <> c, char const* from = __builtin_FUNCTION(), int line = __builtin_LINE()) noexcept//std::coroutine_handle <promise_type> 
	{
		printf ("future_io::await_suspend called from %s  ");
		// std::cout << "future_io::await_suspend called from " << from << line << std::endl;
		// std::cout << "await_suspend" << std::endl;
		// printf ("await_suspend\n");
		handle.promise().continuation = c;

		return handle;
	}
	auto await_resume () noexcept -> void {
		printf ("future_io::await_resume\n");
		// return handle.promise().txt;
	}
	explicit future_io (promise_type& p) noexcept : handle {std::coroutine_handle <promise_type>::from_promise (p)} {}
	future_io (future_io&& o) noexcept : handle {std::exchange (o.handle, {})} {} 
	future_io (future_io const&) = delete;
	~future_io () {if (handle) handle.destroy();}
private:
	std::coroutine_handle <promise_type> handle;
};

struct my_coro_handle {
	auto await_ready () noexcept {
		return false;
	}
	auto await_suspend (std::coroutine_handle <> h) noexcept {
		// return h;
		handle = h;
		return false;
		
	}
	auto await_resume () noexcept {
		return handle;
	}
	std::coroutine_handle <> handle;
};

export struct window {
	auto click () noexcept -> future_io {
		printf("setting glfwSetWindowUserPointer\n");
		co_yield handle;
		printf("setting glfwSetWindowUserPointer\n");
		co_await std::suspend_always {};
		// printf("setting glfwSetWindowUserPointer\n");
		co_return;
		// auto h = co_await my_coro_handle {};
		// void * h;
		// co_yield h;
		// printf("setting glfwSetWindowUserPointer\n");
		// glfwSetWindowUserPointer (handle, h);
		// co_await std::suspend_always {};
		// printf("Release?\n");
		// co_yield;
		// printf ("registering a click\n");
		// co_return nullptr;
	}

	window () noexcept;
	window (int width, int height, char const* title) noexcept : handle {glfwCreateWindow (width, height, title, nullptr, nullptr)} {
		glfwSetWindowCloseCallback (handle, window_close_callback);
	}
	window (window&&) noexcept;
	window (window const&) = delete;
	~window ();

	auto show () noexcept -> bool {
		return true;
	}
	GLFWwindow* handle;
private:
	
	static void window_close_callback(GLFWwindow* window)
	{
		// printf ("trying to close\n");
		// if (!time_to_close)
		// glfwSetWindowShouldClose (window, true);
		glfwDestroyWindow (window);
	}
	static auto mouse_button_callback (GLFWwindow* w, int button, int action, int mods) noexcept -> void {
		// printf ("m\n");
		if (auto * p = glfwGetWindowUserPointer (w); p == nullptr) {
			printf ("m\n");
			auto h = std::coroutine_handle <>::from_address (p); 
				// if (not h.done ()) {
				// 	printf ("yay\n");
				// 	h.resume ();
				// 	// printf ("~m\n");
				// 	glfwSetWindowUserPointer (w, nullptr);
				// } else {
				// 	printf("what\n");
				// }
			h.resume ();
			glfwSetWindowUserPointer (w, nullptr);
			
		} else {
			printf ("user pointer is null\n");
		}
		
		// printf ("~m\n");
	}
};

