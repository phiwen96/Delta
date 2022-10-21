// #include <iostream>
// #include <semaphore>
// #include <coroutine>
// #include <utility>
// #include <GLFW/glfw3.h>
// import App; 
// import Window;
#pragma GCC optimize("O2")
#include <coroutine>
#include <utility>
// #include <iostream>
#include <cstdlib> // exit
#include <array>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <tuple>
#include <type_traits>
#include <string>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <ft2build.h>
#include FT_FREETYPE_H

import Vector;
import Array;

// import Concepts.Bool;
// import Window; 
// import App;
// import Coro;
// import Graphics;

// import Graphics.Window;
// import Concepts.Bool;


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


template <typename T>
concept Window = requires (T window, int i) {
	i = window.open ();
	{window.hide ()} noexcept;
	{window.show ()} noexcept;
	{window.draw_frame ()} noexcept;
};


struct window_coro {
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

struct /*[[nodiscard]]*/ future_io {
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

auto get_surface_capabilities (VkPhysicalDevice const & device, VkSurfaceKHR const & surface) noexcept -> VkSurfaceCapabilitiesKHR {
	
	auto capabilities = VkSurfaceCapabilitiesKHR {};

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR (device, surface, &capabilities);

	return capabilities;
}

auto get_devices (
	VkInstance const & instance ) noexcept -> std::vector <VkPhysicalDevice> {
		// std::cout << "yo" << std::endl;
	auto count = uint32_t {0};
	vkEnumeratePhysicalDevices (instance, &count, nullptr);
	if (count > 0) {
		auto devices = std::vector <VkPhysicalDevice> {count};
		vkEnumeratePhysicalDevices (instance, &count, devices.data());
		return devices;
	} else {
		std::cout << "error >> failed to find any physical devices" << std::endl;
		exit (-1);
		// return std::vector <VkPhysicalDevice> {0};
	}
}

auto get_properties (
	VkPhysicalDevice const& device) noexcept -> VkPhysicalDeviceProperties {
	auto properties = VkPhysicalDeviceProperties {};
	vkGetPhysicalDeviceProperties (device, &properties);
	return properties;
}

auto get_features (
	VkPhysicalDevice const& device) noexcept -> VkPhysicalDeviceFeatures {
	auto features = VkPhysicalDeviceFeatures {};
	vkGetPhysicalDeviceFeatures (device, &features);
	return features;
}

auto get_memory_properties (
	VkPhysicalDevice const& device) noexcept -> VkPhysicalDeviceMemoryProperties {
	auto properties = VkPhysicalDeviceMemoryProperties {};
	vkGetPhysicalDeviceMemoryProperties (device, &properties);
	return properties;
}

auto get_queue_family_properties (
	VkPhysicalDevice const& device) noexcept -> std::vector <VkQueueFamilyProperties> {
	auto count = uint32_t {0};
	vkGetPhysicalDeviceQueueFamilyProperties (device, &count, nullptr);
	auto queue_family_properties = std::vector <VkQueueFamilyProperties> {count};
	vkGetPhysicalDeviceQueueFamilyProperties (device, &count, queue_family_properties.data());
	return queue_family_properties;
}

auto make_shader_module (std::vector <char> && code, VkDevice const & device) noexcept -> VkShaderModule {
	auto create_info = VkShaderModuleCreateInfo {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = code.size (),
		.pCode = reinterpret_cast <uint32_t const *> (code.data ())
	};

	auto shader_module = VkShaderModule {};	

	if (vkCreateShaderModule (device, &create_info, nullptr, &shader_module) != VK_SUCCESS) {
		std::cout << "error >> failed to create shader module" << std::endl;
		exit (-1);
	}

	return shader_module;
}

auto read_byte_file (char const * path) noexcept -> std::vector <char> {
	// auto file = std::ifstream {path, std::ios::ate | std::ios::binary};

	// if (!file.is_open ()) {
	// 	std::cout << "error >> failed to open file" << std::endl;
	// 	exit (-1);
	// }

	// auto file_size = file.tellg ();
	// auto buffer = std::vector <char> {};
	// buffer.resize (file_size);
	// file.seekg (0);
	// file.read (buffer.data (), file_size);
	// file.close ();
	
	auto * fileptr = fopen(path, "rb");
	if (not fileptr) {
		printf ("error >> failed to read file\nfile >> %s\n", path);
		exit (-1);
	}
	fseek(fileptr, 0, SEEK_END);  
	
	auto filelen = ftell(fileptr);
	rewind(fileptr);     
	auto buffer = std::vector <char> {}; 
	buffer.resize (filelen);
	fread(buffer.data(), filelen, 1, fileptr);
	fclose(fileptr);
	return buffer;
}

auto make_shader_module (char const * path, VkDevice const & device) noexcept -> VkShaderModule {
	return make_shader_module (read_byte_file (path), device);
}

template <typename... T> 
struct Details;


struct Instance {
	// template <int I, auto J>
	Instance (auto && layers, auto && extensions) {
		VkApplicationInfo app_info {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = nullptr,
			.pApplicationName = "",
			.applicationVersion = VK_MAKE_VERSION (1, 0, 0),
			.pEngineName = "",
			.engineVersion = VK_MAKE_VERSION (1, 0, 0),
			.apiVersion = VK_MAKE_VERSION (1, 3, 0)
		};

		auto const createInfo = VkInstanceCreateInfo {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = nullptr,
			 .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
			.pApplicationInfo = &app_info,
			.enabledLayerCount = static_cast <uint32_t> (layers.size()),
			.ppEnabledLayerNames = layers.data(),
			.enabledExtensionCount = static_cast <uint32_t> (extensions.size()),
			.ppEnabledExtensionNames = extensions.data()
		};

		auto instance = VkInstance {};

		if (vkCreateInstance (/*VkInstanceCreateInfo const * pCreateInfo */&createInfo, /*VkAllocationCallbacks const * pAllocator */nullptr, /*VkInstance* pInstance*/&instance) != VK_SUCCESS) {
			std::cout << "error >> failed to create instance" << std::endl;
			exit (-1);
		}
	}
private:
	VkInstance handle;
};

template <>
struct Details <VkInstance> {
	vector <char const*> layers;
	vector <char const*> extensions;
	VkApplicationInfo app_info {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = nullptr,
		.pApplicationName = "",
		.applicationVersion = VK_MAKE_VERSION (1, 0, 0),
		.pEngineName = "",
		.engineVersion = VK_MAKE_VERSION (1, 0, 0),
		.apiVersion = VK_MAKE_VERSION (1, 3, 0)
	};

	auto operator () () noexcept -> VkInstance {
			// char const * const * layer_names = nullptr,
		// uint32_t layer_count = 0,
		// char const * const * extension_names = nullptr,
		// uint32_t extension_count = 0,
	

		auto const createInfo = VkInstanceCreateInfo {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = nullptr,
			 .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
			.pApplicationInfo = &app_info,
			.enabledLayerCount = static_cast <uint32_t> (layers.size()),
			.ppEnabledLayerNames = layers.data(),
			.enabledExtensionCount = static_cast <uint32_t> (extensions.size()),
			.ppEnabledExtensionNames = extensions.data()
		};

		auto instance = VkInstance {};

		if (vkCreateInstance (/*VkInstanceCreateInfo const * pCreateInfo */&createInfo, /*VkAllocationCallbacks const * pAllocator */nullptr, /*VkInstance* pInstance*/&instance) != VK_SUCCESS) {
			std::cout << "error >> failed to create instance" << std::endl;
			exit (-1);
		}

		return instance;
	}
};


template <>
struct Details <VkSurfaceKHR> {
	VkInstance const & instance;
	GLFWwindow * & window;

	auto operator () () noexcept -> VkSurfaceKHR {
	
		auto surface = VkSurfaceKHR {};

		if (glfwCreateWindowSurface (instance, window, nullptr, &surface) != VK_SUCCESS) {
			std::cout << "error >> failed to create window surface" << std::endl;
			exit (-1);
		}

		return surface;
	}
};

struct PhysicalDevice {

	VkPhysicalDevice handle;

	auto get_memory_type_index (VkDeviceSize const& size, VkMemoryPropertyFlags const& flags) const noexcept -> uint32_t {

		auto const properties = get_memory_properties ();

		for (auto i = 0; i < properties.memoryTypeCount; ++i) {
			if ((properties.memoryTypes[i].propertyFlags & flags) && (size < properties.memoryHeaps [properties.memoryTypes[i].heapIndex].size)) {
				return static_cast <uint32_t> (i);
			}
		}
		std::cout << "error >> failed to find memeory type index" << std::endl;
		exit (-1);
	}

	auto get_memory_properties () const noexcept -> VkPhysicalDeviceMemoryProperties {
		VkPhysicalDeviceMemoryProperties mem_properties;
		vkGetPhysicalDeviceMemoryProperties (handle, &mem_properties);
		return mem_properties;
	}

	auto get_extension_properties () const noexcept -> std::vector <VkExtensionProperties> {
		auto count = uint32_t {0};
		vkEnumerateDeviceExtensionProperties (handle, nullptr, &count, nullptr);
		auto extension_properties = std::vector <VkExtensionProperties> {count};
		vkEnumerateDeviceExtensionProperties (handle, nullptr, &count, extension_properties.data());
		return extension_properties;
	}

	auto get_queue_family_properties () const noexcept -> std::vector <VkQueueFamilyProperties> {
		auto count = uint32_t {0};
		vkGetPhysicalDeviceQueueFamilyProperties (handle, &count, nullptr);
		auto queue_family_properties = std::vector <VkQueueFamilyProperties> {count};
		vkGetPhysicalDeviceQueueFamilyProperties (handle, &count, queue_family_properties.data());
		return queue_family_properties;
	}

	auto get_surface_support (uint32_t queue_family_index, VkSurfaceKHR const & surface) const noexcept -> VkBool32 {
		auto result = VkBool32 {};
		vkGetPhysicalDeviceSurfaceSupportKHR (handle, queue_family_index, surface, &result);
		return result;
	}

	auto get_properties () const noexcept -> VkPhysicalDeviceProperties {
		auto properties = VkPhysicalDeviceProperties {};
		vkGetPhysicalDeviceProperties (handle, &properties);
		return properties;
	}

	auto get_features () const noexcept -> VkPhysicalDeviceFeatures {
		auto features = VkPhysicalDeviceFeatures {};
		vkGetPhysicalDeviceFeatures (handle, &features);
		return features;
	}
};

template <>
struct Details <PhysicalDevice> {
	VkInstance const & instance;
	VkPhysicalDevice (*picker) (std::vector <VkPhysicalDevice> const &);

	auto operator () () noexcept -> PhysicalDevice {
		return {picker (get_devices (instance))};
	}
};

struct CommandBuffer {
	VkCommandBuffer handle;

	auto begin (VkCommandBufferUsageFlags const usage_flags = 0, VkCommandBufferInheritanceInfo const * inheritence_info = nullptr) const noexcept -> void {

		auto const begin_info = VkCommandBufferBeginInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = usage_flags,
			.pInheritanceInfo = inheritence_info
		};

		if (vkBeginCommandBuffer (handle, &begin_info) != VK_SUCCESS) {
			std::cout << "error >> failed to begin recording command buffer" << std::endl;
			exit (-1);
		}
	}
	auto end () const noexcept -> void {
		if (vkEndCommandBuffer (handle) != VK_SUCCESS) {
			std::cout << "error >> failed to end recording command buffer" << std::endl;
			exit (-1);
		}
	}
	auto record (auto && lambda, uint32_t const image_index, VkCommandBufferUsageFlags const usage_flags = 0, VkCommandBufferInheritanceInfo const * inheritence_info = nullptr) const noexcept -> void {

		auto const begin_info = VkCommandBufferBeginInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = usage_flags,
			.pInheritanceInfo = inheritence_info
		};

		if (vkBeginCommandBuffer (handle, &begin_info) != VK_SUCCESS) {
			std::cout << "error >> failed to begin recording command buffer" << std::endl;
			exit (-1);
		}
		
		lambda (image_index);

		if (vkEndCommandBuffer (handle) != VK_SUCCESS) {
			std::cout << "error >> failed to end recording command buffer" << std::endl;
			exit (-1);
		}
	}
	auto reset (VkCommandBufferResetFlags flags = 0) noexcept -> CommandBuffer& {
		vkResetCommandBuffer (handle, flags);
		return *this;
	}

	//////////////////////////////////
	///	Commands
	/////////////////////////////////
	auto copy_buffer (VkBuffer const & src, VkBuffer const & dst, std::vector <VkBufferCopy> const & regions) const noexcept -> void {
		vkCmdCopyBuffer (handle, src, dst, (uint32_t) regions.size(), regions.data());}
	auto set_viewport (VkViewport const & viewport) const noexcept -> void {
		vkCmdSetViewport (handle, 0, 1, &viewport);}
	auto set_scissor (VkRect2D const & scissor) const noexcept -> void {
		vkCmdSetScissor (handle, 0, 1, &scissor);}
	auto set_line_width (float const width) const noexcept -> void {
		vkCmdSetLineWidth (handle, width);}
	// auto push_constants (VkPipelineLayout const & layout, VkShaderStageFlags const & flags)

	
};

struct CommandPool {
	VkCommandPool handle;
	
	auto make_command_buffers (VkCommandBufferLevel const level, uint32_t const count, VkDevice const & device) const noexcept -> std::vector <CommandBuffer> {

		auto command_buffer_handles = std::vector <VkCommandBuffer> {count};

		auto const allocate_info = VkCommandBufferAllocateInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = handle,
			.level = level,
			.commandBufferCount = count,
		};

		if (vkAllocateCommandBuffers (device, &allocate_info, command_buffer_handles.data ()) != VK_SUCCESS) {
			std::cout << "error >> failed to create command buffers" << std::endl;
			exit (-1);
		}

		auto command_buffers = std::vector <CommandBuffer> {count};

		for (auto i = 0; i < command_buffers.size (); ++i) {
			std::swap (command_buffers[i].handle, command_buffer_handles[i]);
		}

		return command_buffers;
	}
	auto make_command_buffer (VkCommandBufferLevel const level, VkDevice const & device) const noexcept -> CommandBuffer {

		auto command_buffer = CommandBuffer {};

		auto const allocate_info = VkCommandBufferAllocateInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
			.commandPool = handle,
			.level = level,
			.commandBufferCount = 1,
		};

		if (vkAllocateCommandBuffers (device, &allocate_info, &command_buffer.handle) != VK_SUCCESS) {
			std::cout << "error >> failed to create command buffers" << std::endl;
			exit (-1);
		}

		return command_buffer;
	}
};

struct Queue {
	VkQueue handle;
	
	auto present (VkPresentInfoKHR const & info) noexcept -> void {
		vkQueuePresentKHR (handle, &info);}
	auto submit (VkSubmitInfo const & submits, VkFence const & fence = VK_NULL_HANDLE) const noexcept -> void {
		if (vkQueueSubmit (handle, 1, &submits, fence) != VK_SUCCESS) {
			std::cout << "error >> failed to submit queue" << std::endl;
			exit (-1);
		}}
	auto submit (std::vector <VkSubmitInfo> const & submits, VkFence const & fence = VK_NULL_HANDLE) const noexcept -> void {
		if (vkQueueSubmit (handle, (uint32_t) submits.size (), submits.data (), fence) != VK_SUCCESS) {
			std::cout << "error >> failed to submit queue" << std::endl;
			exit (-1);
		}}
	auto wait_idle () const noexcept -> void {
		vkQueueWaitIdle (handle);
	}
};

struct QueueFamily {
	std::vector <Queue> handles;
	CommandPool command_pool;
	uint32_t index;
	VkQueueFlags capabilities;
	VkBool32 present_support;

	auto supports_graphics_operations () const noexcept -> bool {
		return capabilities & VK_QUEUE_GRAPHICS_BIT;
	}
	auto supports_compute_operations () const noexcept -> bool {
		return capabilities & VK_QUEUE_COMPUTE_BIT;
	}
	auto supports_transfer_operations () const noexcept -> bool {
		return capabilities & VK_QUEUE_TRANSFER_BIT;
	}
	auto supports_memory_binding_operations () const noexcept -> bool {
		return capabilities & VK_QUEUE_SPARSE_BINDING_BIT;
	}
};

struct LogicalDevice {
	PhysicalDevice physical_device;
	VkDevice handle;
	std::vector <QueueFamily> queue_families;

	auto wait_idle () const noexcept -> void {
		vkDeviceWaitIdle (handle);}
	auto wait_for_fence (VkFence const & fence, uint64_t timeout = UINT64_MAX) const noexcept -> void {
		vkWaitForFences (handle, 1, &fence, VK_TRUE, timeout);}
	auto reset_fence (VkFence const & fence) const noexcept -> void {
		vkResetFences (handle, 1, &fence);}	
	
	auto make_command_buffer (VkQueueFlags const queue_capabilities, VkCommandBufferLevel const level) const noexcept -> decltype (auto) {
		for (auto const & queue_family : queue_families) {
			if (queue_family.capabilities & queue_capabilities) {
				return queue_family.command_pool.make_command_buffer (level, handle);
			}
		}

		std::cout << "error >> failed to create command buffer, could not find queue family with those capabilities" << std::endl;
		exit (-1);
	}
	auto make_graphics_command_buffer (VkCommandBufferLevel const level) const noexcept -> decltype (auto) {
		for (auto const & queue_family : queue_families) {
			if (queue_family.supports_graphics_operations ()) {
				return queue_family.command_pool.make_command_buffer (level, handle);
			}
		}

		std::cout << "error >> failed to create command buffer, could not find queue family with support for graphics operations" << std::endl;
		exit (-1);
	}
	auto make_compute_command_buffer (VkCommandBufferLevel const level) const noexcept -> decltype (auto) {
		for (auto const & queue_family : queue_families) {
			if (queue_family.supports_compute_operations ()) {
				return queue_family.command_pool.make_command_buffer (level, handle);
			}
		}

		std::cout << "error >> failed to create command buffer, could not find queue family with support for compute operations" << std::endl;
		exit (-1);
	}
	auto make_tranfer_command_buffer (VkCommandBufferLevel const level) const noexcept -> decltype (auto) {
		for (auto const & queue_family : queue_families) {
			if (queue_family.supports_transfer_operations ()) {
				return queue_family.command_pool.make_command_buffer (level, handle);
			}
		}

		std::cout << "error >> failed to create command buffer, could not find queue family with support for transfer operations" << std::endl;
		exit (-1);
	}
	auto make_memory_binding_command_buffer (VkCommandBufferLevel const level) const noexcept -> decltype (auto) {
		for (auto const & queue_family : queue_families) {
			if (queue_family.supports_memory_binding_operations ()) {
				return queue_family.command_pool.make_command_buffer (level, handle);
			}
		}

		std::cout << "error >> failed to create command buffer, could not find queue family with support for memory binding operations" << std::endl;
		exit (-1);
	}
	auto make_semaphore () const noexcept -> VkSemaphore {
		auto const create_info = VkSemaphoreCreateInfo {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
		};
		auto semaphore = VkSemaphore {};
		if (vkCreateSemaphore (handle, &create_info, nullptr, &semaphore) != VK_SUCCESS) {
			std::cout << "error >> failed to create semaphore" << std::endl;
			exit (-1);
		}
		return semaphore;
	}
};

template <>
struct Details <VkSemaphore> {
	VkDevice const & device;

	auto operator () () const noexcept -> VkSemaphore {
		auto const create_info = VkSemaphoreCreateInfo {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
		};
		auto semaphore = VkSemaphore {};
		if (vkCreateSemaphore (device, &create_info, nullptr, &semaphore) != VK_SUCCESS) {
			std::cout << "error >> failed to create semaphore" << std::endl;
			exit (-1);
		}
		return semaphore;
	}
};

template <>
struct Details <std::vector <VkSemaphore>> {
	VkDevice const & device;
	size_t count;

	auto operator () () const noexcept -> std::vector <VkSemaphore> {
		auto semaphores = std::vector <VkSemaphore> {count};
		auto const create_info = VkSemaphoreCreateInfo {
			.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
		};
		for (auto & semaphore : semaphores) {
			if (vkCreateSemaphore (device, &create_info, nullptr, &semaphore) != VK_SUCCESS) {
				std::cout << "error >> failed to create semaphore" << std::endl;
				exit (-1);
			}
		}
		
		return semaphores;
	}
};

template <>
struct Details <VkFence> {
	VkDevice const & device;
	VkFenceCreateFlags flags; // VK_FENCE_CREATE_SIGNALED_BIT

	auto operator () () const noexcept -> VkFence {
		auto const create_info = VkFenceCreateInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = flags
		};
		auto fence = VkFence {};
		if (vkCreateFence (device, &create_info, nullptr, &fence) != VK_SUCCESS) {
			std::cout << "error >> failed to create fence" << std::endl;
			exit (-1);
		}
		return fence;
	}
};

template <>
struct Details <std::vector <VkFence>> {
	VkDevice const & device;
	VkFenceCreateFlags flags; // VK_FENCE_CREATE_SIGNALED_BIT
	size_t count;

	auto operator () () const noexcept -> std::vector <VkFence> {
		auto const create_info = VkFenceCreateInfo {
			.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
			.flags = flags
		};

		auto fences = std::vector <VkFence> {count};
		for (auto & fence : fences) {
			if (vkCreateFence (device, &create_info, nullptr, &fence) != VK_SUCCESS) {
				std::cout << "error >> failed to create fence" << std::endl;
				exit (-1);
			}
		}
		
		return fences;
	}
};

template <>
struct Details <LogicalDevice> {
	PhysicalDevice & physical_device;
	VkSurfaceKHR const & surface;
	std::vector <char const*> const layers;
	std::vector <char const*> const extensions;
	VkPhysicalDeviceFeatures const features;

	auto operator () () noexcept -> LogicalDevice {

		auto const queue_family_properties = physical_device.get_queue_family_properties ();
		auto queue_create_infos = std::vector <VkDeviceQueueCreateInfo> {queue_family_properties.size()};
		// auto queues = std::vector <Queue> {};
		auto indices = std::vector <uint32_t> {};

		auto queue_families = std::vector <QueueFamily> {queue_family_properties.size()};

		auto const priority = float {1.0};

		for (auto i = 0; i < queue_family_properties.size (); ++i) {

			auto const count = queue_family_properties [i].queueCount;
			auto const family_index = static_cast <uint32_t> (i);
			auto const capabilities = queue_family_properties [i].queueFlags;

			queue_families [i].handles.resize (count);
			queue_families [i].index = family_index;
			queue_families [i].capabilities = capabilities;

			queue_create_infos [i] = {
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.queueFamilyIndex = family_index,
				.queueCount = count,
				.pQueuePriorities = &priority
			};

			// auto const old_size = queues.size ();
			// queues.resize (old_size + count);
			// indices.resize (old_size + count);

			// std::iota (indices.begin () + old_size, indices.end (), 0);
			
			// for (auto j = old_size; j < queues.size (); ++j) {
			// 	queues [j] = {
			// 		.family = family_index,
			// 		.capabilities = capabilities
			// 	};
			// }
		}

		constexpr auto dynamic_rendering_features = VkPhysicalDeviceDynamicRenderingFeaturesKHR {
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
			.dynamicRendering = VK_TRUE
		};

		auto const createInfo = VkDeviceCreateInfo {
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext = &dynamic_rendering_features,
			.flags = 0,
			.queueCreateInfoCount = static_cast <uint32_t> (queue_create_infos.size()),
			.pQueueCreateInfos = queue_create_infos.data(),
			.enabledLayerCount = static_cast <uint32_t> (layers.size()),
			.ppEnabledLayerNames = layers.data(),
			.enabledExtensionCount = static_cast <uint32_t> (extensions.size()),
			.ppEnabledExtensionNames = extensions.data(),
			.pEnabledFeatures = &features
		};

		auto device = VkDevice {};
		
		if (vkCreateDevice (physical_device.handle, &createInfo, nullptr, &device) != VK_SUCCESS) {
			std::cout << "error >> failed to create device" << std::endl;
			exit (-1);
		}

		for (auto & queue_family : queue_families) {
			for (auto i = 0; i < queue_family.handles.size (); ++i) {
				vkGetDeviceQueue (device, queue_family.index, static_cast <uint32_t> (i), &queue_family.handles [i].handle);
				queue_family.present_support = physical_device.get_surface_support (queue_family.index, surface);

				auto const command_pool_create_info = VkCommandPoolCreateInfo {
					.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
					.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
					.queueFamilyIndex = queue_family.index
				};

				if (vkCreateCommandPool(device, &command_pool_create_info, nullptr, &queue_family.command_pool.handle) != VK_SUCCESS) {
					std::cout << "error >> failed to create command pool" << std::endl;
					exit (-1);
				}
			}
		}

		// for (auto i = 0; i < queues.size (); ++i) {
		// 	vkGetDeviceQueue (device, queues [i].family, indices [i], &queues [i].handle);
		// }

		return {physical_device, device, queue_families};
	}
};

auto get_window_extent (GLFWwindow * window) noexcept -> VkExtent2D {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	return {
		.width = static_cast <uint32_t> (width),
		.height = static_cast <uint32_t> (height)
	};
}


struct Swapchain {
	VkDevice device;
	VkSwapchainKHR handle;
	std::vector <VkImage> images;
	std::vector <VkImageView> views;
	VkExtent2D image_extent;
	std::vector <VkFramebuffer> framebuffers;

	auto acquire_next_image (VkSemaphore const & s = VK_NULL_HANDLE, VkFence const & f = VK_NULL_HANDLE) const noexcept -> uint32_t {
		uint32_t index;
		vkAcquireNextImageKHR (device, handle, UINT64_MAX, s, f, &index);
		return index;
	}

	auto destroy () noexcept -> void {
		for (auto i = 0; i < framebuffers.size (); ++i) {
			vkDestroyFramebuffer (device, framebuffers [i], nullptr);
			vkDestroyImageView (device, views [i], nullptr);
		}

		vkDestroySwapchainKHR (device, handle, nullptr);
	}
};

template <>
struct Details <Swapchain> {
	VkDevice const & device;
	VkSurfaceKHR const & surface;
	VkRenderPass const & render_pass;
	uint32_t image_count;
	VkFormat image_format;
	VkColorSpaceKHR image_color_space;
	VkExtent2D image_extent;
	VkImageUsageFlags image_usage;
	VkSharingMode image_sharing_mode;
	VkSurfaceTransformFlagBitsKHR pre_transform;
	VkCompositeAlphaFlagBitsKHR composite_alpha;
	VkPresentModeKHR present_mode;
	VkBool32 clipped;
	VkSwapchainKHR old_swapchain = VK_NULL_HANDLE;
	VkImageViewType const view_type;
	VkComponentMapping view_components;
	VkImageSubresourceRange view_subresource_range;

	auto operator () () noexcept -> Swapchain {

		auto const create_info = VkSwapchainCreateInfoKHR {
			.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			.surface = surface,
			.minImageCount = image_count,
			.imageFormat = image_format,
			.imageColorSpace = image_color_space,
			.imageExtent = image_extent,
			.imageArrayLayers = 1,
			.imageUsage = image_usage,
			.imageSharingMode = image_sharing_mode,
			.queueFamilyIndexCount = 0,
			.pQueueFamilyIndices = nullptr,
			.preTransform = pre_transform,
			.compositeAlpha = composite_alpha,
			.presentMode = present_mode,
			.clipped = clipped,
			.oldSwapchain = old_swapchain
		};

		auto swapchain = VkSwapchainKHR {};

		if (vkCreateSwapchainKHR (device, &create_info, nullptr, &swapchain) != VK_SUCCESS) {
			std::cout << "error >> failed to create swapchain" << std::endl;
			exit (-1);
		}

		auto count = uint32_t {};
		vkGetSwapchainImagesKHR (device, swapchain, &count, nullptr);
		auto images = std::vector <VkImage> {count};
		vkGetSwapchainImagesKHR (device, swapchain, &count, images.data ());

		auto view_create_info = VkImageViewCreateInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.viewType = view_type,
			.format = image_format,
			.components = view_components,
			.subresourceRange = view_subresource_range
		};

		auto views = std::vector <VkImageView> {images.size ()};
		auto frame_buffers = std::vector <VkFramebuffer> {views.size ()};

		for (auto i = 0; i < images.size (); ++i) {

			view_create_info.image = images [i];

			if (vkCreateImageView (device, &view_create_info, nullptr, & views [i]) != VK_SUCCESS) {
				std::cout << "error >> failed to create image view" << std::endl;
				exit (-1);
			}

			VkImageView attachments [] = {
				views [i]
			};

			auto const framebuffer_create_info = VkFramebufferCreateInfo {
				.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
				.renderPass = render_pass,
				.attachmentCount = 1,
				.pAttachments = attachments,
				.width = image_extent.width,
				.height = image_extent.height,
				.layers = 1
			};

			if (vkCreateFramebuffer (device, &framebuffer_create_info, nullptr, &frame_buffers [i]) != VK_SUCCESS) {
				std::cout << "error >> failed to create framebuffers" << std::endl;
				exit (-1);
			}
		}

		return {device, swapchain, images, views, image_extent, frame_buffers};
	}
	auto recreate (Swapchain & s) noexcept -> void {
		// s.destroy ();
		old_swapchain = s.handle;
		auto new_s = this->operator()();
		s.destroy ();
		std::swap (s.handle, new_s.handle);
		std::swap (s.images, new_s.images);
		std::swap (s.views, new_s.views);
		std::swap (s.image_extent, new_s.image_extent);
		std::swap (s.framebuffers, new_s.framebuffers);
	}
};

struct RenderPass {
	VkDevice device;
	VkRenderPass handle;

	auto destroy () noexcept -> void {
		vkDestroyRenderPass (device, handle, nullptr);
	}
};

template <>
struct Details <RenderPass> {
	VkDevice device;
	std::vector <VkAttachmentDescription> attachment_descriptions;
	std::vector <VkAttachmentReference> attachment_references;
	std::vector <VkSubpassDescription> subpass_descriptions;
	std::vector <VkSubpassDependency> subpass_dependencies;

	auto operator () () noexcept -> RenderPass {
		auto const create_info = VkRenderPassCreateInfo {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
			.attachmentCount = (uint32_t) attachment_descriptions.size (),
			.pAttachments = attachment_descriptions.data (),
			.subpassCount = (uint32_t) subpass_descriptions.size (),
			.pSubpasses = subpass_descriptions.data (),
			.dependencyCount = (uint32_t) subpass_dependencies.size (),
			.pDependencies = subpass_dependencies.data ()
		};

		auto render_pass = VkRenderPass {};

		if (vkCreateRenderPass (device, &create_info, nullptr, &render_pass) != VK_SUCCESS) {
			std::cout << "error >> failed to create render pass" << std::endl;
			exit (-1);
		}

		return {device, render_pass};
	}
};

struct GraphicsPipeline {
	LogicalDevice device;
	VkPipeline handle;
	VkDescriptorSetLayout descriptor_set_layout;
	VkPipelineLayout layout;

	auto destroy () noexcept -> void {
		vkDestroyDescriptorSetLayout (device.handle, descriptor_set_layout, nullptr);
		vkDestroyPipelineLayout (device.handle, layout, nullptr);
		vkDestroyPipeline (device.handle, handle, nullptr);
	}
};

struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 tex_coord;
	glm::vec2 as_texture;
};

struct LineVertex {
	glm::vec3 pos;
	glm::vec3 color;
};

struct mvp_push_constants {
	alignas (16) glm::mat4 model;
	alignas (16) glm::mat4 view;
	alignas (16) glm::mat4 proj;
};

struct texture_push_constants {
	int imageIdx;
};
template <>
struct Details <GraphicsPipeline> {
	LogicalDevice const & device;
	VkRenderPass const & render_pass;
	VkFormat image_format;
	std::vector <std::tuple <char const*, VkShaderStageFlagBits>> shaders;
	// std::vector <VkPipelineShaderStageCreateInfo> shaders;

	// VkPipelineVertexInputStateCreateInfo vertex_input;
	std::vector <VkVertexInputBindingDescription> vertex_binding_descriptions;
	std::vector <VkVertexInputAttributeDescription> vertex_attribute_descriptions;
	VkPipelineInputAssemblyStateCreateInfo input_assembly;
	std::vector <VkViewport> viewports;
	std::vector <VkRect2D> scissors;
	VkPipelineRasterizationStateCreateInfo rasterization;
	VkPipelineMultisampleStateCreateInfo multisample;
	std::vector <VkPipelineColorBlendAttachmentState> color_blend_attachments;
	VkPipelineColorBlendStateCreateInfo color_blend;
	std::vector<VkDynamicState> dynamic_states;
	std::vector <VkDescriptorSetLayoutBinding> descriptor_set_layout_bindings;
	std::vector <VkPushConstantRange> push_constant_ranges;

	auto operator () () noexcept -> GraphicsPipeline {
		auto shader_modules = std::vector <VkShaderModule> {shaders.size ()};
		auto shader_create_infos = std::vector <VkPipelineShaderStageCreateInfo> {shaders.size ()};
		for (auto i = 0; i < shaders.size (); ++i) {
			auto const & [path, stage] = shaders [i];
			// printf (path);
			// printf ("\n");
			// printf ("graphics pipeline\n");
			shader_modules [i] = make_shader_module (path, device.handle);
			// printf ("graphics pipeline\n");
			shader_create_infos [i] = VkPipelineShaderStageCreateInfo {
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				nullptr,
				{},
				stage,
				shader_modules [i],
				"main",
				0
			};
		}
		// printf ("graphics pipeline\n");
		color_blend.attachmentCount = static_cast <uint32_t> (color_blend_attachments.size ());
		color_blend.pAttachments = color_blend_attachments.data ();

		auto const viewport = VkPipelineViewportStateCreateInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
			.viewportCount = static_cast <uint32_t> (viewports.size ()),
			.pViewports = viewports.data (),
			.scissorCount = static_cast <uint32_t> (scissors.size ()),
			.pScissors = scissors.data ()
		};

		auto const dynamic_state = VkPipelineDynamicStateCreateInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
			.dynamicStateCount = static_cast <uint32_t> (dynamic_states.size ()),
			.pDynamicStates = dynamic_states.data ()
		};

		auto push_constant_range = VkPushConstantRange
		{
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
			.offset = 0,
			.size = sizeof (mvp_push_constants)
		};

		auto const descriptor_set_layout_create_info = VkDescriptorSetLayoutCreateInfo {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.bindingCount = (uint32_t) descriptor_set_layout_bindings.size(),
			.pBindings = descriptor_set_layout_bindings.data()
		};
		auto descriptor_set_layout = VkDescriptorSetLayout {}; 

		if (vkCreateDescriptorSetLayout (device.handle, &descriptor_set_layout_create_info, nullptr, &descriptor_set_layout) != VK_SUCCESS) {
			std::cout << "error >> failed to create descriptor set layout" << std::endl;
			exit (-1);
		}

		auto const pipeline_layout_create_info = VkPipelineLayoutCreateInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.setLayoutCount = 1,
			.pSetLayouts = &descriptor_set_layout,
			.pushConstantRangeCount = static_cast <uint32_t> (push_constant_ranges.size ()),
			.pPushConstantRanges = push_constant_ranges.data ()
		};

		auto pipeline_layout = VkPipelineLayout {};

		if (vkCreatePipelineLayout(device.handle, &pipeline_layout_create_info, nullptr, &pipeline_layout) != VK_SUCCESS) {
			std::cout << "error >> failed to create pipeline layout" << std::endl;
			exit (-1);
		}

		auto const rendering_create_info = VkPipelineRenderingCreateInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
			.colorAttachmentCount = 1,
			.pColorAttachmentFormats = &image_format
		};

		auto const vertex_input = VkPipelineVertexInputStateCreateInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
			.vertexBindingDescriptionCount = (uint32_t) vertex_binding_descriptions.size (),
			.pVertexBindingDescriptions = vertex_binding_descriptions.data (),
			.vertexAttributeDescriptionCount = (uint32_t) vertex_attribute_descriptions.size (),
			.pVertexAttributeDescriptions = vertex_attribute_descriptions.data ()
		};

		auto const create_info = VkGraphicsPipelineCreateInfo {
			.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
			.pNext = nullptr,//&rendering_create_info,
			.stageCount = static_cast <uint32_t> (shader_create_infos.size ()),
			.pStages = shader_create_infos.data (),
			.pVertexInputState = &vertex_input,
			.pInputAssemblyState = &input_assembly,
			.pViewportState = &viewport,
			.pRasterizationState = &rasterization,
			.pMultisampleState = &multisample,
			.pDepthStencilState = nullptr, //&depth_stencil,
			.pColorBlendState = &color_blend,
			.pDynamicState = &dynamic_state,
			.layout = pipeline_layout,
			.renderPass = render_pass,
			.subpass = 0,
			.basePipelineHandle = VK_NULL_HANDLE,
			.basePipelineIndex = -1
		};

		auto pipeline = VkPipeline {};

		if (vkCreateGraphicsPipelines (device.handle, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline) != VK_SUCCESS) {
			std::cout << "error >> failed to create graphics pipeline" << std::endl;
		}

		for (auto & i : shader_modules) {
			vkDestroyShaderModule (device.handle, i, nullptr);
		}

		return {.device = device, .handle = pipeline, .descriptor_set_layout = descriptor_set_layout, .layout = pipeline_layout};
	}
	auto recreate (GraphicsPipeline & p) noexcept -> void {
		p.destroy ();
		auto new_p = this->operator()();
		p.handle = new_p.handle;
		p.layout = new_p.layout;
	}
};

struct DeviceMemory {
	LogicalDevice device;
	VkDeviceMemory handle;

	auto paste (auto * data, VkDeviceSize const size) noexcept -> void {
		void * mem;
		vkMapMemory (device.handle, handle, 0, size, 0, &mem);
		memcpy (mem, data, (size_t) size);
		vkUnmapMemory (device.handle, handle);
	}

	auto bind (VkBuffer const & buffer, VkDeviceSize const & offset = 0) noexcept -> DeviceMemory& {
		vkBindBufferMemory (device.handle, buffer, handle, offset);
		return *this;
	}

	auto bind (VkImage const & image, VkDeviceSize const & offset = 0) noexcept -> DeviceMemory& {
		vkBindImageMemory (device.handle, image, handle, offset);
		return *this;
	}

	auto map_memory (VkDeviceSize const & size, VkDeviceSize const & offset = 0, VkMemoryMapFlags const & flags = 0) const noexcept -> void * {
		void * data;
		vkMapMemory (device.handle, handle, offset, size, flags, &data);
		return data;
	}
	auto unmap_memory () const noexcept -> void {
		vkUnmapMemory (device.handle, handle);
	}
	auto destroy () noexcept -> void {
		vkFreeMemory (device.handle, handle, nullptr);
	}
};

template <>
struct Details <DeviceMemory> {
	PhysicalDevice const & physical_device;
	LogicalDevice & device;
	VkDeviceSize size;
	VkMemoryPropertyFlags flags;
	// uint32_t memory_type_index;

	auto operator () () noexcept -> DeviceMemory {

		auto const allocate_info = VkMemoryAllocateInfo {
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.allocationSize = size,
			.memoryTypeIndex = physical_device.get_memory_type_index (size, flags)
		};

		auto handle = VkDeviceMemory {};

		if (vkAllocateMemory (device.handle, &allocate_info, nullptr, &handle) != VK_SUCCESS) {
			std::cout << "error >> failed to allocate device memory" << std::endl;
			exit (-1);
		}

		return {.device = device, .handle = handle};
	}
	
};

struct Buffer {
	LogicalDevice device;
	VkBuffer handle;
	VkDeviceSize size;

	operator VkBuffer& () noexcept {
		return handle;
	}

	auto bind (DeviceMemory & device_memory, VkDeviceSize const & offset = 0) noexcept -> void {
		vkBindBufferMemory (device.handle, handle, device_memory.handle, offset);
	}
	auto get_memory_requirements () const noexcept -> VkMemoryRequirements {
		VkMemoryRequirements mem_requirements;
		vkGetBufferMemoryRequirements (device.handle, handle, &mem_requirements);
		return mem_requirements;
	}
	auto copy_from (Buffer & src) noexcept -> void {
		for (auto const & queue_family : device.queue_families) {
			if (queue_family.supports_transfer_operations ()) {	
				auto command_buffer = queue_family.command_pool.make_command_buffer (VK_COMMAND_BUFFER_LEVEL_PRIMARY, device.handle);
				auto & queue = queue_family.handles.back();
				command_buffer.begin (VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

				command_buffer.copy_buffer (src.handle, handle, {VkBufferCopy {.srcOffset = 0, .dstOffset = 0, .size = src.get_memory_requirements().size}});
				// vkCmdCopyBuffer (command_buffer.handle, src.handle, handle, 1, &copy_region);
				command_buffer.end ();
	
				// queue.submit (submit_info);
				auto const s2 = VkSubmitInfo {
					.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
					.commandBufferCount = 1,
					.pCommandBuffers = &command_buffer.handle
				};
				vkQueueSubmit(queue.handle, 1, &s2, VK_NULL_HANDLE);
				
				queue.wait_idle ();
				vkFreeCommandBuffers (device.handle, queue_family.command_pool.handle, 1, &command_buffer.handle);
				return;
			}
		}
		std::cout << "error << failed to copy buffer" << std::endl;
		exit (-1);
	}
	auto destroy () noexcept -> void {
		vkDestroyBuffer (device.handle, handle, nullptr);
	}
};

template <>
struct Details <Buffer> {
	LogicalDevice & device;
	VkDeviceSize size;
	VkBufferUsageFlags usage;
	VkSharingMode sharing_mode;

	auto operator () () const noexcept -> Buffer {

		auto const create_info = VkBufferCreateInfo {
			.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
			.size = size,
			.usage = usage,
			.sharingMode = sharing_mode
		};

		auto buffer = VkBuffer {};

		if (vkCreateBuffer (device.handle, &create_info, nullptr, &buffer) != VK_SUCCESS) {
			std::cout << "error >> failed to create buffer" << std::endl;
			exit (-1);
		}

		return {device, buffer, size};
	}
};

struct Image {
	LogicalDevice const & device;
	VkImage handle;
	DeviceMemory device_local_memory;
	VkImageView view;

	auto bind (DeviceMemory & device_memory, VkDeviceSize const & offset = 0) noexcept -> void {
		vkBindImageMemory (device.handle, handle, device_memory.handle, offset);
	}
	auto get_memory_requirements () const noexcept -> VkMemoryRequirements {
		VkMemoryRequirements mem_requirements;
		vkGetImageMemoryRequirements (device.handle, handle, &mem_requirements);
		return mem_requirements;
	}
	auto destroy () noexcept -> void {
		vkDestroyImageView (device.handle, view, nullptr);
		vkDestroyImage (device.handle, handle, nullptr);
		device_local_memory.destroy ();
	}
};

template <>
struct Details <Image> {
	PhysicalDevice const & physical_device;
	LogicalDevice & device;
	VkFormat const & format;
	char const * path;



	auto operator () () noexcept -> Image {

		int texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load (path, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		VkDeviceSize image_size = texWidth * texHeight * 4; // 4 bytes per pixel in the case of STBI_rgb_alpha
		if (!pixels) {
			std::cout << "error >> failed to load texture image" << std::endl;
			exit (-1);
		}

		auto host_visible_buffer = Details <Buffer> {
			.device = device,
			.size = image_size,
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.sharing_mode = VK_SHARING_MODE_EXCLUSIVE
		} ();

		auto host_visible_buffer_memory = Details <DeviceMemory> {
			.physical_device = physical_device,
			.device = device,
			.size = image_size,
			.flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		} ();

		host_visible_buffer_memory.bind (host_visible_buffer).paste (pixels, image_size);
		stbi_image_free (pixels);


		// createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB,
		// VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT |
		// VK_IMAGE_USAGE_SAMPLED_BIT,
		// VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage,
		// textureImageMemory);
		
		auto const create_info = VkImageCreateInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.flags = 0,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = format,
			.extent = {
				.width = static_cast<uint32_t>(texWidth),
				.height = static_cast<uint32_t>(texHeight),
				.depth = 1,
			},
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
		};

		auto image = VkImage {};

		if (vkCreateImage (device.handle, &create_info, nullptr, &image) != VK_SUCCESS) {
			std::cout << "error >> failed to create image" << std::endl;
			exit (-1);
		}

		// return 

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements (device.handle, image, &memRequirements);

		auto device_local_memory = Details <DeviceMemory> {
			.physical_device = physical_device,
			.device = device,
			.size = memRequirements.size,
			.flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		} ();

		device_local_memory.bind (image);

		auto queue_family = [&] -> QueueFamily {
			for (auto & queue_family : device.queue_families) {
				if (queue_family.supports_transfer_operations ()) {
					return queue_family;
				}
			}
			std::cout << "error >> failed to create image" << std::endl;
			exit (-1);
		} ();
		auto & queue = queue_family.handles.front();
		auto cmd_buffer = queue_family.command_pool.make_command_buffer (VK_COMMAND_BUFFER_LEVEL_PRIMARY, device.handle);
		auto barrier = VkImageMemoryBarrier {
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.srcAccessMask = 0,
			.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
			.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image = image,
			.subresourceRange = VkImageSubresourceRange {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			},
		};
		auto copy_region = VkBufferImageCopy {
			.bufferOffset = 0,
			.bufferRowLength = 0,
			.bufferImageHeight = 0,
			.imageSubresource = VkImageSubresourceLayers {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.mipLevel = 0,
				.baseArrayLayer = 0,
				.layerCount = 1
			},
			.imageOffset = {0, 0, 0},
			.imageExtent = {(uint32_t) texWidth, (uint32_t) texHeight, 1}
		};
		auto source_stage = VkPipelineStageFlags {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT};
		auto destination_stage = VkPipelineStageFlags {VK_PIPELINE_STAGE_TRANSFER_BIT};

		cmd_buffer.begin (VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		vkCmdPipelineBarrier (cmd_buffer.handle, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		vkCmdCopyBufferToImage (cmd_buffer.handle, host_visible_buffer.handle, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_region);
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		vkCmdPipelineBarrier (cmd_buffer.handle, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		// cmd_buffer.copy_buffer (src.handle, handle, {VkBufferCopy {.srcOffset = 0, .dstOffset = 0, .size = src.get_memory_requirements().size}});
				// vkCmdCopyBuffer (command_buffer.handle, src.handle, handle, 1, &copy_region);
		cmd_buffer.end ();
		auto const submit_info = VkSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.commandBufferCount = 1,
			.pCommandBuffers = &cmd_buffer.handle	
		};

		queue.submit (submit_info);
	
		queue.wait_idle ();

		vkFreeCommandBuffers (device.handle, queue_family.command_pool.handle, 1, &cmd_buffer.handle);

		host_visible_buffer.destroy ();
		host_visible_buffer_memory.destroy ();

		auto const view_create_info = VkImageViewCreateInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = format,
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};

		auto view = VkImageView {};

		if (vkCreateImageView (device.handle, &view_create_info, nullptr, &view) != VK_SUCCESS) {
			std::cout << "error >> failed to create image view" << std::endl;
			exit (-1);
		}

		return {.device = device, .handle = image, .device_local_memory = device_local_memory, .view = view};
	}
};


struct Details_image2 {
	PhysicalDevice const & physical_device;
	LogicalDevice & device;
	VkFormat const & format;
	uint32_t tex_width, tex_height;
	unsigned char * pixels;



	auto operator () () noexcept -> Image {

		VkDeviceSize image_size = tex_width * tex_height * 4; // 4 bytes per pixel in the case of STBI_rgb_alpha

		auto host_visible_buffer = Details <Buffer> {
			.device = device,
			.size = image_size,
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.sharing_mode = VK_SHARING_MODE_EXCLUSIVE
		} ();

		auto host_visible_buffer_memory = Details <DeviceMemory> {
			.physical_device = physical_device,
			.device = device,
			.size = image_size,
			.flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		} ();

		host_visible_buffer_memory.bind (host_visible_buffer).paste (pixels, image_size);
		// stbi_image_free (pixels);


		// createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB,
		// VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT |
		// VK_IMAGE_USAGE_SAMPLED_BIT,
		// VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage,
		// textureImageMemory);
		
		auto const create_info = VkImageCreateInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
			.flags = 0,
			.imageType = VK_IMAGE_TYPE_2D,
			.format = format,
			.extent = {
				.width = static_cast<uint32_t>(tex_width),
				.height = static_cast<uint32_t>(tex_height),
				.depth = 1
			},
			.mipLevels = 1,
			.arrayLayers = 1,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
		};

		auto image = VkImage {};

		if (vkCreateImage (device.handle, &create_info, nullptr, &image) != VK_SUCCESS) {
			std::cout << "error >> failed to create image" << std::endl;
			exit (-1);
		}

		// return 

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements (device.handle, image, &memRequirements);

		auto device_local_memory = Details <DeviceMemory> {
			.physical_device = physical_device,
			.device = device,
			.size = memRequirements.size,
			.flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		} ();

		device_local_memory.bind (image);

		auto queue_family = [&] -> QueueFamily {
			for (auto & queue_family : device.queue_families) {
				if (queue_family.supports_transfer_operations ()) {
					return queue_family;
				}
			}
			std::cout << "error >> failed to create image" << std::endl;
			exit (-1);
		} ();
		auto & queue = queue_family.handles.front();
		auto cmd_buffer = queue_family.command_pool.make_command_buffer (VK_COMMAND_BUFFER_LEVEL_PRIMARY, device.handle);
		auto barrier = VkImageMemoryBarrier {
			.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
			.srcAccessMask = 0,
			.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
			.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
			.image = image,
			.subresourceRange = VkImageSubresourceRange {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};
		auto copy_region = VkBufferImageCopy {
			.bufferOffset = 0,
			.bufferRowLength = 0,
			.bufferImageHeight = 0,
			.imageSubresource = VkImageSubresourceLayers {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.mipLevel = 0,
				.baseArrayLayer = 0,
				.layerCount = 1
			},
			.imageOffset = {0, 0, 0},
			.imageExtent = {(uint32_t) tex_width, (uint32_t) tex_height, 1}
		};
		auto source_stage = VkPipelineStageFlags {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT};
		auto destination_stage = VkPipelineStageFlags {VK_PIPELINE_STAGE_TRANSFER_BIT};

		cmd_buffer.begin (VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		vkCmdPipelineBarrier (cmd_buffer.handle, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		vkCmdCopyBufferToImage (cmd_buffer.handle, host_visible_buffer.handle, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copy_region);
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
		source_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destination_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		vkCmdPipelineBarrier (cmd_buffer.handle, source_stage, destination_stage, 0, 0, nullptr, 0, nullptr, 1, &barrier);
		// cmd_buffer.copy_buffer (src.handle, handle, {VkBufferCopy {.srcOffset = 0, .dstOffset = 0, .size = src.get_memory_requirements().size}});
				// vkCmdCopyBuffer (command_buffer.handle, src.handle, handle, 1, &copy_region);
		cmd_buffer.end ();
		auto const submit_info = VkSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.commandBufferCount = 1,
			.pCommandBuffers = &cmd_buffer.handle	
		};

		queue.submit (submit_info);
	
		queue.wait_idle ();

		vkFreeCommandBuffers (device.handle, queue_family.command_pool.handle, 1, &cmd_buffer.handle);

		host_visible_buffer.destroy ();
		host_visible_buffer_memory.destroy ();

		auto const view_create_info = VkImageViewCreateInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = format,
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1
			}
		};

		auto view = VkImageView {};

		if (vkCreateImageView (device.handle, &view_create_info, nullptr, &view) != VK_SUCCESS) {
			std::cout << "error >> failed to create image view" << std::endl;
			exit (-1);
		}

		return {.device = device, .handle = image, .device_local_memory = device_local_memory, .view = view};
	}
};



struct Sampler {
	LogicalDevice const & device;
	VkSampler handle;

	auto destroy () const noexcept -> void {
		vkDestroySampler (device.handle, handle, nullptr);
	}
};

template <>
struct Details <Sampler> {
	LogicalDevice const & device;

	auto operator () () const noexcept -> Sampler {
		auto const create_info = VkSamplerCreateInfo {
			.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
			.magFilter = VK_FILTER_LINEAR,
			.minFilter = VK_FILTER_LINEAR,
			.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
			.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.mipLodBias = 0.0f,
			.anisotropyEnable = VK_TRUE,
			.maxAnisotropy = device.physical_device.get_properties().limits.maxSamplerAnisotropy,
			.compareEnable = VK_FALSE,
			.compareOp = VK_COMPARE_OP_NEVER,// VK_COMPARE_OP_ALWAYS,
			.minLod = 0.0f,
			.maxLod = 1.0f,
			.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
			.unnormalizedCoordinates = VK_FALSE
		};

		auto sampler = VkSampler {};

		if (vkCreateSampler (device.handle, &create_info, nullptr, &sampler) != VK_SUCCESS) {
			std::cout << "error >> failed to create sampler" << std::endl;
			exit (-1);
		}

		return {.device = device, .handle = sampler};
	}
};

struct DescriptorPool {
	LogicalDevice device;
	VkDescriptorPool handle;

	auto allocate (VkDescriptorSetLayout const& layout) const noexcept -> std::vector <VkDescriptorSet> {
		
		auto const allocate_info = VkDescriptorSetAllocateInfo {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.descriptorPool = handle,
			.descriptorSetCount = 1,
			.pSetLayouts = &layout
		};

		auto descriptor_sets = std::vector <VkDescriptorSet> {1};

		if (vkAllocateDescriptorSets (device.handle, &allocate_info, descriptor_sets.data()) != VK_SUCCESS) {
			std::cout << "error >> failed to allocate descriptor sets" << std::endl;
			exit (-1);
		}

		return descriptor_sets;
	}
	auto allocate (std::vector <VkDescriptorSetLayout> const & layouts) const noexcept -> std::vector <VkDescriptorSet> {
		
		auto const allocate_info = VkDescriptorSetAllocateInfo {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
			.descriptorPool = handle,
			.descriptorSetCount = (uint32_t) layouts.size (),
			.pSetLayouts = layouts.data ()
		};

		auto descriptor_sets = std::vector <VkDescriptorSet> {layouts.size ()};

		if (vkAllocateDescriptorSets (device.handle, &allocate_info, descriptor_sets.data()) != VK_SUCCESS) {
			std::cout << "error >> failed to allocate descriptor sets" << std::endl;
			exit (-1);
		}

		return descriptor_sets;
	}
	auto destroy () const noexcept -> void {
		vkDestroyDescriptorPool (device.handle, handle, nullptr);
	}
};

template <>
struct Details <DescriptorPool> {
	LogicalDevice device;
	std::vector <VkDescriptorPoolSize> pool_sizes;
	uint32_t const & max_sets;

	auto operator () () const noexcept -> DescriptorPool {

		auto const create_info = VkDescriptorPoolCreateInfo {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.maxSets = max_sets,
			.poolSizeCount = (uint32_t) pool_sizes.size(),
			.pPoolSizes = pool_sizes.data(),
		};

		auto handle = VkDescriptorPool {};

		if (vkCreateDescriptorPool (device.handle, &create_info, nullptr, &handle) != VK_SUCCESS) {
			std::cout << "error >> failed to create descriptor pool" << std::endl;
			exit (-1);
		}

		return {.device = device, .handle = handle};
	}
};

template <uint32_t _width, uint32_t _height>
struct FontBitmap3 {
	constexpr static uint32_t image_width = _width;
	constexpr static uint32_t image_height = _height;
	unsigned char buffer [image_width * image_height * 4];
	int width [128];
	int height [128];
	int x0 [128];
	int y0 [128];
	int hang [128];
};

// constexpr auto font_size = 128;
// constexpr auto padding = 2;
// auto bitmap = FontBitmap3 <(font_size+padding)*16, (font_size+padding)*8> {};


struct window {
	auto hide () noexcept -> void {
		glfwHideWindow (handle);
	}
	auto show () noexcept -> void {
		glfwShowWindow (handle);
	}
	auto open () const noexcept -> bool {
		return is_open;
	}
	// auto click () noexcept -> future_io {
	// 	printf("setting glfwSetWindowUserPointer\n");
	// 	co_yield handle;
	// 	printf("setting glfwSetWindowUserPointer\n");
	// 	co_await std::suspend_always {};
	// 	// printf("setting glfwSetWindowUserPointer\n");
	// 	co_return;
	// 	// auto h = co_await my_coro_handle {};
	// 	// void * h;
	// 	// co_yield h;
	// 	// printf("setting glfwSetWindowUserPointer\n");
	// 	// glfwSetWindowUserPointer (handle, h);
	// 	// co_await std::suspend_always {};
	// 	// printf("Release?\n");
	// 	// co_yield;
	// 	// printf ("registering a click\n");
	// 	// co_return nullptr;
	// }
	auto get_extent () const noexcept -> VkExtent2D {
		int width, height;
		glfwGetFramebufferSize(handle, &width, &height);

		return {
			.width = static_cast <uint32_t> (width),
			.height = static_cast <uint32_t> (height)
		};
	}
	auto draw_frame () noexcept -> void {

	}

	friend auto swap (window& lhs, window& rhs) noexcept -> void {
		using std::swap;
		std::swap (lhs.graphics_pipeline, rhs.graphics_pipeline);
		std::swap (lhs.windows, rhs.windows);
		std::swap (lhs.handle, rhs.handle);
		std::swap (lhs.render_pass, rhs.render_pass);
		std::swap (lhs.surface, rhs.surface);
		std::swap (lhs.device, rhs.device);
		std::swap (lhs.swapchain, rhs.swapchain);
		std::swap (lhs.is_open, rhs.is_open);
		std::swap (lhs.text_vertices, rhs.text_vertices);
		std::swap (lhs.line_vertices, rhs.line_vertices);
		std::swap (lhs.line_indices, rhs.line_indices);
		glfwSetWindowUserPointer (lhs.handle, &lhs);
	}

	// window (std::vector <window> * windows, VkInstance instance, PhysicalDevice physical_device) noexcept : window {windows, instance, physical_device, 640, 480, ""} {
	// 	// auto device = Details <LogicalDevice> {
	// 	// 	.physical_device = physical_device,
	// 	// 	.surface = surface,
	// 	// 	.extensions = {"VK_KHR_swapchain", "VK_KHR_portability_subset", "VK_KHR_dynamic_rendering", "VK_KHR_depth_stencil_resolve", "VK_KHR_create_renderpass2"},
	// 	// 	.features = {.samplerAnisotropy = true},
	// 	// 	.layers = {}
	// 	// } ();

	// }

	window (auto & bitmap, std::vector <window> * windows, VkInstance instance, PhysicalDevice physical_device, int width, int height, char const* title) noexcept : windows {windows}, handle {glfwCreateWindow (width, height, title, nullptr, nullptr)}, is_open {true} {
		glfwSetWindowUserPointer (handle, this);
		glfwSetWindowCloseCallback (handle, window_close_callback);
		///*
		surface = Details <VkSurfaceKHR> {
			.instance = instance,
			.window = handle
		} ();
		device = Details <LogicalDevice> {
			.physical_device = physical_device,
			.surface = surface,
			.layers = {},
			.extensions = {"VK_KHR_swapchain", "VK_KHR_portability_subset", "VK_KHR_dynamic_rendering", "VK_KHR_depth_stencil_resolve", "VK_KHR_create_renderpass2"},
			.features = {.samplerAnisotropy = true}
		} ();

		auto render_pass_details = Details <RenderPass> {
			.device = device.handle,
			.attachment_descriptions = {
				{
					.format = VK_FORMAT_B8G8R8A8_SRGB,
					.samples = VK_SAMPLE_COUNT_1_BIT,
					.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
					.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
					.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
					.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
					.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
					.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
				}
			},
			.attachment_references = {
				{
					.attachment = 0,
					.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
				}
			},
			.subpass_descriptions = {
				{
					.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
					.colorAttachmentCount = 1,
				}
			},
			.subpass_dependencies = {
				{
					.srcSubpass = VK_SUBPASS_EXTERNAL,
					.dstSubpass = 0,
					.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					.srcAccessMask = 0,
					.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
				}
			}
		};

		render_pass_details.subpass_descriptions[0].pColorAttachments = &render_pass_details.attachment_references[0];

		render_pass = render_pass_details ();

		auto swapchain_details = Details <Swapchain> {
			.device = device.handle,
			.surface = surface,
			.render_pass = render_pass.handle,
			.image_count = get_surface_capabilities (device.physical_device.handle, surface).maxImageCount,
			.image_format = VK_FORMAT_B8G8R8A8_SRGB,
			.image_color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
			.image_extent = this -> get_extent (),
			.image_usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
			.image_sharing_mode = VK_SHARING_MODE_EXCLUSIVE,
			.pre_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
			.composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			.present_mode = VK_PRESENT_MODE_FIFO_KHR,
			.clipped = VK_TRUE,
			.view_type = VK_IMAGE_VIEW_TYPE_2D,
			.view_components {
				.r = VK_COMPONENT_SWIZZLE_IDENTITY, 
				.g = VK_COMPONENT_SWIZZLE_IDENTITY, 
				.b = VK_COMPONENT_SWIZZLE_IDENTITY, 
				.a = VK_COMPONENT_SWIZZLE_IDENTITY},
			.view_subresource_range {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, 
				.baseMipLevel = 0,
				.levelCount = 1, 
				.baseArrayLayer = 0, 
				.layerCount = 1}
		};

		swapchain = swapchain_details ();

		auto graphics_pipeline_details = Details <GraphicsPipeline> {
			.device = device,
			.render_pass = render_pass.handle,
		
			.image_format = VK_FORMAT_B8G8R8A8_SRGB,
			// };
			.shaders = {{"Graphics.Triangle.vert.spv", VK_SHADER_STAGE_VERTEX_BIT}, {"Graphics.Triangle.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT}},
			.vertex_binding_descriptions = {
				{
					.binding = 0,
					.stride = sizeof (Vertex),
					.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
				}},
			.vertex_attribute_descriptions = {
				{
					.location = 0,
					.binding = 0,
					.format = VK_FORMAT_R32G32B32_SFLOAT,
					.offset = offsetof (Vertex, pos)
				},
				{
					.location = 1,
					.binding = 0,
					.format = VK_FORMAT_R32G32B32_SFLOAT,
					.offset = offsetof (Vertex, color)
				},
				{
					.location = 2,
					.binding = 0,
					.format = VK_FORMAT_R32G32_SFLOAT,
					.offset = offsetof (Vertex, tex_coord)
				},
				{
					.location = 3,
					.binding = 0,
					.format = VK_FORMAT_R32G32_SFLOAT,
					.offset = offsetof (Vertex, as_texture)
				}},
			.input_assembly = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
				.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
				.primitiveRestartEnable = VK_FALSE},
			.viewports = {
				{
					.x = 0.0f,
					.y = 0.0f,
					.width = (float) swapchain.image_extent.width,
					.height = (float) swapchain.image_extent.height,
					.minDepth = 0.0f,
					.maxDepth = 1.0f
				}},
			.scissors = {
				{
					.offset = {0, 0},
					.extent = swapchain.image_extent
				}},
			.rasterization = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
				.depthClampEnable = VK_FALSE,
				.rasterizerDiscardEnable = VK_FALSE,
				.polygonMode = VK_POLYGON_MODE_FILL,
				.cullMode = VK_CULL_MODE_BACK_BIT,
				.frontFace = VK_FRONT_FACE_CLOCKWISE,
				.depthBiasEnable = VK_FALSE,
				.depthBiasConstantFactor = 0.0f,
				.depthBiasClamp = 0.0f,
				.depthBiasSlopeFactor = 0.0f,
				.lineWidth = 1.0f},
			.multisample = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
				.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
				.sampleShadingEnable = VK_FALSE,
				.minSampleShading = 1.0f,
				.pSampleMask = nullptr,
				.alphaToCoverageEnable = VK_FALSE,
				.alphaToOneEnable = VK_FALSE},
			.color_blend_attachments = {
				{
					.blendEnable = VK_FALSE,
					.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
					.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
					.colorBlendOp = VK_BLEND_OP_ADD,
					.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
					.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
					.alphaBlendOp = VK_BLEND_OP_ADD,
					.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
				}},
			.color_blend = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
				.logicOpEnable = VK_FALSE,
				.logicOp = VK_LOGIC_OP_COPY,
				.blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}},
			.dynamic_states = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_LINE_WIDTH},
			.descriptor_set_layout_bindings = {
				{
					.binding = 0,
					.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					.descriptorCount = 1,
					.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
					.pImmutableSamplers = nullptr
				}},
			.push_constant_ranges = {
				{
					.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
					.offset = 0,
					.size = sizeof (mvp_push_constants)
				},
				{
					.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
					.offset = 0,
					.size = sizeof (texture_push_constants)
				}}
		};

		graphics_pipeline = graphics_pipeline_details ();

		auto presentation_queue = [&] -> Queue {
			for (auto const & queue_family : device.queue_families) {
				if (queue_family.present_support) {
					return queue_family.handles.front ();
				}
			}
			// std::cout << "error >> failed to find a presentation queue" << std::endl;
			printf ("error >> failed to find a presentation queue\n");
			exit (-1);
		} ();

		auto graphics_queue = [&] -> Queue {
				for (auto const & queue_family : device.queue_families) {
					if (queue_family.supports_graphics_operations ()) {
						return queue_family.handles.front ();
					}
				}
				// std::cout << "error >> failed to find a graphics queue" << std::endl;
				printf ("error >> failed to find a graphics queue\n");
				exit (-1);
			} ();

		auto transfer_queue = [&] -> Queue {
				for (auto i = device.queue_families.size () - 1; i >= 0; --i){
					if (device.queue_families[i].supports_graphics_operations ()) {
						return device.queue_families[i].handles.front ();
					}
				}
				// std::cout << "error >> failed to find a graphics queue" << std::endl;
				printf ("error >> failed to find a graphics queue\n");
				exit (-1);
			} ();

		auto const max_frames_in_flight = swapchain.images.size ();

		auto current_frame = 0;

		auto draw_command_buffer = [&] -> std::vector <CommandBuffer> {
			auto buffers = std::vector <CommandBuffer> {max_frames_in_flight};

			for (auto const & queue_family : device.queue_families) {
				if (queue_family.supports_graphics_operations ()) {
					for (auto & buffer : buffers) {
						buffer = queue_family.command_pool.make_command_buffer (VK_COMMAND_BUFFER_LEVEL_PRIMARY, device.handle);
					}
					return buffers;
				}
			}
			// std::cout << "error >> failed to find a graphics queue" << std::endl;
			printf ("error >> failed to find a graphics queue\n");
			exit (-1);
		} ();
		
		// device.make_graphics_command_buffer (VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		auto pushConstants = mvp_push_constants {
			.model = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},//glm::rotate (glm::mat4 (1.0f), time * glm::radians (90.0f), glm::vec3 (0.0f, 0.0f, 1.0f)),
			.view = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},//glm::lookAt (glm::vec3 (2.0f, 2.0f, 2.0f), glm::vec3 (0.0f,0.0f, 0.0f), glm::vec3 (0.0f, 0.0f, 1.0f)),
			.proj = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},//glm::perspective (glm::radians (45.0f), surfaceExtent.width / (float) surfaceExtent.height, 0.1f, 10.0f)
		};

		auto texturePushConstants = texture_push_constants {

		};
	



		auto device_local_vertex_buffer_details = Details <Buffer> {
			.device = device,
			.size = sizeof (text_vertices) * 2048,
			.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			.sharing_mode = VK_SHARING_MODE_EXCLUSIVE
		};

		auto device_local_vertex_buffer = device_local_vertex_buffer_details ();

		auto device_local_vertex_buffer_memory_details = Details <DeviceMemory> {
			.physical_device = physical_device,
			.device = device,
			.size = device_local_vertex_buffer.get_memory_requirements ().size,
			.flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		};

		auto device_local_vertex_buffer_memory = device_local_vertex_buffer_memory_details ();

		device_local_vertex_buffer_memory.bind (device_local_vertex_buffer);

		auto host_visible_vertex_buffer_details = Details <Buffer> {
			.device = device,
			.size = sizeof (text_vertices) * 2048,
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.sharing_mode = VK_SHARING_MODE_EXCLUSIVE
		};

		auto host_visible_vertex_buffer = host_visible_vertex_buffer_details ();

		auto host_visible_vertex_buffer_memory_details = Details <DeviceMemory> {
			.physical_device = physical_device,
			.device = device,
			.size = host_visible_vertex_buffer.get_memory_requirements ().size,
			.flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		auto host_visible_vertex_buffer_memory = host_visible_vertex_buffer_memory_details ();

		host_visible_vertex_buffer_memory.bind (host_visible_vertex_buffer);

		auto const dublicate_vertex_buffers = [&] {
			device.wait_idle ();
			host_visible_vertex_buffer.destroy ();
			host_visible_vertex_buffer_memory.destroy ();
			device_local_vertex_buffer.destroy ();
			device_local_vertex_buffer_memory.destroy ();

			device_local_vertex_buffer_details.size *= 2;
			device_local_vertex_buffer = device_local_vertex_buffer_details ();

			device_local_vertex_buffer_memory_details.size *= 2;
			device_local_vertex_buffer_memory = device_local_vertex_buffer_memory_details ();
			device_local_vertex_buffer_memory.bind (device_local_vertex_buffer);

			host_visible_vertex_buffer_details.size *= 2;
			host_visible_vertex_buffer = host_visible_vertex_buffer_details ();

			host_visible_vertex_buffer_memory_details.size *= 2;
			host_visible_vertex_buffer_memory = host_visible_vertex_buffer_memory_details ();
			host_visible_vertex_buffer_memory.bind (host_visible_vertex_buffer);
		};






		auto device_local_line_vertex_buffer_details = Details <Buffer> {
			.device = device,
			.size = sizeof (line_vertices) * 2048,
			.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			.sharing_mode = VK_SHARING_MODE_EXCLUSIVE
		};

		auto device_local_line_vertex_buffer = device_local_line_vertex_buffer_details ();

		auto device_local_line_vertex_buffer_memory_details = Details <DeviceMemory> {
			.physical_device = physical_device,
			.device = device,
			.size = device_local_line_vertex_buffer.get_memory_requirements ().size,
			.flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		};

		auto device_local_line_vertex_buffer_memory = device_local_line_vertex_buffer_memory_details ();

		device_local_line_vertex_buffer_memory.bind (device_local_line_vertex_buffer);

		auto host_visible_line_vertex_buffer_details = Details <Buffer> {
			.device = device,
			.size = sizeof (line_vertices) * 2048,
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.sharing_mode = VK_SHARING_MODE_EXCLUSIVE
		};

		auto host_visible_line_vertex_buffer = host_visible_line_vertex_buffer_details ();

		auto host_visible_line_vertex_buffer_memory_details = Details <DeviceMemory> {
			.physical_device = physical_device,
			.device = device,
			.size = host_visible_line_vertex_buffer.get_memory_requirements ().size,
			.flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		auto host_visible_line_vertex_buffer_memory = host_visible_line_vertex_buffer_memory_details ();

		host_visible_line_vertex_buffer_memory.bind (host_visible_line_vertex_buffer);

		auto const dublicate_line_vertex_buffers = [&] {
			device.wait_idle ();
			host_visible_line_vertex_buffer.destroy ();
			host_visible_line_vertex_buffer_memory.destroy ();
			device_local_line_vertex_buffer.destroy ();
			device_local_line_vertex_buffer_memory.destroy ();

			device_local_line_vertex_buffer_details.size *= 2;
			device_local_line_vertex_buffer = device_local_line_vertex_buffer_details ();

			device_local_line_vertex_buffer_memory_details.size *= 2;
			device_local_line_vertex_buffer_memory = device_local_line_vertex_buffer_memory_details ();
			device_local_line_vertex_buffer_memory.bind (device_local_line_vertex_buffer);

			host_visible_line_vertex_buffer_details.size *= 2;
			host_visible_line_vertex_buffer = host_visible_line_vertex_buffer_details ();

			host_visible_line_vertex_buffer_memory_details.size *= 2;
			host_visible_line_vertex_buffer_memory = host_visible_line_vertex_buffer_memory_details ();
			host_visible_line_vertex_buffer_memory.bind (host_visible_line_vertex_buffer);
		};






		auto device_local_line_index_buffer_details = Details <Buffer> {
			.device = device,
			.size = sizeof (uint16_t) * 2048,
			.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
			.sharing_mode = VK_SHARING_MODE_EXCLUSIVE
		};

		auto device_local_line_index_buffer = device_local_line_index_buffer_details ();

		auto device_local_line_index_buffer_memory_details = Details <DeviceMemory> {
			.physical_device = physical_device,
			.device = device,
			.size = device_local_line_index_buffer.get_memory_requirements ().size,
			.flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		};

		auto device_local_line_index_buffer_memory = device_local_line_index_buffer_memory_details ();

		device_local_line_index_buffer_memory.bind (device_local_line_index_buffer);

		auto host_visible_line_index_buffer_details = Details <Buffer> {
			.device = device,
			.size = sizeof (uint16_t) * 2048,
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.sharing_mode = VK_SHARING_MODE_EXCLUSIVE
		};

		auto host_visible_line_index_buffer = host_visible_line_index_buffer_details ();

		auto host_visible_line_index_buffer_memory_details = Details <DeviceMemory> {
			.physical_device = physical_device,
			.device = device,
			.size = host_visible_line_index_buffer.get_memory_requirements ().size,
			.flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		};

		auto host_visible_line_index_buffer_memory = host_visible_line_index_buffer_memory_details ();

		host_visible_line_index_buffer_memory.bind (host_visible_line_index_buffer);

		auto const dublicate_line_index_buffers = [&] {
			device.wait_idle ();
			host_visible_line_index_buffer.destroy ();
			host_visible_line_index_buffer_memory.destroy ();
			device_local_line_index_buffer.destroy ();
			device_local_line_index_buffer_memory.destroy ();

			device_local_line_index_buffer_details.size *= 2;
			device_local_line_index_buffer = device_local_line_index_buffer_details ();

			device_local_line_index_buffer_memory_details.size *= 2;
			device_local_line_index_buffer_memory = device_local_line_index_buffer_memory_details ();
			device_local_line_index_buffer_memory.bind (device_local_line_index_buffer);

			host_visible_line_index_buffer_details.size *= 2;
			host_visible_line_index_buffer = host_visible_line_index_buffer_details ();

			host_visible_line_index_buffer_memory_details.size *= 2;
			host_visible_line_index_buffer_memory = host_visible_line_index_buffer_memory_details ();
			host_visible_line_index_buffer_memory.bind (host_visible_line_index_buffer);
		};

		auto texture_image = Details_image2 {
			.physical_device = physical_device,
			.device = device,
			.format = VK_FORMAT_R8G8B8A8_SRGB,
			.tex_width = bitmap.image_width,
			.tex_height = bitmap.image_height,
			.pixels = bitmap.buffer
		} ();

		auto texture_sampler = Details <Sampler> {
			.device = device
		} ();

		auto descriptor_pool = Details <DescriptorPool> {
			.device = device,
			.pool_sizes = {
				{
					.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					.descriptorCount = (uint32_t) max_frames_in_flight
				}
			},
			.max_sets = (uint32_t) max_frames_in_flight
		} ();

		auto descriptor_sets = descriptor_pool.allocate ({max_frames_in_flight, graphics_pipeline.descriptor_set_layout});
		// auto descriptor_sets = descriptor_pool.allocate (graphics_pipeline.descriptor_set_layout);
		{
			auto const sampler_info = VkDescriptorImageInfo {
				.sampler = texture_sampler.handle,
				.imageView = texture_image.view,
				.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
			};

			auto const sampler_info2 = VkDescriptorImageInfo {
				// .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
				// .imageView = texture_image.view,
				.sampler = texture_sampler.handle
			};

			auto descriptor_writes = std::vector <VkWriteDescriptorSet> {
				{
					.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
					.dstBinding = 0,
					.dstArrayElement = 0,
					.descriptorCount = 1,
					.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
					.pImageInfo = &sampler_info,
				}
				// {
				// 	.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				// 	.dstBinding = 1,
				// 	.dstArrayElement = 0,
				// 	.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
				// 	.descriptorCount = TEXTURE_ARRAY_SIZE,
				// 	.pImageInfo = &sampler_info,
				// }
			};

			for (auto const & descriptor_set : descriptor_sets){
				descriptor_writes[0].dstSet = descriptor_set;
				vkUpdateDescriptorSets (device.handle, static_cast <uint32_t> (descriptor_writes.size()), descriptor_writes.data(), 0, nullptr);
			}
		}

		auto viewport = VkRect2D {.offset = {.x = 0, .y = 0}, .extent = {.width = swapchain.image_extent.width, .height = swapchain.image_extent.height}};

		//*/
	}
	window (window&& other) noexcept : window {} {
		swap (*this, other);
		// std::swap (render_pass);
		// std::swap (physical_device, other.physical_device);
	}
	window (window const& o) noexcept : windows {o.windows}, render_pass {o.render_pass}, surface {o.surface}, device {o.device}, swapchain {o.swapchain}, handle {o.handle}, is_open {o.is_open} {
		// glfwSetWindowUserPointer (handle, this);
	}
	window () noexcept : windows {nullptr}, handle {nullptr} {}
	~window () {
		glfwDestroyWindow (handle);
	}
	auto operator= (window other) noexcept -> window& {
		swap (*this, other);
		return *this;
	}
	auto operator == (window const & o) noexcept -> bool {
		// printf("==?\n");
		return handle == o.handle;
	}

	

private:
	std::vector <window> * windows;
	std::vector <Vertex> text_vertices;
	std::vector <LineVertex> line_vertices;
	std::vector <uint16_t> line_indices;
	GraphicsPipeline graphics_pipeline;
	RenderPass render_pass;
	VkSurfaceKHR surface;
	LogicalDevice device;
	// PhysicalDevice physical_device;
	Swapchain swapchain;
	GLFWwindow* handle;
	bool is_open;
	
	
	static void window_close_callback(GLFWwindow* w) {
		window * me = (window *) glfwGetWindowUserPointer (w);
		for (auto i = me->windows->begin(); i < me->windows->end(); ++i) {
			if  (*i == *me) {
				me->windows->erase (i);
				break;
			}
		}
		// glfwDestroyWindow (w);
		// printf ("trying to close\n");
		// if (!time_to_close)
		// glfwSetWindowShouldClose (w, true);
		// glfwDestroyWindow (window);
	}
	static auto mouse_button_callback (GLFWwindow* w, int button, int action, int mods) noexcept -> void {
		// printf ("m\n");
		// if (auto * p = glfwGetWindowUserPointer (w); p == nullptr) {
		// 	printf ("m\n");
		// 	auto h = std::coroutine_handle <>::from_address (p); 
		// 		// if (not h.done ()) {
		// 		// 	printf ("yay\n");
		// 		// 	h.resume ();
		// 		// 	// printf ("~m\n");
		// 		// 	glfwSetWindowUserPointer (w, nullptr);
		// 		// } else {
		// 		// 	printf("what\n");
		// 		// }
		// 	h.resume ();
		// 	glfwSetWindowUserPointer (w, nullptr);
			
		// } else {
		// 	printf ("user pointer is null\n");
		// }
		
		// printf ("~m\n");
	}
};

// template <typename T>
// concept Graphics = requires (T t) {
// 	t.draw_frame ();
// 	t.process ();
// };


struct graphics {
	// auto windows_open () const noexcept -> int {
	// 	auto count = 0;
	// 	return windows.size ();
	// 	// printf ("count: %i\n", count);
	// 	// for (auto& w : windows) {
	// 	// 	// printf ("loop\n");
	// 	// 	if (w.open ()) {
	// 	// 		// printf ("open!\n");	
	// 	// 		++count;
	// 	// 	} else {
	// 	// 		// printf ("closed!\n");
	// 	// 	}
	// 	// }
	// 	// printf ("count: %i\n", count);
	// 	return count;
	// }

	// auto make_window (auto width, auto height) noexcept -> window & {
	// 	windows.push_back ({&windows, instance, physical_device, width, height, ""});
	// 	return windows.back ();
	// }
	// auto draw_frames () noexcept -> void {
	// 	for (auto & w : windows) {
	// 		w.draw_frame ();
	// 	}
	// }
	// auto process () noexcept -> void {
	// 	glfwPollEvents ();
	// }
	graphics () noexcept // : windows {0} 
	{
		constexpr auto width = 640;
		constexpr auto height = 480;
		glfwInit ();
		// glfwWindowHint (GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
	// #if defined (MACOS)
	// 	glfwWindowHint (GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_NO_API);
	// #endif

		// VkApplicationInfo app_info {
		// 	.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		// 	.pNext = nullptr,
		// 	.pApplicationName = "",
		// 	.applicationVersion = VK_MAKE_VERSION (1, 0, 0),
		// 	.pEngineName = "",
		// 	.engineVersion = VK_MAKE_VERSION (1, 0, 0),
		// 	.apiVersion = VK_MAKE_VERSION (1, 3, 0)
		// };
		// auto layers = std::array <char const*, 1> {"VK_LAYER_KHRONOS_validation"}; /*, "VK_LAYER_LUNARG_api_dump"*/
		// auto extensions = std::vector <char const*> {"VK_KHR_portability_enumeration", "VK_KHR_surface", "VK_EXT_metal_surface", "VK_MVK_macos_surface", "VK_KHR_get_physical_device_properties2"};

		// auto const createInfo = VkInstanceCreateInfo {
		// 	.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		// 	.pNext = nullptr,
		// 	 .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
		// 	.pApplicationInfo = &app_info,
		// 	.enabledLayerCount = static_cast <uint32_t> (layers.size()),
		// 	.ppEnabledLayerNames = layers.data(),
		// 	.enabledExtensionCount = static_cast <uint32_t> (extensions.size()),
		// 	.ppEnabledExtensionNames = extensions.data()
		// };

		// if (vkCreateInstance (/*VkInstanceCreateInfo const * pCreateInfo */&createInfo, /*VkAllocationCallbacks const * pAllocator */nullptr, /*VkInstance* pInstance*/&instance) != VK_SUCCESS) {
		// 	printf ("error >> failed to create instance");
		// 	exit (-1);
		// }

		// physical_device = Details <PhysicalDevice> {
		// 	.instance = instance,
		// 	.picker = [] (std::vector <VkPhysicalDevice> const & devices) -> VkPhysicalDevice {
		// 		return devices.back ();
		// 	}
		// } ();

		// int max_under_baseline = 0;
		// int max_height = 0;
		// constexpr auto font_size = 128;
		// constexpr auto padding = 2;
		// auto bitmap = FontBitmap3 <(font_size+padding)*16, (font_size+padding)*8> {};

		// {
			

		// 	auto lib = FT_Library {};

		// 	if (FT_Init_FreeType (&lib)) printNexit ("error >> failed to initialize freetype lib");
			
		// 	auto face = FT_Face {};
		// 	// "/Impact.ttf"
		// 	if (FT_New_Face (lib, (std::string {FONTS_DIR} + "/Charter.ttc").c_str (), 0, &face)) printNexit ("error >> failed to load font file");

		// 	if (FT_Set_Pixel_Sizes (face, 0, font_size)) printNexit ("error >> failed to set pixel sizes");

		// 	// create bitmap font
		// 	// int image_width = (font_size+2)*16;
		// 	// int image_height = (font_size+2)*8;

		// 	// create a buffer for the bitmap
		// 	// unsigned char* buffer = new unsigned char[image_width*image_height*4];
		// 	// memset( buffer , 0 , image_width*image_height*4 );

		// 	// create an array to save the character widths
		// 	// int* widths = new int[128];
		// 	// int* heights = new int[128];
		// 	// int* xs = new int[128];
		// 	// int* ys = new int[128];
		// 	// int* hang = new int[128];

		// 	// we need to find the character that goes below the baseline by the biggest value
		// 	int maxUnderBaseline = 0;
		// 	for ( int i = 32 ; i < 127 ; ++i )
		// 	{
		// 		// get the glyph index from character code
		// 		auto glyphIndex = FT_Get_Char_Index( face , i );

		// 		// load the glyph image into the slot
		// 		if (FT_Load_Glyph( face , glyphIndex , FT_LOAD_DEFAULT )) printNexit ("BitmapFontGenerator > failed to load glyph");
				

		// 		// get the glyph metrics
		// 		const FT_Glyph_Metrics& glyphMetrics = face->glyph->metrics;
				
		// 		// find the character that reaches below the baseline by the biggest value
		// 		int glyphHang = (glyphMetrics.horiBearingY-glyphMetrics.height)/64;
		// 		// std::cout << (char) i << " " << glyphHang << std::endl;
		// 		bitmap.hang [i] = glyphHang;
		// 		if ((glyphMetrics.height/64) + glyphHang > max_height) {
		// 			max_height = (glyphMetrics.height/64) + glyphHang;
		// 		}
				
		// 		if( glyphHang < maxUnderBaseline )
		// 		{
		// 			maxUnderBaseline = glyphHang;
		// 		}
		// 	}

		// 	max_under_baseline = maxUnderBaseline;

		// 	// draw all characters
		// 	for ( int i = 0 ; i < 128 ; ++i )
		// 	{
		// 		// get the glyph index from character code
		// 		auto glyphIndex = FT_Get_Char_Index( face , i );

		// 		// load the glyph image into the slot
		// 		if (FT_Load_Glyph( face , glyphIndex , FT_LOAD_DEFAULT )) printNexit ("BitmapFontGenerator > failed to load glyph, error code: ");

		// 		// convert to an anti-aliased bitmap
		// 		if (FT_Render_Glyph( face->glyph , FT_RENDER_MODE_NORMAL )) printNexit ("BitmapFontGenerator > failed to render glyph, error code: ");
			

		// 		// save the character width
		// 		bitmap.width [i] = face->glyph->metrics.width/64;

		// 		// find the tile position where we have to draw the character
		// 		int x = (i%16)*(font_size+padding);
		// 		int y = (i/16)*(font_size+padding);
				
		// 		x += 1; // 1 pixel padding from the left side of the tile
		// 		y += (font_size+padding) - face->glyph->bitmap_top + maxUnderBaseline - 1;

		// 		bitmap.x0 [i] = x;
		// 		bitmap.y0 [i] = y;

		// 		// draw the character
		// 		const FT_Bitmap& _bitmap = face->glyph->bitmap;
		// 		bitmap.height [i] = _bitmap.rows;
		// 		for ( int xx = 0 ; xx < _bitmap.width ; ++xx )
		// 		{
		// 			for ( int yy = 0 ; yy < _bitmap.rows ; ++yy )
		// 			{
		// 			unsigned char r = _bitmap.buffer[(yy*(_bitmap.width)+xx)];
		// 			bitmap.buffer[(y+yy)*bitmap.image_width*4+(x+xx)*4+0] = r;
		// 			bitmap.buffer[(y+yy)*bitmap.image_width*4+(x+xx)*4+1] = r;
		// 			bitmap.buffer[(y+yy)*bitmap.image_width*4+(x+xx)*4+2] = r;
		// 			bitmap.buffer[(y+yy)*bitmap.image_width*4+(x+xx)*4+3] = 255;
		// 			}
		// 		}
		// 	}

		// 	// bitm.buffer = buffer;
		// 	// bitm.width = image_width;
		// 	// bitm.height = image_height;
		// 	// bitm.widths = widths;
		// 	// bitm.heights = heights;
		// 	// bitm.xs = xs;
		// 	// bitm.ys = ys;
		// 	// bitm.hang = hang;
		// }
	}
	graphics (graphics&&) {}
	~graphics () {
		glfwTerminate ();
	}

private:
	vector <window> windows;
	VkInstance instance;
	PhysicalDevice physical_device;
};	




// // import Vulkan;
// // auto sem = std::binary_semaphore {0};

// auto mouse_button_callback (GLFWwindow* w, int button, int action, int mods) noexcept -> void {
// 		// printf ("m\n");
// 		if (void * p = glfwGetWindowUserPointer (w); p != nullptr) {
// 			printf ("mmm\n");
// 			auto h = std::coroutine_handle <>::from_address (p); 
// 				// if (not h.done ()) {
// 				// 	printf ("yay\n");
// 				// 	h.resume ();
// 				// 	// printf ("~m\n");
// 				// 	glfwSetWindowUserPointer (w, nullptr);
// 				// } else {
// 				// 	printf("what\n");
// 				// }
// 			h.resume ();
// 			// glfwSetWindowUserPointer (w, nullptr);
			
// 		} else {
// 			printf ("user pointer is null\n");
// 		}
		
// 		// printf ("~m\n");
// 	}




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
constexpr auto font_size = 128;
constexpr auto padding = 2;
// auto bitmap = FontBitmap3 <(font_size+padding)*16, (font_size+padding)*8> {};




auto main () -> int {
	// auto v = vector {1, 3, 5};
	// std::cout << v.size() << std::endl;
	// v += 7;
	// for (auto i : v) {
	// 	std::cout << i << std::endl;
	// 	// printf ("%i\n", i);
	// }
	// auto g = graphics {};
	// constexpr auto width = 640;
	// constexpr auto height = 480;
		// glfwInit ();
	// 	// glfwWindowHint (GLFW_VISIBLE, GLFW_FALSE);
	// 	glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
	// #if defined (MACOS)
	// 	glfwWindowHint (GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_NO_API);
	// #endif

	// auto v = vector <int> {2, 3};
	// std::cout << v.size() << std::endl;
	// auto a = array {"hej", "san"};

	// auto instance = Instance {array{"VK_LAYER_KHRONOS_validation"/*, "VK_LAYER_LUNARG_api_dump"*/}, array{"VK_KHR_portability_enumeration", "VK_KHR_surface", "VK_EXT_metal_surface", "VK_MVK_macos_surface", "VK_KHR_get_physical_device_properties2"}};
	// auto instance = Details <VkInstance> {
	// 	.layers = {"VK_LAYER_KHRONOS_validation"/*, "VK_LAYER_LUNARG_api_dump"*/},
	// 	.extensions = {"VK_KHR_portability_enumeration", "VK_KHR_surface", "VK_EXT_metal_surface", "VK_MVK_macos_surface", "VK_KHR_get_physical_device_properties2"}
	// } ();
		// auto layers = std::vector <char const*> {"VK_LAYER_KHRONOS_validation"/*, "VK_LAYER_LUNARG_api_dump"*/};
		// auto extensions = std::vector <char const*> {"VK_KHR_portability_enumeration", "VK_KHR_surface", "VK_EXT_metal_surface", "VK_MVK_macos_surface", "VK_KHR_get_physical_device_properties2"};

		// auto const createInfo = VkInstanceCreateInfo {
		// 	.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		// 	.pNext = nullptr,
		// 	 .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
		// 	.pApplicationInfo = &app_info,
		// 	.enabledLayerCount = static_cast <uint32_t> (layers.size()),
		// 	.ppEnabledLayerNames = layers.data(),
		// 	.enabledExtensionCount = static_cast <uint32_t> (extensions.size()),
		// 	.ppEnabledExtensionNames = extensions.data()
		// };

		// auto instance = VkInstance {};

		// if (vkCreateInstance (/*VkInstanceCreateInfo const * pCreateInfo */&createInfo, /*VkAllocationCallbacks const * pAllocator */nullptr, /*VkInstance* pInstance*/&instance) != VK_SUCCESS) {
		// 	printf ("error >> failed to create instance");
		// 	exit (-1);
		// }

		// auto physical_device = Details <PhysicalDevice> {
		// 	.instance = instance,
		// 	.picker = [] (std::vector <VkPhysicalDevice> const & devices) -> VkPhysicalDevice {
		// 		return devices.back ();
		// 	}
		// } ();
	// Graphics auto g = graphics_t {};
	// Window auto & w1 = g.make_window ();

	// // Window auto & w2 = g.make_window ();
	// // std::cout << g.windows_open () << std::endl;
	// while (g.windows_open ()) {
	// 	g.draw_frame ();
	// 	g.process ();
	// }
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