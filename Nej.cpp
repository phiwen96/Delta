// import Ja;
// #include <vulkan/vulkan_core.h>
// import Vulkan;
#include <iostream>
#include <vector>
#include <coroutine>
#include <utility>
import Vulkan;
// import Standard;

struct vRectangle {

};

struct something {
	struct promise_type {
		std::coroutine_handle <> continuation;
		// char* txt;
		auto get_return_object () -> something {
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
		// auto await_transform (vWindowEvent) -> auto {
		// 	struct awaiter {
		// 		auto await_ready () noexcept -> bool {
					
		// 			return false;
		// 		}
		// 		auto await_suspend (std::coroutine_handle <promise_type> h) noexcept -> std::coroutine_handle <> {
		// 			// return h.promise().continuation;
		// 			// std::cout << "future::final_suspend::await_suspend" << std::endl;
		// 			return h.promise().continuation ? h.promise().continuation : std::noop_coroutine ();
		// 		}
		// 		auto await_resume () noexcept -> void {
		// 			// std::cout << "future::final_suspend::await_resume" << std::endl;
		// 			// std::cout << "final_suspend::await_resume" << std::endl;
		// 		}
		// 	};
		// }
	};

	auto open () const -> bool {
		return handle.done();
	}

	auto draw () {
		
	}

	explicit something (promise_type& p) noexcept : handle {std::coroutine_handle <promise_type>::from_promise (p)} {}
	something (something&& o) noexcept : handle {std::exchange (o.handle, {})} {} 
	something (something const&) = delete;
	~something () {if (handle) handle.destroy();}
private:
	std::coroutine_handle <promise_type> handle;
};

auto run () -> something {
	auto window = vWindow {640, 480, "window 1"};
	co_await window.is().closed();
	// auto button = window.make_rectangle(10, 10);
	// co_await button.is().hovered();
	std::cout << "button hovered!" << std::endl;
	std::cout << "exiting app!" << std::endl;
	co_return;
}



// auto make_app_2 () -> vApp {
// 	auto button = 
// }



auto main (int argc, char** argv) -> int {
	// auto instance = vInstance {{"VK_LAYER_KHRONOS_validation"}, {"VK_KHR_surface"}};
	// auto physicalDevice = vPhysicalDevice {instance};


	auto app = run ();

	while (app.open ())
	{
		app.draw ();
	}
	

	// auto logicalDevice = vLogicalDevice {
		
	// 	physicalDevice};
	// auto instance = vInstance {{"VK_LAYER_KHRONOS_validation"}, {"VK_KHR_surface"}};
	std::cout << "hello world" << std::endl;
	// std::vector <int> baj;
	// VkInstance inst;
	return 0;
}