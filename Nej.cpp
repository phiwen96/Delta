// import Ja;
// #include <vulkan/vulkan_core.h>
// import Vulkan;
#include <iostream>
#include <vector>
#include <coroutine>
#include <utility>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
import Vulkan;
// import Standard;

struct vRectangle {

};

struct make_window {
	int width, height;
	char const* title;
};

struct is_clicked {
	GLFWwindow* handle;
};

struct is_closed {
	GLFWwindow* handle;
};

struct something {
	struct promise_type {
		std::coroutine_handle <> continuation;
		// GLFWwindow* window;
		// char* txt;
		auto get_return_object () -> something {
			glfwInit ();
			glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
			// window = glfwCreateWindow (640, 480, "window 1", nullptr, nullptr);
			// glfwSetWindowUserPointer (handle, this);
			// glfwSetWindowCloseCallback (handle, window_close_callback);
			return something {*this};
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
		auto await_transform (make_window&& m) -> auto {
			
			struct awaiter {
				auto await_ready () noexcept -> bool {
					
					return true;
				}
				auto await_suspend (std::coroutine_handle <promise_type> h) noexcept -> std::coroutine_handle <> {
					// return h.promise().continuation;
					// std::cout << "future::final_suspend::await_suspend" << std::endl;
					return h.promise().continuation ? h.promise().continuation : std::noop_coroutine ();
				}
				auto await_resume () noexcept -> auto {
					// std::cout << "future::final_suspend::await_resume" << std::endl;
					// std::cout << "final_suspend::await_resume" << std::endl;
					return glfwCreateWindow (640, 480, "window 1", nullptr, nullptr);
				}
			};
			
			return awaiter {};
		}
		static void mouse_button_callback(GLFWwindow* w, int button, int action, int mods)
		{
			// if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			if (action == GLFW_RELEASE) {
				glfwSetMouseButtonCallback (w, nullptr);
				std::coroutine_handle <promise_type>::from_address (glfwGetWindowUserPointer (w)).resume ();
			}
				
		}
		static void window_close_callback (GLFWwindow* w)
		{
			glfwSetWindowCloseCallback (w, nullptr);
			std::coroutine_handle <promise_type>::from_address (glfwGetWindowUserPointer (w)).resume ();
				// glfwSetWindowShouldClose(window, GLFW_FALSE);
		}
		auto await_transform (is_clicked&& m) -> auto {
			glfwSetMouseButtonCallback (m.handle, mouse_button_callback);
			glfwSetWindowUserPointer (m.handle, std::coroutine_handle <promise_type>::from_promise (*this).address());
			struct awaiter {
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
					// return glfwCreateWindow (640, 480, "window 1", nullptr, nullptr);
				}
			};
			
			return awaiter {};
		}
		auto await_transform (is_closed&& m) -> auto {
			glfwSetWindowCloseCallback (m.handle, window_close_callback);
			glfwSetWindowUserPointer (m.handle, std::coroutine_handle <promise_type>::from_promise (*this).address());
			struct awaiter {
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
					// return glfwCreateWindow (640, 480, "window 1", nullptr, nullptr);
				}
			};
			
			return awaiter {};
		}
	};

	auto open () const -> bool {
		return not handle.done();
		// return not glfwWindowShouldClose (handle.promise().window);
	}

	auto draw () {
		glfwPollEvents ();
		
	}

	explicit something (promise_type& p) noexcept : handle {std::coroutine_handle <promise_type>::from_promise (p)} {}
	something (something&& o) noexcept : handle {std::exchange (o.handle, {})} {} 
	something (something const&) = delete;
	~something () {if (handle) handle.destroy();}
private:
	std::coroutine_handle <promise_type> handle;
};

struct vWindow {
	vWindow (auto width, auto height, auto title) : handle {glfwCreateWindow (width, height, title, nullptr, nullptr)} {

	}

	struct vWindowEvent {
		vWindowEvent (GLFWwindow* h) : handle {h} {}
		auto closed () {
			struct awaitable {
					auto await_ready () noexcept -> bool {
						
						return false;
					}
					// in this case, the await_coro is wanna get notified, so we should resume/return its continuation
					auto await_suspend (std::coroutine_handle <something::promise_type> awaiting_coro) noexcept -> std::coroutine_handle <> {
						// return h.promise().continuation;
						// std::cout << "future::final_suspend::await_suspend" << std::endl;
						return awaiting_coro.promise().continuation ? awaiting_coro.promise().continuation : std::noop_coroutine ();
						// return h;
						// return awaiting_coro;
					}
					auto await_resume () noexcept -> void {
						// std::cout << "future::final_suspend::await_resume" << std::endl;
						// std::cout << "final_suspend::await_resume" << std::endl;
					}
				};
				return awaitable {};
		}

	private:
		GLFWwindow* handle;
	};

	auto is () -> vWindowEvent {
		return {handle};
	}
private:
	GLFWwindow* handle;
};

struct vApp {

};



auto run () -> something {
	std::cout << "run ..." << std::endl;
	auto window = glfwCreateWindow (640, 480, "window 1", nullptr, nullptr);
	// auto button = 
	// co_await is_clicked {make_rectangle {window, 10, 10}};
	co_await is_clicked {window};
	co_await is_closed {window};

	// co_await is_clicked {window};
	// co_await is_clicked {window};
	// co_await is_clicked {window};
	
	
	// co_await mainWindow.is().closed();
	// auto button = window.make_rectangle(10, 10);
	// co_await button.is().hovered();
	// std::cout << "button hovered!" << std::endl;
	// std::cout << "exiting app!" << std::endl;
	std::cout << "... run" << std::endl;
	co_return;
}



// auto make_app_2 () -> vApp {
// 	auto button = 
// }





auto main (int argc, char** argv) -> int {
	// auto instance = vInstance {{"VK_LAYER_KHRONOS_validation"}, {"VK_KHR_surface"}};
	// auto physicalDevice = vPhysicalDevice {instance};
	
	std::cout << "main ..." << std::endl;
	
	auto app = run ();

	while (app.open ())
	{
		// std::cout << "hello world" << std::endl;
		app.draw ();
	}
	

	// auto logicalDevice = vLogicalDevice {
		
	// 	physicalDevice};
	// auto instance = vInstance {{"VK_LAYER_KHRONOS_validation"}, {"VK_KHR_surface"}};
	
	// std::vector <int> baj;
	// VkInstance inst;
	std::cout << "... main" << std::endl;
	return 0;
}