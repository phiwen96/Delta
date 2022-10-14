module;
#include <stdio.h>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
// #include <ft2build.h>
// #include FT_FREETYPE_H
#include <thread>
#include <vector>
export module App;

// export import :Concept;
// import Graphics;
// import Window;

// template <typename...>
// struct Details;

// template <>
// struct Details <VkInstance> {
// 	std::vector <char const*> layers;
// 	std::vector <char const*> extensions;
// 	VkApplicationInfo app_info {
// 		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
// 		.pNext = nullptr,
// 		.pApplicationName = "",
// 		.applicationVersion = VK_MAKE_VERSION (1, 0, 0),
// 		.pEngineName = "",
// 		.engineVersion = VK_MAKE_VERSION (1, 0, 0),
// 		.apiVersion = VK_MAKE_VERSION (1, 3, 0)
// 	};

// 	auto operator () () noexcept -> VkInstance {
// 			// char const * const * layer_names = nullptr,
// 		// uint32_t layer_count = 0,
// 		// char const * const * extension_names = nullptr,
// 		// uint32_t extension_count = 0,
	

// 		auto const createInfo = VkInstanceCreateInfo {
// 			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
// 			.pNext = nullptr,
// 			 .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
// 			.pApplicationInfo = &app_info,
// 			.enabledLayerCount = static_cast <uint32_t> (layers.size()),
// 			.ppEnabledLayerNames = layers.data(),
// 			.enabledExtensionCount = static_cast <uint32_t> (extensions.size()),
// 			.ppEnabledExtensionNames = extensions.data()
// 		};

// 		auto instance = VkInstance {};

// 		if (vkCreateInstance (/*VkInstanceCreateInfo const * pCreateInfo */&createInfo, /*VkAllocationCallbacks const * pAllocator */nullptr, /*VkInstance* pInstance*/&instance) != VK_SUCCESS) {
// 			printf ("error >> failed to create instance");
// 			exit (-1);
// 		}

// 		return instance;
// 	}
// };

// export template <typename T>
// concept App = // Coro <T> and 
// requires (T app) {
// 	true;
// };
// // import Graphics;
// // export import :Type;

// export struct app {
// 	auto windows_open () -> int {
// 		auto tot = 0;
// 		for (auto& w : windows) {
// 			if (not glfwWindowShouldClose (w.handle)) ++tot;
// 		}
// 		return tot;
// 	}
// 	app (auto&& title) noexcept{
// 		constexpr auto width = 640;
// 		constexpr auto height = 480;
// 		glfwInit ();
// 		glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
// 	#if defined (MACOS)
// 		glfwWindowHint (GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_NO_API);
// 	#endif

// 		VkApplicationInfo app_info {
// 			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
// 			.pNext = nullptr,
// 			.pApplicationName = "",
// 			.applicationVersion = VK_MAKE_VERSION (1, 0, 0),
// 			.pEngineName = "",
// 			.engineVersion = VK_MAKE_VERSION (1, 0, 0),
// 			.apiVersion = VK_MAKE_VERSION (1, 3, 0)
// 		};
// 		auto layers = std::vector <char const*> {"VK_LAYER_KHRONOS_validation"/*, "VK_LAYER_LUNARG_api_dump"*/};
// 		auto extensions = std::vector <char const*> {"VK_KHR_portability_enumeration", "VK_KHR_surface", "VK_EXT_metal_surface", "VK_MVK_macos_surface", "VK_KHR_get_physical_device_properties2"};

// 		auto const createInfo = VkInstanceCreateInfo {
// 			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
// 			.pNext = nullptr,
// 			 .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
// 			.pApplicationInfo = &app_info,
// 			.enabledLayerCount = static_cast <uint32_t> (layers.size()),
// 			.ppEnabledLayerNames = layers.data(),
// 			.enabledExtensionCount = static_cast <uint32_t> (extensions.size()),
// 			.ppEnabledExtensionNames = extensions.data()
// 		};

// 		auto instance = VkInstance {};

// 		if (vkCreateInstance (/*VkInstanceCreateInfo const * pCreateInfo */&createInfo, /*VkAllocationCallbacks const * pAllocator */nullptr, /*VkInstance* pInstance*/&instance) != VK_SUCCESS) {
// 			printf ("error >> failed to create instance");
// 			exit (-1);
// 		}

// 		auto count = uint32_t {0};
// 		vkEnumeratePhysicalDevices (instance, &count, nullptr); 
// 		if (count == 0) { 
// 			printf ("failed to find a physical device");
// 			exit (-1);
// 		}
// 		auto devices = std::vector <VkPhysicalDevice> {count};
// 		vkEnumeratePhysicalDevices (instance, &count, devices.data());
// 		// printf ("yoyoyo\n");



// 		// auto const instance = Details <VkInstance> {
// 		// 	.layers = {"VK_LAYER_KHRONOS_validation"/*, "VK_LAYER_LUNARG_api_dump"*/},
// 		// 	.extensions = {"VK_KHR_portability_enumeration", "VK_KHR_surface", "VK_EXT_metal_surface", "VK_MVK_macos_surface", "VK_KHR_get_physical_device_properties2"}
// 		// } ();
		
// 		// auto * window = glfwCreateWindow (width, height, "", nullptr, nullptr);
// 	}
// 	auto run () noexcept -> bool{
	
// 		// auto * window2 = glfwCreateWindow (width, height, "", nullptr, nullptr);
// 		auto keepRunning = true;
// 		// std::thread {[&] {
// 		while (keepRunning) {
// 			glfwPollEvents ();
// 		}
// 			// while ((not glfwWindowShouldClose (window)) or (not glfwWindowShouldClose (window2))) {
// 				// glfwPollEvents ();
// 				/* code */
// 			// }
// 		// }}.join ();
		
// 		return true;
// 	}
	
// 	auto new_window (auto && title) noexcept -> window & {
// 		windows.push_back (window {640, 480, title});
// 		return windows.back ();
// 	}
// 	auto render () noexcept -> void {

// 	}
// 	auto process () noexcept -> void {
// 		glfwPollEvents ();
// 	}
// 	~app () {
// 		glfwTerminate ();
// 	}
// private:
// 	std::vector <window> windows;
// };


