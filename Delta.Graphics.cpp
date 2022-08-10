#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
export module Delta.Graphics;
import Delta.Range;
import std;

export auto get_surface_capabilities (VkPhysicalDevice const & device, VkSurfaceKHR const & surface) noexcept -> VkSurfaceCapabilitiesKHR {
	
	auto capabilities = VkSurfaceCapabilitiesKHR {};

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR (device, surface, &capabilities);

	return capabilities;
}

export auto get_surface_presentation_modes (VkPhysicalDevice const & device, VkSurfaceKHR const & surface) noexcept -> std::vector <VkPresentModeKHR> {
	
	auto count = uint32_t {};
	
	vkGetPhysicalDeviceSurfacePresentModesKHR (device, surface, &count, nullptr);

	if (count != 0) {
		auto modes = std::vector <VkPresentModeKHR> {count};
		vkGetPhysicalDeviceSurfacePresentModesKHR (device, surface, &count, modes.data ());
		return modes;
	} else {
		return {};
	}
}

export auto get_surface_formats (VkPhysicalDevice const & device, VkSurfaceKHR const & surface) noexcept -> std::vector <VkSurfaceFormatKHR> {
	
	auto count = uint32_t {};
	
	vkGetPhysicalDeviceSurfaceFormatsKHR (device, surface, &count, nullptr);

	if (count != 0) {
		auto formats = std::vector <VkSurfaceFormatKHR> {count};
		vkGetPhysicalDeviceSurfaceFormatsKHR (device, surface, &count, formats.data ());
		return formats;
	} else {
		return {};
	}
}

// export auto make_render_pass_with_framebuffers (VkExtent2D const & extent, VkFormat const & format, VkDevice const & device) noexcept -> std::tuple <>

export auto make_shader_module (std::vector <char> && code, VkDevice const & device) noexcept -> VkShaderModule {
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

export auto read_byte_file (char const * path) noexcept -> std::vector <char> {
	auto file = std::ifstream {path, std::ios::ate | std::ios::binary};

	if (!file.is_open ()) {
		std::cout << "error >> failed to open file" << std::endl;
		exit (-1);
	}

	auto file_size = file.tellg ();
	auto buffer = std::vector <char> {};
	buffer.resize (file_size);
	file.seekg (0);
	file.read (buffer.data (), file_size);
	file.close ();
	return buffer;
}

export auto make_shader_module (char const * path, VkDevice const & device) noexcept -> VkShaderModule {
	return make_shader_module (read_byte_file (path), device);
}


/*
	get layers available to instances
*/
export auto get_instance_layer_properties (
	) noexcept -> std::vector <VkLayerProperties> {
	auto count = uint32_t {0};
	vkEnumerateInstanceLayerProperties (&count, nullptr);
	auto layer_properties = std::vector <VkLayerProperties> {count};
	vkEnumerateInstanceLayerProperties (&count, layer_properties.data());
	return layer_properties;
}

/*
	Get layer properties available to a physical device
*/
export auto get_layer_properties (
	VkPhysicalDevice const& device) noexcept -> std::vector <VkLayerProperties> {
	auto count = uint32_t {0};
	vkEnumerateDeviceLayerProperties (device, &count, nullptr);
	auto layer_properties = std::vector <VkLayerProperties> {count};
	vkEnumerateDeviceLayerProperties (device, &count, layer_properties.data());
	return layer_properties;
}

/*
	Get available extensions to instances
*/
export auto get_instance_extension_properties (
	) noexcept -> std::vector <VkExtensionProperties> {
	auto count = uint32_t {0};
	vkEnumerateInstanceExtensionProperties (nullptr, &count, nullptr);
	auto extensions_properties = std::vector <VkExtensionProperties> {count};
	vkEnumerateInstanceExtensionProperties (nullptr, &count, extensions_properties.data());
	return extensions_properties;
}

/*
	get extension properties available to a physical device
*/
export auto get_extension_properties (
	VkPhysicalDevice const& device) noexcept -> std::vector <VkExtensionProperties> {
	auto count = uint32_t {0};
	vkEnumerateDeviceExtensionProperties (device, nullptr, &count, nullptr);
	auto extension_properties = std::vector <VkExtensionProperties> {count};
	vkEnumerateDeviceExtensionProperties (device, nullptr, &count, extension_properties.data());
	return extension_properties;
}

using swapchain_details_t = std::tuple <VkSurfaceCapabilitiesKHR, std::vector <VkSurfaceFormatKHR>, std::vector <VkPresentModeKHR>>;




export auto get_swapchain_details (
	VkPhysicalDevice const & device,
	VkSurfaceKHR const & surface) noexcept -> swapchain_details_t {
		
		auto details = swapchain_details_t {};

		auto & [capabilities, formats, present_modes] = details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR (device, surface, &capabilities);


		auto count = uint32_t {};


		vkGetPhysicalDeviceSurfaceFormatsKHR (device, surface, &count, nullptr);

		if (count != 0) {
			formats.resize (count);
			vkGetPhysicalDeviceSurfaceFormatsKHR (device, surface, &count, formats.data ());
		}


		vkGetPhysicalDeviceSurfacePresentModesKHR (device, surface, &count, nullptr);

		if (count != 0) {
			present_modes.resize (count);
			vkGetPhysicalDeviceSurfacePresentModesKHR (device, surface, &count, present_modes.data ());
		}


		return details;
}

/*
	Takes a vector of layer properties and makes a vector of layer names out of it
*/
export auto make_names (
	std::vector <VkLayerProperties> const& layers_properties) noexcept -> std::vector <char const*> {
	auto layer_names = std::vector <char const *> {layers_properties.size()};
	for (auto i = 0; i < layer_names.size(); ++i) {
		layer_names [i] = layers_properties [i].layerName;
	}
	return layer_names;
}

/*
	Takes a vector of extension properties and makes a vector of extension names out of it
*/
export auto make_names (
	std::vector <VkExtensionProperties> const& extension_properties) noexcept -> std::vector <char const*> {
	auto extension_names = std::vector <char const *> {extension_properties.size()};
	for (auto i = 0; i < extension_names.size(); ++i) {
		extension_names [i] = extension_properties [i].extensionName;
	}
	return extension_names;
}


export auto make_names (auto const& arg, auto (*fun) (decltype (arg))) 
{
	return make_names (fun (arg));
};

export auto make_names (auto const& arg, auto (*fun) (decltype (arg)), auto... rest) //-> decltype ()
{
	return std::make_tuple (make_names (arg, fun), make_names (arg, rest)...);
};

export auto make_app_info (
	char const * app_name = "app", 
	uint32_t app_version = VK_MAKE_VERSION (1, 0, 0),
	char const* engine_name = "engine", 
	uint32_t engine_version = VK_MAKE_VERSION (1, 0, 0), 
	uint32_t api_version = VK_MAKE_VERSION (1, 3, 0)) noexcept -> VkApplicationInfo {

	return {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = nullptr,
		.pApplicationName = app_name,
		.applicationVersion = app_version,
		.pEngineName = engine_name,
		.engineVersion = engine_version,
		.apiVersion = api_version
	};
}

export auto get_devices (
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

export auto get_properties (
	VkPhysicalDevice const& device) noexcept -> VkPhysicalDeviceProperties {
	auto properties = VkPhysicalDeviceProperties {};
	vkGetPhysicalDeviceProperties (device, &properties);
	return properties;
}

export auto get_features (
	VkPhysicalDevice const& device) noexcept -> VkPhysicalDeviceFeatures {
	auto features = VkPhysicalDeviceFeatures {};
	vkGetPhysicalDeviceFeatures (device, &features);
	return features;
}

export auto get_memory_properties (
	VkPhysicalDevice const& device) noexcept -> VkPhysicalDeviceMemoryProperties {
	auto properties = VkPhysicalDeviceMemoryProperties {};
	vkGetPhysicalDeviceMemoryProperties (device, &properties);
	return properties;
}

export auto get_queue_family_properties (
	VkPhysicalDevice const& device) noexcept -> std::vector <VkQueueFamilyProperties> {
	auto count = uint32_t {0};
	vkGetPhysicalDeviceQueueFamilyProperties (device, &count, nullptr);
	auto queue_family_properties = std::vector <VkQueueFamilyProperties> {count};
	vkGetPhysicalDeviceQueueFamilyProperties (device, &count, queue_family_properties.data());
	return queue_family_properties;
}

export template <typename... T> 
struct Details;

export struct Window {
	GLFWwindow * handle;
	
	auto get_extent () const noexcept -> VkExtent2D {
		int width, height;
		glfwGetFramebufferSize(handle, &width, &height);

		return {
			.width = static_cast <uint32_t> (width),
			.height = static_cast <uint32_t> (height)
		};
	}
	auto should_close () const noexcept -> bool {
		return glfwWindowShouldClose (handle);
	}
	auto destroy () noexcept -> void {
		glfwDestroyWindow (handle);
	}

	auto set_resize_callback (void (*callback) (GLFWwindow* window, int width, int height)) {
		glfwSetFramebufferSizeCallback (handle, callback);
	}
};

export template <>
struct Details <Window> {
	VkExtent2D const & extent;
	char const * title = "";
	std::vector <std::tuple <int, int>> hints;

	auto operator () () noexcept -> Window {
		for (auto const [type, value] : hints) {
			glfwWindowHint (type, value);
		}
	
		return {.handle = glfwCreateWindow (extent.width, extent.height, title, nullptr, nullptr)};
	}
};

export template <>
struct Details <VkInstance> {
	std::vector <char const*> layers;
	std::vector <char const*> extensions;
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


export template <>
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

export struct PhysicalDevice {

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

export template <>
struct Details <PhysicalDevice> {
	VkInstance const & instance;
	VkPhysicalDevice (*picker) (std::vector <VkPhysicalDevice> const &);

	auto operator () () noexcept -> PhysicalDevice {
		return {picker (get_devices (instance))};
	}
};

export struct CommandBuffer {
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
	// auto push_constants (VkPipelineLayout const & layout, VkShaderStageFlags const & flags)

	
};

export struct CommandPool {
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

export struct Queue {
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

export struct QueueFamily {
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

export struct LogicalDevice {
	PhysicalDevice const & physical_device;
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

export template <>
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

export template <>
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

export template <>
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

export template <>
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

export template <>
struct Details <LogicalDevice> {
	PhysicalDevice const & physical_device;
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

export auto get_window_extent (GLFWwindow * window) noexcept -> VkExtent2D {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	return {
		.width = static_cast <uint32_t> (width),
		.height = static_cast <uint32_t> (height)
	};
}


export struct Swapchain {
	VkDevice const & device;
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

export template <>
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
		s.destroy ();
		auto new_s = this->operator()();
		std::swap (s.handle, new_s.handle);
		std::swap (s.images, new_s.images);
		std::swap (s.views, new_s.views);
		std::swap (s.image_extent, new_s.image_extent);
		std::swap (s.framebuffers, new_s.framebuffers);
	}
};

export struct RenderPass {
	VkDevice const & device;
	VkRenderPass handle;

	auto destroy () noexcept -> void {
		vkDestroyRenderPass (device, handle, nullptr);
	}
};

export template <>
struct Details <RenderPass> {
	VkDevice const & device;
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

export struct GraphicsPipeline {
	LogicalDevice const & device;
	VkPipeline handle;
	VkDescriptorSetLayout descriptor_set_layout;
	VkPipelineLayout layout;

	auto destroy () noexcept -> void {
		vkDestroyDescriptorSetLayout (device.handle, descriptor_set_layout, nullptr);
		vkDestroyPipelineLayout (device.handle, layout, nullptr);
		vkDestroyPipeline (device.handle, handle, nullptr);
	}
};

export struct Vertex {
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 tex_coord;
};

export struct mvp_push_constants {
	alignas (16) glm::mat4 model;
	alignas (16) glm::mat4 view;
	alignas (16) glm::mat4 proj;
};

export template <>
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
			shader_modules [i] = make_shader_module (path, device.handle);
			shader_create_infos [i] = {
				.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				nullptr,
				{},
				stage,
				shader_modules [i],
				"main"
			};
		}
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
			.offset = 0,
			.size = sizeof (mvp_push_constants),
			.stageFlags = VK_SHADER_STAGE_VERTEX_BIT
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

export struct DeviceMemory {
	LogicalDevice const & device;
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

export template <>
struct Details <DeviceMemory> {
	PhysicalDevice const & physical_device;
	LogicalDevice const & device;
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

export struct Buffer {
	LogicalDevice const & device;
	VkBuffer handle;	

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

export template <>
struct Details <Buffer> {
	LogicalDevice const & device;
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

		return {device, buffer};
	}
};

export struct Image {
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

export template <>
struct Details <Image> {
	PhysicalDevice const & physical_device;
	LogicalDevice const & device;
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
			.imageType = VK_IMAGE_TYPE_2D,
			.extent.width = static_cast<uint32_t>(texWidth),
			.extent.height = static_cast<uint32_t>(texHeight),
			.extent.depth = 1,
			.mipLevels = 1,
			.arrayLayers = 1,
			.format = format,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.flags = 0
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
			.srcAccessMask = 0,
			.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT
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


export struct Details_image2 {
	PhysicalDevice const & physical_device;
	LogicalDevice const & device;
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
			.imageType = VK_IMAGE_TYPE_2D,
			.extent.width = static_cast<uint32_t>(tex_width),
			.extent.height = static_cast<uint32_t>(tex_height),
			.extent.depth = 1,
			.mipLevels = 1,
			.arrayLayers = 1,
			.format = format,
			.tiling = VK_IMAGE_TILING_OPTIMAL,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.flags = 0
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
			.srcAccessMask = 0,
			.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT
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



export struct Sampler {
	LogicalDevice const & device;
	VkSampler handle;

	auto destroy () const noexcept -> void {
		vkDestroySampler (device.handle, handle, nullptr);
	}
};

export template <>
struct Details <Sampler> {
	LogicalDevice const & device;

	auto operator () () const noexcept -> Sampler {
		auto const create_info = VkSamplerCreateInfo {
			.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
			.magFilter = VK_FILTER_LINEAR,
			.minFilter = VK_FILTER_LINEAR,
			.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
			.anisotropyEnable = VK_TRUE,
			.maxAnisotropy = device.physical_device.get_properties().limits.maxSamplerAnisotropy,
			.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
			.unnormalizedCoordinates = VK_FALSE,
			.compareEnable = VK_FALSE,
			.compareOp = VK_COMPARE_OP_ALWAYS,
			.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
			.mipLodBias = 0.0f,
			.minLod = 0.0f,
			.maxLod = 0.0f
		};

		auto sampler = VkSampler {};

		if (vkCreateSampler (device.handle, &create_info, nullptr, &sampler) != VK_SUCCESS) {
			std::cout << "error >> failed to create sampler" << std::endl;
			exit (-1);
		}

		return {.device = device, .handle = sampler};
	}
};

export struct DescriptorPool {
	LogicalDevice const & device;
	VkDescriptorPool handle;

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

export template <>
struct Details <DescriptorPool> {
	LogicalDevice const & device;
	std::vector <VkDescriptorPoolSize> pool_sizes;
	uint32_t const & max_sets;

	auto operator () () const noexcept -> DescriptorPool {

		auto const create_info = VkDescriptorPoolCreateInfo {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
			.poolSizeCount = (uint32_t) pool_sizes.size(),
			.pPoolSizes = pool_sizes.data(),
			.maxSets = max_sets,
		};

		auto handle = VkDescriptorPool {};

		if (vkCreateDescriptorPool (device.handle, &create_info, nullptr, &handle) != VK_SUCCESS) {
			std::cout << "error >> failed to create descriptor pool" << std::endl;
			exit (-1);
		}

		return {.device = device, .handle = handle};
	}
};