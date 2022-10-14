#include <iostream>
#include <semaphore>
#include <coroutine>
#include <utility>
#include <GLFW/glfw3.h>
// import App; 
// import Window;

import Concepts.Bool;
// import Window; 
// import App;
import Coro;
import Graphics;



template <typename...>
struct future;

template <>
struct future <> {
	struct promise_type {
		std::coroutine_handle <> continuation;
		auto get_return_object () -> future {
			return future {*this};
		}
		auto initial_suspend () -> std::suspend_never {
			return {};
		}
		auto final_suspend () noexcept -> auto {
			struct awaitable {
				auto await_ready () noexcept -> bool {
					
					return false;
				}
				auto await_suspend (std::coroutine_handle <promise_type> h) noexcept -> std::coroutine_handle <> {
					// return h.promise().continuation;
					// std::cout << "future::final_suspend::await_suspend" << std::endl;
					return h.promise().continuation ? h.promise().continuation : std::noop_coroutine ();
				}
				auto await_resume () noexcept -> void {
					// std::cout << "future::final_suspend::await_resume" << std::endl;
					// std::cout << "final_suspend::await_resume" << std::endl;
				}
			};
			return awaitable {};
		}
		void unhandled_exception() {std::cout << "error >> unhandled_exception" << std::endl; exit (-1);}
		auto return_void () -> void {}
	};
	auto await_ready () noexcept -> bool {
		return false;
	}
	auto await_suspend (std::coroutine_handle <> c, auto from = __builtin_FUNCTION()) noexcept -> std::coroutine_handle <promise_type> {
		std::cout << "future::await_suspend called from " << from << std::endl;
		// std::cout << "await_suspend" << std::endl;
		handle.promise().continuation = c;
		return handle;
	}
	auto await_resume () noexcept -> void {
		std::cout << "future::await_resume" << std::endl;
	}
	auto done () const noexcept -> bool {
		return handle.done ();
	}
	explicit future (promise_type& p) noexcept : handle {std::coroutine_handle <promise_type>::from_promise (p)} {}
	future (future&& o) noexcept : handle {std::exchange (o.handle, {})} {} 
	future (future const&) = delete;
	~future () {if (handle) handle.destroy();}
private:
	std::coroutine_handle <promise_type> handle;
};



// import Vulkan;
// auto sem = std::binary_semaphore {0};

auto mouse_button_callback (GLFWwindow* w, int button, int action, int mods) noexcept -> void {
		// printf ("m\n");
		if (void * p = glfwGetWindowUserPointer (w); p != nullptr) {
			printf ("mmm\n");
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
			// glfwSetWindowUserPointer (w, nullptr);
			
		} else {
			printf ("user pointer is null\n");
		}
		
		// printf ("~m\n");
	}




// auto make_express (App auto& ap) noexcept -> future <> {
	
// 	Window auto& w1 = ap.new_window ("express");
// 	// glfwSetMouseButtonCallback (w1.handle, mouse_button_callback);
// 	std::cout << "waiting for a click" << std::endl;
// 	co_await w1.click ();
// 	std::cout << "click done" << std::endl;
// 	std::cout << "waiting for a click 2" << std::endl;
// 	co_await w1.click ();
// 	std::cout << "click 2 done" << std::endl;
// 	// std::cout <<

// 	// sem.release ();
// }



// auto exp


auto main () -> int {

	Graphics auto g = graphics_t {};
	Window auto & w1 = g.make_window ();

	// Window auto & w2 = g.make_window ();
	// std::cout << g.windows_open () << std::endl;
	while (g.windows_open ()) {
		g.draw_frame ();
		g.process ();
	}
	// App auto my_app = app {"Express"};
	// my_app.new_window ("1");
	// my_app.new_window ("2");
	// auto& win = my_app.new_window ("Express");
	// glfwWindowShouldClose (win.handle);
	// glfwPollEvents ();
	// while (my_app.windows_open ())
	// {
	// 	my_app.render ();
	// 	my_app.process ();
	// 	/* code */
	// }
	
	// while (my_app.windows_open () > 0) {
	// 	my_app.draw_frames ();
	// }
	
	// auto ap = make_express (my_app);
	// if (not my_app.run ()) {
	// 	std::cout << "failed to run express" << std::endl;
	// 	exit (-1);
	// }

	// sem.acquire ();
	// App auto my_app = app {"Express"};
	// Window auto w1 = my_app.new_window ();
	// if (not my_app.run ()) {
	// 	std::cout << "error >> failed to run app" << std::endl;
	// 	exit (-1);
	// }
	
	return 0;
}