// import Ja;
// #include <vulkan/vulkan_core.h>
// import Vulkan;
#include <iostream>
// #include <vector>
#include <coroutine>
#include <utility>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#include <semaphore>
#include <assert.h>
#include <array>
#include <tuple>
#include <span>
#include <concepts>
import Delta;
// import Standard;

struct vRectangle {

};







// struct vWindow {
// 	vWindow (auto width, auto height, auto title) : handle {glfwCreateWindow (width, height, title, nullptr, nullptr)} {

// 	}

// 	struct vWindowEvent {
// 		vWindowEvent (GLFWwindow* h) : handle {h} {}
// 		auto closed () {
// 			struct awaitable {
// 					auto await_ready () noexcept -> bool {
						
// 						return false;
// 					}
// 					// in this case, the await_coro is wanna get notified, so we should resume/return its continuation
// 					auto await_suspend (std::coroutine_handle <vApp::promise_type> awaiting_coro) noexcept -> std::coroutine_handle <> {
// 						// return h.promise().continuation;
// 						// std::cout << "future::final_suspend::await_suspend" << std::endl;
// 						return awaiting_coro.promise().continuation ? awaiting_coro.promise().continuation : std::noop_coroutine ();
// 						// return h;
// 						// return awaiting_coro;
// 					}
// 					auto await_resume () noexcept -> void {
// 						// std::cout << "future::final_suspend::await_resume" << std::endl;
// 						// std::cout << "final_suspend::await_resume" << std::endl;
// 					}
// 				};
// 				return awaitable {};
// 		}

// 	private:
// 		GLFWwindow* handle;
// 	};

// 	auto is () -> vWindowEvent {
// 		return {handle};
// 	}
// private:
// 	GLFWwindow* handle;
// };


struct coWindow {
	struct promise_type {
		// vInstance instance {{"VK_LAYER_KHRONOS_validation"}, {"VK_KHR_surface"}};
		std::coroutine_handle <> continuation;
		std::tuple <GLFWwindow*, VkSurfaceKHR> value;

		// char* txt;
		auto get_return_object () -> coWindow {
			// glfwInit ();
			// glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
			// window = glfwCreateWindow (640, 480, "window 1", nullptr, nullptr);
			// glfwSetWindowUserPointer (handle, this);
			// glfwSetWindowCloseCallback (handle, window_close_callback);
			return coWindow {*this};
		}
		auto initial_suspend () -> std::suspend_never {
			// std::cout << "initial_suspend ()" << std::endl;
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
		
		auto return_value (std::tuple <GLFWwindow*, VkSurfaceKHR> && v) noexcept -> void {
			value = std::move (v);
		}
	};
		
	explicit coWindow (promise_type& p) noexcept : handle {std::coroutine_handle <promise_type>::from_promise (p)} {
		// std::cout << "explicit vApp (promise_type& p) noexcept" << std::endl;

	}
	coWindow (coWindow&& o) noexcept : handle {std::exchange (o.handle, {})} {
		// std::cout << "coWindow (coWindow&& o) noexcept" << std::endl;
	} 
	coWindow (coWindow const&) = delete;
	~coWindow () {
		if (handle) handle.destroy();
	
		// std::cout << "~vApp ()" << std::endl;
	}
	// Awaitable interface
	bool await_ready() noexcept { return true; }
	auto await_suspend(std::coroutine_handle<> c) noexcept {
		handle.promise().continuation = c;
		return handle;
	}
	auto await_resume() noexcept -> std::tuple <GLFWwindow*, VkSurfaceKHR> {
		// if (handle.promise().e_)
		// std::rethrow_exception(handle.promise().e_);
		return handle.promise().value;
	}
private:
	std::coroutine_handle <promise_type> handle;
};


auto make_window (VkInstance& instance, int width, int height, char const* title = "") noexcept -> std::tuple <GLFWwindow*, VkSurfaceKHR> {
	auto* window = glfwCreateWindow (width, height, title, nullptr, nullptr);
	auto surface = VkSurfaceKHR {};
	if (glfwCreateWindowSurface (instance, window, nullptr, &surface) != VK_SUCCESS) {
		std::cout << "error >> failed to create window surface" << std::endl;
		exit (-1);
	}
	return {window, surface};
}

template <typename T>
auto make () noexcept -> T;

template <>
auto make <vector<VkExtensionProperties>> () noexcept -> vector<VkExtensionProperties> {
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
nullptr);
vector<VkExtensionProperties> extensions {extensionCount};
// extensions.resize (extensionCount);
vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
extensions.data());
return extensions;
}

template <>
auto make <VkInstance> () noexcept -> VkInstance {
	auto layers = vector <char const*> {"VK_LAYER_KHRONOS_validation"};
	auto glfwExtensionCount = uint32_t {0};
	char const** glfwExtensions = glfwGetRequiredInstanceExtensions (&glfwExtensionCount);
	// std::cout << glfwExtensionCount << std::endl;
	auto create_info = VkInstanceCreateInfo {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.enabledLayerCount = static_cast <uint32_t> (layers.size()),
		.ppEnabledLayerNames = layers.data(),
		.enabledExtensionCount = glfwExtensionCount,
		.ppEnabledExtensionNames = glfwExtensions
	};
	auto instance = VkInstance {};
	if (vkCreateInstance (&create_info, nullptr, &instance) != VK_SUCCESS) {
		std::cout << "error >> failed to create instance" << std::endl;
		exit (-1);
	}
	return instance;
}

template <typename T>
struct iInstance {
	iInstance () noexcept : iInstance {{"VK_LAYER_KHRONOS_validation"}, necessary_instance_extensions ()} {

	}
	iInstance (vector <char const*> && layers, vector <char const*> && extensions) noexcept {
		VkApplicationInfo app_info {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = nullptr,
			.pApplicationName = "",
			.applicationVersion = VK_MAKE_API_VERSION (1, 0, 0, 0),
			.pEngineName = "",
			.engineVersion = VK_MAKE_API_VERSION (1, 0, 0, 0),
			.apiVersion = VK_MAKE_API_VERSION (1, 3, 0, 0)
		};

		auto const createInfo = VkInstanceCreateInfo {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = nullptr,
			.flags = MACOS ? VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR : 0,
			.pApplicationInfo = &app_info,
			.enabledLayerCount = static_cast <uint32_t> (layers.size()),
			.ppEnabledLayerNames = layers.data(),
			.enabledExtensionCount = static_cast <uint32_t> (extensions.size()),
			.ppEnabledExtensionNames = extensions.data()
		};
		

		if (auto res = vkCreateInstance (/*VkInstanceCreateInfo const * pCreateInfo */&createInfo, /*VkAllocationCallbacks const * pAllocator */nullptr, /*VkInstance* pInstance*/&handle); res != VK_SUCCESS) {
			switch (res)
			{
			case VK_ERROR_INCOMPATIBLE_DRIVER:
				std::cout << "VK_ERROR_INCOMPATIBLE_DRIVER" << std::endl;
				break;
			
			default:
				std::cout << "Unknown error" << std::endl;
				break;
			}
			std::cout << "error >> failed to create instance" << std::endl;
			exit (-1);
		}

		if constexpr (requires {static_cast <T*> (this) -> on_instance_finished (handle);}) {
			static_cast <T*> (this) -> on_instance_finished (handle);
		}
	}
	~iInstance () {
		std::cout << "~iInstance ()" << std::endl;
 		if (handle) {
			vkDestroyInstance (handle, nullptr); 
		}
	}
	iInstance (iInstance&& o) noexcept : handle {VK_NULL_HANDLE} {
		std::swap (handle, o.handle);
	}
	iInstance (iInstance const&) noexcept = delete;

	auto devices () const -> vector <VkPhysicalDevice> {
		auto count = uint32_t {0};
		vkEnumeratePhysicalDevices (handle, &count, nullptr);
		if (count > 0) {
			auto devices = vector <VkPhysicalDevice> {count};
			vkEnumeratePhysicalDevices (handle, &count, devices.data());
			return devices;
		} else {
			std::cout << "error >> failed to find any physical devices" << std::endl;
			exit (-1);
			// return vector <VkPhysicalDevice> {0};
		}
	}
	static auto available_extension_properties () noexcept -> vector <VkExtensionProperties> {
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		auto extensions = vector <VkExtensionProperties> {extensionCount};
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
		return extensions;
	}
	static auto necessary_instance_extensions () noexcept -> vector <char const*> {
		auto glfwExtensionCount = uint32_t {0};
		char const** glfwExtensions = glfwGetRequiredInstanceExtensions (&glfwExtensionCount);
		#ifdef MACOS
		auto res = vector <char const*> {glfwExtensionCount + 1};
		res [glfwExtensionCount] = "VK_KHR_portability_enumeration";
		#else 
		auto res = vector <char const*> {glfwExtensionCount};
		#endif
		for (auto i = 0; i < glfwExtensionCount; ++i) {
			res [i] = glfwExtensions [i];
		}
		return res;
	}

protected:
	auto instance () const noexcept -> VkInstance const& {
		return handle;
	}

private:
	// friend struct T;
	VkInstance handle;
};

template <typename T>
struct iPhysicalDevice : iInstance <iPhysicalDevice <T>> {
	// using iInstance::iInstance;
	~iPhysicalDevice () noexcept {
		// std::cout << "~iPhysicalDevice ()" << std::endl;
	}
protected:
	auto physical_device () const noexcept -> VkPhysicalDevice const& {
		return handle;
	}
private:
	template <typename U>
	friend struct iInstance;
	auto on_instance_finished (VkInstance const& i) noexcept -> void {
		// std::cout<<"mfd"<<std::endl;
		handle = iInstance <iPhysicalDevice <T>>::devices ().back ();
		// if constexpr (requires {static_cast <T*> (this) -> on_instance_finished (i);}) {
		// 	static_cast <T*> (this) -> on_instance_finished (i);
		// }
		if constexpr (requires {static_cast <T*> (this) -> on_physical_device_finished (handle);}) {
			static_cast <T*> (this) -> on_physical_device_finished (handle);
		}
	}
	
	// iInstance <iPhysicalDevice> instance;
	VkPhysicalDevice handle;
};

struct iDevice : iPhysicalDevice <iDevice> {
	~iDevice () noexcept {
		// std::cout << "~iDevice ()" << std::endl;
	}
protected:
	auto device () const noexcept -> VkDevice const& {
		return handle;
	}
private:
	template <typename U>
	friend struct iPhysicalDevice;
	auto on_physical_device_finished (VkPhysicalDevice const& p) noexcept -> void {

		// if constexpr (requires {static_cast <T*> (this) -> on_device_finished (handle);}) {
		// 	static_cast <T*> (this) -> on_device_finished (handle);
		// }
	}
	VkDevice handle;
};


auto run () -> vApp {
	std::cout << "run ..." << std::endl;
	// auto extensions = iInstance<iPhysicalDevice>::extension_properties ();
	// std::cout << extensions << std::endl;

	// auto glfwExtensionCount = uint32_t {0};
	// char const** glfwExtensions = glfwGetRequiredInstanceExtensions (&glfwExtensionCount);
	// for (auto i = 0; i < glfwExtensionCount; ++i) {
	// 	std::cout << glfwExtensions [i] << std::endl;
	// }
	// std::cout << glfwExtensionCount << std::endl;
	// #if defined (MACOS)
	auto ii = iDevice {};
	// #else 
	// auto ii = iPhysicalDevice {{"VK_LAYER_KHRONOS_validation"}, {"VK_KHR_portability_enumeration"}};
	// #en

	// std::cout << extensions << std::endl;
	// co_await 

	// auto instanceExtensions = make <vector<VkExtensionProperties>> ();
	// for (auto const& i : instanceExtensions) std::cout << i.extensionName << std::endl;
	// auto instance = make <VkInstance> ();
	// auto window = co_await make_window (640, 480);
	

	// auto surface = co_await make_surface (window, instance);
	

	// co_await std::suspend_always {};
	// auto instance = vInstance {{"VK_LAYER_KHRONOS_validation"}, {"VK_KHR_surface"}};
	// auto pDevice = vPhysicalDevice {instance.devices().back()};
	// auto lDevice = vLogicalDevice {pDevice}
	// auto window = glfwCreateWindow (640, 480, "window 1", nullptr, nullptr);
	// auto button = 
	// co_await is_clicked {make_rectangle {window, 10, 10}};
	// auto button = draw_rectangle {window};
	// co_await is_clicked {window};
	// co_await is_closed {window};

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

auto run_app () -> int {
	std::cout << "run_app ..." << std::endl;
	auto app = run ();
	// app.wait();
	while (app.windows_open ())
	{
		// std::cout << "yay" << std::endl;
		// std::cout << "hello world" << std::endl;
		app.draw ();
	}

	std::cout << "... run app" << std::endl;
	
	return EXIT_SUCCESS;
}

auto main (int argc, char** argv) -> int {
	// std::cout << "main ..." << std::endl;
	auto res = run_app ();
	// std::cout << "... main" << std::endl;
	return res;
	// auto instance = vInstance {{"VK_LAYER_KHRONOS_validation"}, {"VK_KHR_surface"}};
	// auto physicalDevice = vPhysicalDevice {instance};
	
	// std::cout << "main ..." << std::endl;
	
	// sync_wait_app (run ());

	// auto app = run ();
	// // app.wait();

	// while (app.open ())
	// {
	// 	// std::cout << "hello world" << std::endl;
	// 	app.draw ();
	// }
	
	

	// auto logicalDevice = vLogicalDevice {
		
	// 	physicalDevice};
	// auto instance = vInstance {{"VK_LAYER_KHRONOS_validation"}, {"VK_KHR_surface"}};
	
	// vector <int> baj;
	// VkInstance inst;
	// std::cout << "... main" << std::endl;
	// return 0;
}