module;
#include <iostream>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#include <coroutine>
#include <utility>
#include <array>
#include <bit>
#include <algorithm>
export module Vulkan.App;
import Vulkan.Window;

export struct is_clicked {
	GLFWwindow* handle;
	is_clicked (GLFWwindow* h) : handle {h} {
		glfwSetMouseButtonCallback (handle, mouse_button_callback);
	}

	static void mouse_button_callback (GLFWwindow* w, int button, int action, int mods) {
		// if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		if (action == GLFW_RELEASE) {
			glfwSetMouseButtonCallback (w, nullptr);
			std::coroutine_handle <>::from_address (glfwGetWindowUserPointer (w)).resume ();
		}
				
	}
};

export struct is_closed {
	GLFWwindow* handle;
	is_closed (GLFWwindow* h) : handle {h} {
		glfwSetWindowCloseCallback (handle, window_close_callback);
	}

	static void window_close_callback (GLFWwindow* w) {
		glfwSetWindowCloseCallback (w, nullptr);
		std::coroutine_handle <>::from_address (glfwGetWindowUserPointer (w)).resume ();
	}
};

struct make_window {
	int width, height;
};


export struct xWindow {
	xWindow (int width, int height, char const* title) noexcept : handle {glfwCreateWindow (width, height, title, nullptr, nullptr)} {

	}
	xWindow (xWindow&& o) : handle {nullptr} {
		std::swap (handle, o.handle);
	}
	~xWindow () {
		// std::cout << "~xWindow()" << std::endl;
		if (handle) {
			glfwDestroyWindow (handle);
		}
	}
	friend auto swap (xWindow& lhs, xWindow& rhs) -> void {
		using std::swap;
		std::swap (lhs.handle, rhs.handle);
	}
	auto open () const noexcept -> bool {
		return not glfwWindowShouldClose (handle);
	}

private:
	GLFWwindow* handle;
};

export template <typename coro, typename mixin>
struct promise_t : mixin {
	std::coroutine_handle <> continuation;
	auto get_return_object () -> coro {
		return coro {*this};
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
			auto await_suspend (std::coroutine_handle <promise_t> h) noexcept -> std::coroutine_handle <> {
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

	// auto yield_value (auto && v) -> decltype (auto) //requires requires {mixin::yield_value (std::forward <decltype (v)> (v));} 
	// {
	// 	return mixin::yield_value (std::forward <decltype (v)> (v));
	// }

	// auto yield_value (GLFWwindow* w) -> auto {
	// 	struct awaitable {
	// 		bool await_ready() { return true; }
	// 		auto await_suspend(std::coroutine_handle<> c) {
		
	// 		}
	// 		void await_resume() {
	// 			// if (handle.promise().e_)
	// 			// std::rethrow_exception(handle.promise().e_);
	// 		}
	// 	};
	// 	return awaitable {};
	// }
	
};

export template <template <typename...> typename mixin, typename promise>
struct coro_t : mixin <coro_t <mixin, promise>, promise> {
	using promise_type = promise_t <coro_t <mixin, promise>, promise>;
	explicit coro_t (promise_type& p) noexcept : handle {std::coroutine_handle <promise_type>::from_promise (p)} {
		
	}
	coro_t (coro_t&& o) noexcept : handle {std::exchange (o.handle, {})} {
		// std::cout << "coro_t (coro_t&& o) noexcept" << std::endl;
	} 
	coro_t (coro_t const&) = delete;
	~coro_t () {
		if (handle) handle.destroy();
	}
	// Awaitable interface
	bool await_ready() { return false; }
	auto await_suspend(std::coroutine_handle<> c) {
		handle.promise().continuation = c;
		return handle;
	}
	void await_resume() {
		// if (handle.promise().e_)
		// std::rethrow_exception(handle.promise().e_);
	}
// private:
	std::coroutine_handle <promise_type> handle;
};

export struct vAppPromise {
	std::vector <GLFWwindow*> windows;

	auto yield_value (GLFWwindow* w) noexcept -> decltype (auto) {
		windows.push_back (w);
		struct awaitable {
			inline bool await_ready() noexcept { return true; }
			inline void await_suspend(std::coroutine_handle<> c) noexcept {
		
			}
			inline void await_resume() noexcept {
				// if (handle.promise().e_)
				// std::rethrow_exception(handle.promise().e_);
			}
		};
		return awaitable {};
	}
};

export template <typename coro, typename promise>
struct vAppCoro {
	vAppCoro () {
		glfwInit ();
		glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
	}
	auto windows_open () -> bool {
		auto& prom = (static_cast <coro&> (*this)).handle.promise();
		auto& windows = (static_cast <promise&> (prom)).windows;
		for (auto i = windows.begin(); i < windows.end(); ++i) {
			if (not glfwWindowShouldClose (*i)) {
				return true;
			} else {
				glfwDestroyWindow (*i);
				i = windows.erase(i);
			}
		}
		return false;
	}
	auto draw () -> void {

	}
};

export using vApp = coro_t <vAppCoro, vAppPromise>;

// export template <int max_windows = 2>
// struct [[nodiscard]] vApp {
// 	struct promise_type {
// 		// vInstance instance {{"VK_LAYER_KHRONOS_validation"}, {"VK_KHR_surface"}};
// 		std::coroutine_handle <> continuation;
// 		std::vector <GLFWwindow*> windows;
// 		// std::array <GLFWwindow*, max_windows> windows;
// 		// int windows_count {0};
// 		// alignas (alignof (xWindow)) std::byte xwindows [max_windows * sizeof (xWindow)];
// 		// auto* user = ::new (memory) User("john");
// 		// user->~User();
// 		int xwindows_count {0};
// 		// GLFWwindow* window;
// 		// char* txt;
// 		// auto await_transform (make_window&& m) {
// 		// 	if (windows_count == max_windows) {
// 		// 		std::cout << "error >> not enough windows in array" << std::endl;
// 		// 		exit (-1);
// 		// 	}

// 		// 	windows [windows_count] = glfwCreateWindow (m.width, m.height, "", nullptr, nullptr);
// 		// 	struct awaitable {
// 		// 		GLFWwindow* w;
// 		// 		auto await_ready () noexcept -> bool {
// 		// 			return true;
// 		// 		}
// 		// 		auto await_suspend (std::coroutine_handle <> h) noexcept -> void {
// 		// 			// std::cout << "future_io::final_suspend::await_suspend" << std::endl;
// 		// 			// return h.promise().continuation ? h.promise().continuation : std::noop_coroutine ();
// 		// 		}
// 		// 		auto await_resume () noexcept -> GLFWwindow* {
// 		// 			// std::cout << "future_io::final_suspend::await_resume" << std::endl;
// 		// 			return w;
// 		// 		}
// 		// 	};
// 		// 	return awaitable {windows [windows_count++]};
// 		// 	// if (xwindows_count == max_windows) {
// 		// 	// 	std::cout << "error >> not enough windows in array" << std::endl;
// 		// 	// 	exit (-1);
// 		// 	// }

// 		// 	// auto * w = ::new (xwindows + (xwindows_count * sizeof (xWindow))) xWindow {m.width, m.height, ""};

// 		// 	// ++xwindows_count;

// 		// 	// // windows [windows_count] = glfwCreateWindow (m.width, m.height, "", nullptr, nullptr);
// 		// 	// struct awaitable {
// 		// 	// 	xWindow& w;
// 		// 	// 	auto await_ready () noexcept -> bool {
// 		// 	// 		return true;
// 		// 	// 	}
// 		// 	// 	auto await_suspend (std::coroutine_handle <> h) noexcept -> void {
// 		// 	// 		// std::cout << "future_io::final_suspend::await_suspend" << std::endl;
// 		// 	// 		// return h.promise().continuation ? h.promise().continuation : std::noop_coroutine ();
// 		// 	// 	}
// 		// 	// 	auto await_resume () noexcept -> xWindow& {
// 		// 	// 		// std::cout << "future_io::final_suspend::await_resume" << std::endl;
// 		// 	// 		return w;
// 		// 	// 	}
// 		// 	// };

// 		// 	// return awaitable {*w};
// 		// }
// 		auto get_return_object () -> vApp {
// 			glfwInit ();
// 			glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
// 			// window = glfwCreateWindow (640, 480, "window 1", nullptr, nullptr);
// 			// glfwSetWindowUserPointer (handle, this);
// 			// glfwSetWindowCloseCallback (handle, window_close_callback);
// 			return vApp {*this};
// 		}
// 		auto initial_suspend () -> std::suspend_never {
// 			// std::cout << "initial_suspend ()" << std::endl;
// 			return {};
// 		}
// 		auto final_suspend () noexcept -> auto {
// 			// std::cout << "final_suspend" << std::endl;
// 			struct awaitable {
// 				auto await_ready () noexcept -> bool {
// 					return false;
// 				}
// 				auto await_suspend (std::coroutine_handle <promise_type> h) noexcept -> std::coroutine_handle <> {
// 					// std::cout << "future_io::final_suspend::await_suspend" << std::endl;
// 					return h.promise().continuation ? h.promise().continuation : std::noop_coroutine ();
// 				}
// 				auto await_resume () noexcept -> void {
// 					// std::cout << "future_io::final_suspend::await_resume" << std::endl;
// 				}
// 			};
// 			return awaitable {};
// 			// return std::suspend_never {};
// 			// return std::suspend_always {};
// 		}
// 		void unhandled_exception() {exit (-1);}
		
// 		auto return_void () noexcept -> void {
			
// 		}
// 		auto yield_value (GLFWwindow* w) {
// 			struct awaitable {
// 				// GLFWwindow* w;
// 				auto await_ready () noexcept {
// 					return true;
// 				}
// 				auto await_suspend (std::coroutine_handle <>) noexcept {

// 				}
// 				auto await_resume () noexcept {
// 					// return w;
// 				}
// 			};
// 			return awaitable {};
// 		}

// 		// auto await_transform (auto&& window_event) -> auto {
// 		// 	// glfwSetWindowCloseCallback (m.handle, window_close_callback);
// 		// 	glfwSetWindowUserPointer (window_event.handle, std::coroutine_handle <promise_type>::from_promise (*this).address());
// 		// 	// m.doit();
// 		// 	struct awaiter {
// 		// 		auto await_ready () noexcept -> bool {
					
// 		// 			return false;
// 		// 		}
// 		// 		auto await_suspend (std::coroutine_handle <promise_type> h) noexcept -> std::coroutine_handle <> {
// 		// 			// return h.promise().continuation;
// 		// 			// std::cout << "future::final_suspend::await_suspend" << std::endl;
// 		// 			return h.promise().continuation ? h.promise().continuation : std::noop_coroutine ();
// 		// 		}
// 		// 		auto await_resume () noexcept -> void {
// 		// 			// std::cout << "future::final_suspend::await_resume" << std::endl;
// 		// 			// std::cout << "final_suspend::await_resume" << std::endl;
// 		// 			// return glfwCreateWindow (640, 480, "window 1", nullptr, nullptr);
// 		// 		}
// 		// 	};
			
// 		// 	return awaiter {};
// 		// }
		
// 	private:
// 		auto initialize_instance () noexcept -> void {

// 		}
// 	};
	
// 	// false if all closed
// 	auto windows_open () const -> bool {
// 		// return not handle.done();
// 		// auto & xwindows = handle.promise().xwindows;
// 		// auto & xwindows_count = handle.promise().xwindows_count;
// 		// // for (auto* i = xwindows; i < xwindows + (xwindows_count * sizeof (xWindow)); i += sizeof (xWindow)) {
// 		// // 	auto* w = (xWindow*) i;
// 		// // 	if (w -> open ()) {
// 		// // 		return true;
// 		// // 	} else {
// 		// // 		w -> ~xWindow ();
// 		// // 		for (auto * )
// 		// // 	}
// 		// // }

// 		// for (auto i = 0; i < xwindows_count; ++i) {
// 		// 	auto* w = (xWindow*) xwindows [i * sizeof (xWindow)];
// 		// 	std::cout << "yay" << std::endl;
// 		// 	auto b = w -> open ();
// 		// 	std::cout << "yay" << std::endl;
// 		// 	if (w -> open ()) {
// 		// 		return true;
// 		// 	} else {
// 		// 		// w -> ~xWindow ();
// 		// 		for (auto j = i + 1; j < xwindows_count; ++j) {
// 		// 			swap (*((xWindow*) xwindows [i * sizeof (xWindow)]), *((xWindow*) xwindows [j * sizeof (xWindow)]));
// 		// 		}
// 		// 		--xwindows_count;
// 		// 	}
// 		// }
// 		// auto& count = handle.promise().windows_count;
// 		// auto& ws = handle.promise().windows;
// 		// for (int i = 0; i < count; ++i) {
// 		// 	if (window_open (ws [i])) {
// 		// 		return true;
// 		// 	} else {
// 		// 		// std::cout<<"destroying window\n";
// 		// 		glfwDestroyWindow (ws [i]);
// 		// 		for (int j = i + 1; j < count; ++j) {
// 		// 			ws [j - 1] = ws [j];
// 		// 		}
// 		// 		--count;
// 		// 	}
// 		// }
// 		// return false;
// 		// // return not glfwWindowShouldClose (handle.promise().window);
// 		// return false;
// 		// for (auto i = 0; i < windows.size (); ++i) {
// 		// 	if (window_open (windows [i])) {
// 		// 		return true;
// 		// 	} else {

// 		// 	}
// 		// }
// 		return false;
// 	}

// 	auto draw () {
// 		glfwPollEvents ();
		
// 	}

// 	explicit vApp (promise_type& p) noexcept : handle {std::coroutine_handle <promise_type>::from_promise (p)} {
// 		// std::cout << "explicit vApp (promise_type& p) noexcept" << std::endl;
// 		initialize_glfw ();
// 		set_glfw_hints ();
// 	}
// 	vApp (vApp&& o) noexcept : handle {std::exchange (o.handle, {})} {
// 		// std::cout << "vApp (vApp&& o) noexcept" << std::endl;
// 	} 
// 	vApp (vApp const&) = delete;
// 	~vApp () {
// 		if (handle) handle.destroy();
// 		terminate_glfw ();
// 		// std::cout << "~vApp ()" << std::endl;
// 	}
// 	// Awaitable interface
// 	bool await_ready() { return false; }
// 	auto await_suspend(std::coroutine_handle<> c) {
// 		handle.promise().continuation = c;
// 		return handle;
// 	}
// 	void await_resume() {
// 		// if (handle.promise().e_)
// 		// std::rethrow_exception(handle.promise().e_);
// 	}
// 	auto wait () -> void {
// 		handle.promise().sem.acquire ();
// 	}
// private:
// 	std::coroutine_handle <promise_type> handle;

// 	static auto window_open (GLFWwindow* w) noexcept -> bool {
// 		return not glfwWindowShouldClose (w);
// 	}
// 	static auto initialize_glfw () noexcept -> void {
// 		glfwInit ();
// 	}
// 	static auto terminate_glfw () noexcept -> void {
// 		glfwTerminate ();
// 	}
// 	static auto set_glfw_hints () noexcept -> void {
// 		glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
// 	}
// };