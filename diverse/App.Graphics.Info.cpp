#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
import Delta;

template <typename T, typename U>
concept Bigger_than = sizeof (T) > sizeof (U);

// color depth
auto choose_surface_format (VkPhysicalDevice const & device, VkSurfaceKHR const & surface) noexcept -> VkSurfaceFormatKHR { 
	
	for (auto const & i : get_surface_formats (device, surface)) {
		if (i.format == VK_FORMAT_B8G8R8A8_SRGB and i.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return i;
		}
	}

	std::cout << "error >> failed to find a suitable surface format" << std::endl;
	exit (-1);
};

auto choose_surface_presentation_mode (VkPhysicalDevice const & device, VkSurfaceKHR const & surface) noexcept -> VkPresentModeKHR { 
	
	for (auto const & i : get_surface_presentation_modes (device, surface)) {
		if (i == VK_PRESENT_MODE_MAILBOX_KHR) {
			return i;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
};

auto choose_surface_extent (VkPhysicalDevice const & device, VkSurfaceKHR const & surface, GLFWwindow * window) noexcept -> VkExtent2D { 
	
	auto const capabilities = get_surface_capabilities (device, surface);

	auto width = int {};
	auto height = int {};

	glfwGetFramebufferSize (window, &width, &height);

	return {(uint32_t) width, (uint32_t) height};

	std::cout << "error >> failed to find a suitable surface extent" << std::endl;
	exit (-1);
};


auto main (int argc, char** argv) -> int {

	glfwInit ();

	glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint (GLFW_RESIZABLE, GLFW_FALSE);
	auto * window = make_window (640, 480);
	
	auto instance = make_instance (
		/*layers*/{//VK_LAYER_LUNARG_api_dump", 
		"VK_LAYER_KHRONOS_validation"},
		/*extensions*/{"VK_KHR_surface",
		"VK_EXT_debug_utils",
		"VK_EXT_metal_surface",
		"VK_MVK_macos_surface"});

	auto const surface = make_surface (instance, window);

	auto const physical_device = get_devices (instance).front ();
	
	auto const [device, queues, command_pools] = make_device (physical_device, /*layers*/{}, /*extensions*/{"VK_KHR_swapchain", "VK_KHR_portability_subset"}, /*features*/{});

	auto && indices = std::vector <uint32_t> {};
	indices.resize (queues.size ());
	std::iota (indices.begin (), indices.end (), 0);

	auto present_support = VkBool32 {false};
	vkGetPhysicalDeviceSurfaceSupportKHR (physical_device, indices.front (), surface, &present_support);
		

	auto const [swapchain, images, image_format, extent] = make_swapchain (
		device, 
		surface, 
		std::move (indices), 
		get_surface_capabilities (physical_device, surface), 
		choose_surface_format (physical_device, surface), 
		choose_surface_extent (physical_device, surface, window), 
		choose_surface_presentation_mode (physical_device, surface)
	);

	auto const views = make (
		image_view_details {
			.format = image_format
		}, 
		images, 
		device
	);

	auto const [render_pass, framebuffers] = make (
		render_pass_details {

		}
	);

	auto const render_pass = make_render_pass (image_format, device);

	

	// auto ss = graphics_pipeline_details {
	// 	.vert_shader_module = make_shader_module ("App.Graphics.Info.vert.spv", device),
	// 	.frag_shader_module = make_shader_module ("App.Graphics.Info.frag.spv", device),
	// 	.rasterization_state.depthBiasEnable = VK_TRUE
	// };

	auto const [graphics_pipeline, graphics_pipeline_layout] = make (
		extent, 
		{	.vert_shader_module = make_shader_module ("App.Graphics.Info.vert.spv", device),
			.frag_shader_module = make_shader_module ("App.Graphics.Info.frag.spv", device),
		}, 
		render_pass, 
		device
	);
	

	auto const get_memory_type_index = [physical_device, properties = get_memory_properties (physical_device)] 
										(VkDeviceSize const& size, 
										VkMemoryPropertyFlags const& flags) noexcept -> uint32_t {
		for (auto i = 0; i < properties.memoryTypeCount; ++i) {
			if ((properties.memoryTypes[i].propertyFlags & flags) && (size < properties.memoryHeaps [properties.memoryTypes[i].heapIndex].size)) {
				return static_cast <uint32_t> (i);
			}
		}
		std::cout << "error >> failed to find memeory type index" << std::endl;
		exit (-1);
	};

	auto const allocate_device_memory = [&d = device, &get_memory_type_index] (VkDeviceSize const& size, VkMemoryPropertyFlags const& flags) noexcept -> VkDeviceMemory {

		auto const info = VkMemoryAllocateInfo {
			.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
			.pNext = nullptr,
			.allocationSize = size,
			.memoryTypeIndex = get_memory_type_index (size, flags)
		};
		
		auto memory = VkDeviceMemory {};

		if (vkAllocateMemory (d, &info, nullptr, &memory) != VK_SUCCESS) {
			std::cout << "error >> failed to allocate device memory" << std::endl;
			exit (-1);
		}

		return memory;
	};

	




	auto mem = allocate_device_memory (1024, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);





	auto const destroy = [&instance, &d = device] <typename T> (T a) noexcept -> void {
		if constexpr (std::is_same_v <T, VkInstance>) {
			std::cout << "cleanup >> instance" << std::endl;
			vkDestroyInstance (instance, nullptr);
		} else if constexpr (std::is_same_v <T, VkDevice>) {
			std::cout << "cleanup >> device" << std::endl;
			vkDeviceWaitIdle (d);
			vkDestroyDevice (d, nullptr);
		} else if constexpr (std::is_same_v <T, std::vector <VkCommandPool>>) {
			std::cout << "cleanup >> command pools" << std::endl;
			for (auto const & i : a) {
				vkDestroyCommandPool (d, i, nullptr);
			}
		} else if constexpr (std::is_same_v <T, GLFWwindow*>) {
			std::cout << "cleanup >> window" << std::endl;
			glfwDestroyWindow (a);
		} else if constexpr (std::is_same_v <T, VkSurfaceKHR>) {
			std::cout << "cleanup >> window surface" << std::endl;
			vkDestroySurfaceKHR (instance, a, nullptr);
		} else if constexpr (std::is_same_v <T, VkSwapchainKHR>) {
			std::cout << "cleanup >> swapchain" << std::endl;
			vkDestroySwapchainKHR (d, a, nullptr);
		} else if constexpr (std::is_same_v <T, std::vector <VkImageView>>) {
			std::cout << "cleanup >> image views" << std::endl;
			for (auto & i : a)
			vkDestroyImageView (d, i, nullptr);
		} else if constexpr (std::is_same_v <T, VkRenderPass>) {
			std::cout << "cleanup >> render pass" << std::endl;
			vkDestroyRenderPass (d, a, nullptr);
		} else if constexpr (std::is_same_v <T, VkPipelineLayout>) {
			std::cout << "cleanup >> pipeline layout" << std::endl;
			vkDestroyPipelineLayout (d, a, nullptr);
		} else if constexpr (std::is_same_v <T, VkPipeline>) {
			std::cout << "cleanup >> pipeline" << std::endl;
			vkDestroyPipeline (d, a, nullptr);
		} 
	};

	auto const free = [&d = device] <typename T> (T & mem) noexcept -> void {
		if constexpr (std::is_same_v <T, VkDeviceMemory>) {
			vkFreeMemory (d, mem, nullptr);
		}
	};


	// main loop
	// while (not glfwWindowShouldClose (window)) {
	// 	glfwPollEvents ();
	// }
	destroy (graphics_pipeline);
	destroy (graphics_pipeline_layout);
	destroy (render_pass);
	destroy (command_pools);
	free (mem);
	destroy (views);
	destroy (swapchain);
	destroy (device);
	destroy (surface);

	destroy (instance);
	destroy (window);


	glfwTerminate ();




	
	// auto p = jus
	// get_physical_devices ();
	// get_physical_devices ();
	// auto const 


	// auto get_physical_devices = make_physical_devices_getter (instance);

	// auto const & devices = get_physical_devices ();

	// auto physical_device =  
	
	// auto layer_properties = get_layer_properties ();
	// for (auto i = 0; i < layer_properties.size(); ++i) {
	// 	std::cout << "layer properties [" << i << "] >> " << layer_properties [i].layerName << std::endl; // " >> " << layers_properties [i].description << std::endl;
	// }
	// auto layer_names = make_names (layer_properties);
	// auto extension_properties = get_extension_properties ();
	// for (auto& i : extension_properties) {
	// 	std::cout << i.extensionName << std::endl;
	// }
	// for (auto& i : layer_names) std::cout << i << std::endl;
	// return 0;
	// auto instance = make_instance ();
	// auto physical_device = [&instance] () noexcept {
	// 	auto available = get_devices (instance);
	// 	if (available.size() == 0) {
	// 		std::cout << "error >> failed to create instance" << std::endl;
	// 		exit (-1);
	// 	}
	// 	for (auto const& device : available) {
	// 		auto properties = get_properties (device);
	// 		std::cout << "gpu >> " << properties.deviceName << std::endl;
	// 		std::cout << "type >> ";
	// 		if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) {
	// 			std::cout << "integrated" << std::endl;
	// 		} else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
	// 			std::cout << "discrete" << std::endl;
	// 		}
	// 		auto features = get_features (device);
	// 		auto memory_properties = get_memory_properties (device);
	// 		std::cout << "memory types >> " << memory_properties.memoryTypeCount << std::endl;
	// 		std::cout << "memory heaps >> " << memory_properties.memoryHeapCount << std::endl;
	// 		auto queue_family_properties = get_queue_family_properties (device);
	// 		std::cout << "queue families >> " << queue_family_properties.size() << std::endl;
	// 		for (auto i = 0; i < queue_family_properties.size(); ++i) {
	// 			std::cout << "queue family [" << i << "] >>";
	// 			if (queue_family_properties [i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
	// 				std::cout << " graphics";
	// 			}
	// 			if (queue_family_properties [i].queueFlags & VK_QUEUE_COMPUTE_BIT) {
	// 				std::cout << " compute";
	// 			}
	// 			if (queue_family_properties [i].queueFlags & VK_QUEUE_TRANSFER_BIT) {
	// 				std::cout << " transfer";
	// 			}
	// 			if (queue_family_properties [i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
	// 				std::cout << " binding";
	// 			}
	// 			std::cout << std::endl << "count >> " << queue_family_properties [i].queueCount << std::endl;
	// 		}
	// 	}
	// 	return available.front();
	// } ();
	// auto queue_family_properties = get_queue_family_properties (physical_device);
	// auto queue_create_infos = make_queue_create_infos (queue_family_properties);

	// for (auto& i : queue_create_infos) {
		// std::cout << i.
	// }
	// auto device = make_device (physical_device, queue_create_infos, {}, {}, get_features (physical_device));
	// auto queues = 
	
	// auto [a, b]  = make_names (physical_device, get_layer_properties, get_extension_properties);
	// auto [c, d, e] = make_names (physical_device, get_layer_properties, get_extension_properties, get_layer_properties);

	// auto [device, indexed_queues] = make_device2 (
	// 	physical_device, 
	// 	queue_family_properties,
	// 	[](VkPhysicalDevice device){
	// 		auto count = uint32_t {0};
	// 		vkEnumerateDeviceLayerProperties (device, &count, nullptr);
	// 		auto layer_properties = std::vector <VkLayerProperties> {count};
	// 		vkEnumerateDeviceLayerProperties (device, &count, layer_properties.data());
	// 		return layer_properties;
	// 	},
	// 	get_extension_properties,
	// 	make_names,
	// 	get_features);


	// auto [device, indexed_queues] = make_device (
	// 	physical_device, 
	// 	queue_family_properties,
	// 	make_names (get_layer_properties (physical_device)), 
	// 	make_names (get_extension_properties (physical_device)), 
	// 	get_features (physical_device));



	// destroy (device);
	// destroy (instance);

	// auto physical_device = choose_physical_device = [&instance, &get_physical_devices] () noexcept {
	// 	auto available = get_physical_devices ();
	// 	if (available.size() == 0) {
	// 		std::cout << "error >> failed to create instance" << std::endl;
	// 		exit (-1);
	// 	}

	// } ();
	// auto physical_devices = 

	return 0;
}