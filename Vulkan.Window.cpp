module;
#include <iostream>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#include <coroutine>
export module Vulkan.Window;

// export import :Coroutine;

export struct vWindowEvent {
	vWindowEvent (GLFWwindow* h) : handle {h} {}
	auto closed () {
		struct awaitable {
				auto await_ready () noexcept -> bool {
					
					return false;
				}
				auto await_suspend (std::coroutine_handle <> awaiting_coro) noexcept -> void {
					// return h.promise().continuation;
					// std::cout << "future::final_suspend::await_suspend" << std::endl;
					// return h.promise().continuation ? h.promise().continuation : std::noop_coroutine ();
					// return h;
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


export struct vWindow {
	vWindow (auto width, auto height, auto title) : handle {glfwCreateWindow (width, height, title, nullptr, nullptr)} {

	}
	auto is () -> vWindowEvent {
		return {handle};
	}
private:
	GLFWwindow* handle;
};