module;
#include <iostream>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#include <array>
#include <vector>
#include <fstream>
#include <string>
export module Vulkan;

// import std::vector;

// export import Vulkan.Instance;
// export import Vulkan.PhysicalDevice;
// export import Vulkan.Window;
// export import Vulkan.App;

export auto operator << (std::ostream& os, VkExtensionProperties const& p) noexcept -> std::ostream& {
	os << p.extensionName;
	return os;
}


/*
	Design notes


*/


export template <typename T>
struct iInstance {
	iInstance () noexcept : iInstance {{"VK_LAYER_KHRONOS_validation"}, necessary_instance_extensions ()} {
		// std::cout << "iInstance ()" << std::endl;
	}
	iInstance (std::vector <char const*> && layers, std::vector <char const*> && extensions) noexcept {
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

		// if constexpr (requires {static_cast <T*> (this) -> on_instance_finished (handle);}) {
		// 	static_cast <T*> (this) -> on_instance_finished (handle);
		// }
	}
	~iInstance () {
		// std::cout << "~iInstance ()" << std::endl;
 		if (handle) {
			vkDestroyInstance (handle, nullptr); 
		}
	}
	iInstance (iInstance&& o) noexcept : handle {VK_NULL_HANDLE} {
		std::swap (handle, o.handle);
	}
	iInstance (iInstance const&) noexcept = delete;

	auto devices () const -> std::vector <VkPhysicalDevice> {
		auto count = uint32_t {0};
		vkEnumeratePhysicalDevices (handle, &count, nullptr);
		if (count > 0) {
			auto devices = std::vector <VkPhysicalDevice> {count};
			vkEnumeratePhysicalDevices (handle, &count, devices.data());
			return devices;
		} else {
			std::cout << "error >> failed to find any physical devices" << std::endl;
			exit (-1);
			// return std::vector <VkPhysicalDevice> {0};
		}
	}
	static auto available_extension_properties () noexcept -> std::vector <VkExtensionProperties> {
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		auto extensions = std::vector <VkExtensionProperties> {extensionCount};
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
		return extensions;
	}
	static auto necessary_instance_extensions () noexcept -> std::vector <char const*> {
		auto glfwExtensionCount = uint32_t {0};
		char const** glfwExtensions = glfwGetRequiredInstanceExtensions (&glfwExtensionCount);
		#ifdef MACOS
		auto res = std::vector <char const*> {glfwExtensionCount + 1};
		res [glfwExtensionCount] = "VK_KHR_portability_enumeration";
		#else 
		auto res = std::vector <char const*> {glfwExtensionCount};
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


export template <typename T>
struct iPhysicalDevice : iInstance <iPhysicalDevice <T>> {
	// using iInstance::iInstance;
	iPhysicalDevice () noexcept {
		handle = iInstance <iPhysicalDevice <T>>::devices ().back ();
	}
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
	// auto on_instance_finished (VkInstance const& i) noexcept -> void {
	// 	// std::cout<<"mfd"<<std::endl;
	// 	handle = iInstance <iPhysicalDevice <T>>::devices ().back ();
	// 	// if constexpr (requires {static_cast <T*> (this) -> on_instance_finished (i);}) {
	// 	// 	static_cast <T*> (this) -> on_instance_finished (i);
	// 	// }
	// 	if constexpr (requires {static_cast <T*> (this) -> on_physical_device_finished (handle);}) {
	// 		static_cast <T*> (this) -> on_physical_device_finished (handle);
	// 	}
	// }
	
	// iInstance <iPhysicalDevice> instance;
	VkPhysicalDevice handle;
};

export struct iQueue {
	VkQueue handle;
};

export struct iQueueFamily {
	iQueue* handles;
	unsigned int count;
	uint32_t index;
	VkQueueFlags capabilities;
	VkBool32 present_support;

	~iQueueFamily () noexcept {
		if (handles) {
			std::free (handles);
			// std::cout<<"freeing\n";
		}
	}
};

export struct iCommandPool : iQueueFamily {
	iCommandPool () noexcept {
		
	}
private:
	VkCommandPool handle;
};

// template <typename T>
export struct iDevice : iPhysicalDevice <iDevice> {
	iDevice () noexcept {
		auto const& p = iPhysicalDevice <iDevice>::physical_device ();
		auto count = uint32_t {0};
		vkGetPhysicalDeviceQueueFamilyProperties (p, &count, nullptr);
		auto queue_family_properties = std::vector <VkQueueFamilyProperties> {count};
		vkGetPhysicalDeviceQueueFamilyProperties (p, &count, queue_family_properties.data());

		auto queue_create_infos = std::vector <VkDeviceQueueCreateInfo> {queue_family_properties.size()};
		auto indices = std::vector <uint32_t> {};
		queueFamilies.resize (queue_family_properties.size());

		auto const priority = float {1.0};

		for (auto i = 0; i < queue_family_properties.size (); ++i) {

			auto const count = queue_family_properties [i].queueCount;
			auto const family_index = static_cast <uint32_t> (i);
			auto const capabilities = queue_family_properties [i].queueFlags;

			queueFamilies[i].handles = (iQueue*) std::malloc (count * sizeof (iQueue));
			queueFamilies[i].count = count;
			// queueFamilies [i].handles.resize (count);
			queueFamilies [i].index = family_index;
			queueFamilies [i].capabilities = capabilities;

			queue_create_infos [i] = {
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.pNext = nullptr,
				.flags = 0,
				.queueFamilyIndex = family_index,
				.queueCount = count,
				.pQueuePriorities = &priority
			};
		}

		constexpr auto dynamic_rendering_features = VkPhysicalDeviceDynamicRenderingFeaturesKHR {
			.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR,
			.dynamicRendering = VK_TRUE
		};

		auto extensions = std::array <char const*, 1> {"VK_KHR_portability_subset"};

		auto const createInfo = VkDeviceCreateInfo {
			.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
			.pNext = &dynamic_rendering_features,
			.flags = 0,
			.queueCreateInfoCount = static_cast <uint32_t> (queue_create_infos.size()),
			.pQueueCreateInfos = queue_create_infos.data(),
			.enabledLayerCount = 0,//static_cast <uint32_t> (layers.size()),
			.ppEnabledLayerNames = nullptr,//layers.data(),
			.enabledExtensionCount = static_cast <uint32_t> (extensions.size()),
			.ppEnabledExtensionNames = extensions.data(),
			// .pEnabledFeatures = &features
		};

		// auto device = VkDevice {};
		
		if (vkCreateDevice (parent::physical_device(), &createInfo, nullptr, &handle) != VK_SUCCESS) {
			std::cout << "error >> failed to create device" << std::endl;
			exit (-1);
		}

		for (auto i = 0; i < queueFamilies.size(); ++i) {
			for (auto j = 0; j < queueFamilies[i].count; ++j) {
				vkGetDeviceQueue (handle, queueFamilies[i].index, static_cast <uint32_t> (j), &queueFamilies[i].handles [j].handle);
				// queueFamilies[i].present_support = physical_device.get_surface_support (queueFamilies[i].index, surface);

				auto const command_pool_create_info = VkCommandPoolCreateInfo {
					.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
					.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
					.queueFamilyIndex = queueFamilies[i].index
				};

				// if (vkCreateCommandPool(device, &command_pool_create_info, nullptr, &queue_family.command_pool.handle) != VK_SUCCESS) {
				// 	std::cout << "error >> failed to create command pool" << std::endl;
				// 	exit (-1);
				// }
			}
		}
	}
	~iDevice () noexcept {
		vkDestroyDevice (handle, nullptr); 
		// std::cout << "~iDevice ()" << std::endl;
	}
protected:
	auto device () const noexcept -> VkDevice const& {
		return handle;
	}
private:
	using parent = iPhysicalDevice <iDevice>;
	std::vector <iQueueFamily> queueFamilies;
	template <typename U>
	friend struct iPhysicalDevice;
	auto on_physical_device_finished (VkPhysicalDevice const& p) noexcept -> void {

		// if constexpr (requires {static_cast <T*> (this) -> on_device_finished (handle);}) {
		// 	static_cast <T*> (this) -> on_device_finished (handle);
		// }
	}
	VkDevice handle;
};

export struct iResources : iDevice {
	
};

export struct iRenderPass : iDevice {

};

export struct iGraphicsPipeline : iRenderPass {

};

/*
	The compute pipeline descibes its buffer usage needs 
	in the compute shader as a pipeline layout, which 
	contains multiple description set layouts.
*/
export struct iComputePipeline : iResources {
	iComputePipeline (char const* shader_path) noexcept {
		// auto const shader_stage_create_info = VkPipelineShaderStageCreateInfo {
		// 	VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		// 	nullptr,
		// 	0,
		// 	VK_SHADER_STAGE_COMPUTE_BIT,
		// 	// make_shader_module (read_byte_file (shader_path))
		// 	createShaderModule (readFile (shader_path)),
		// 	"main"
		// };

		VkDescriptorSetLayoutBinding descriptor_set_layout_bindings [2] = {
			{
				0,
				VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
				1,
				VK_SHADER_STAGE_COMPUTE_BIT,
				0
			},
			{
				1,
				VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
				1,
				VK_SHADER_STAGE_COMPUTE_BIT,
				0
			}
		};

		auto const descriptor_set_layout_create_info = VkDescriptorSetLayoutCreateInfo {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.bindingCount = 2,
			.pBindings = descriptor_set_layout_bindings
		};

		if (vkCreateDescriptorSetLayout (device (), &descriptor_set_layout_create_info, 0, &descriptor_set_layout) != VK_SUCCESS) {
			std::cout << "error << failed to create descriptor set layout" << std::endl;
			exit (-1);
		}

		auto const layout_create_info = VkPipelineLayoutCreateInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.setLayoutCount = 1,
			.pSetLayouts = &descriptor_set_layout,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = nullptr
		};

		if (vkCreatePipelineLayout (device (), &layout_create_info, 0, &layout)) {
			std::cout << "error >> failed to create pipeline layout" << std::endl;
			exit (-1);
		}

		VkShaderModule compute_shader_module = createShaderModule (readFile (shader_path));

		auto const create_info = VkComputePipelineCreateInfo {
			.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			// .stage = shader_stage_create_info,
			.stage = VkPipelineShaderStageCreateInfo {
				VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
				nullptr,
				0,
				VK_SHADER_STAGE_COMPUTE_BIT,
				// make_shader_module (read_byte_file (shader_path))
				compute_shader_module,
				"main",
				0
			},
			.layout = layout,
			.basePipelineHandle = 0,
			.basePipelineIndex = 0
		};

		if (vkCreateComputePipelines (device (), 0, 1, &create_info, 0, &handle) != VK_SUCCESS) {
			std::cout << "error >> failed to create compute pipeline" << std::endl;
			exit (-1);
		}

		vkDestroyShaderModule (device (), compute_shader_module, nullptr);
	}
	~iComputePipeline () noexcept {
		auto const cache_create_info = VkPipelineCacheCreateInfo {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO,
			.pNext = nullptr,
			.flags = 0,
			.initialDataSize = 0,
			.pInitialData = nullptr
		};

		if (vkCreatePipelineCache (device (), &cache_create_info, nullptr, &cache) != VK_SUCCESS) {
			std::cout << "error >> failed to create pipeline cache" << std::endl;
			exit (-1);
		}

		saveCacheToFile ("Vulkan.Pipeline.Cache", cache);
		vkDestroyPipelineCache (device (), cache, nullptr);


		vkDestroyDescriptorSetLayout (device (), descriptor_set_layout, nullptr);
		vkDestroyPipelineLayout (device (), layout, nullptr);
		vkDestroyPipeline (device (), handle, nullptr);
	}
private:
	static std::vector<char> readFile(const std::string& filename) noexcept {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if (!file.is_open()) {
			std::cout << "error >> failed to open file" << std::endl;
			exit (-1);
		}
		size_t fileSize = (size_t) file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();
		return buffer;
	}
	VkShaderModule createShaderModule (const std::vector<char>& code) noexcept {
		VkShaderModuleCreateInfo createInfo {
			.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
			.codeSize = code.size(),
			.pCode = reinterpret_cast<const uint32_t*>(code.data())
		};

		VkShaderModule shaderModule;

		if (vkCreateShaderModule(device (), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			std::cout << "failed to create shader module!" << std::endl;
			exit (-1);
		}
		return shaderModule;
	}

	void saveCacheToFile (char const* filename, VkPipelineCache cache) noexcept {
		auto cacheDataSize = size_t {};
		// Determine the size of the cache data.
		auto result = vkGetPipelineCacheData (device (), cache, &cacheDataSize, nullptr);

		if (result == VK_SUCCESS && cacheDataSize != 0) {
			FILE* pOutputFile;
			void* pData;
			// Allocate a temporary store for the cache data.
			result = VK_ERROR_OUT_OF_HOST_MEMORY;
			pData = malloc (cacheDataSize);
			if (pData != nullptr){
				// Retrieve the actual data from the cache.
				result = vkGetPipelineCacheData (device (),cache,&cacheDataSize,pData);
				if (result == VK_SUCCESS) {
					// Open the file and write the data to it.
					pOutputFile = fopen (filename, "wb");
					if (pOutputFile != nullptr) {
						fwrite (pData, 1, cacheDataSize, pOutputFile);
						fclose (pOutputFile);
					}
					free (pData);
				}
			}
		}
	}

	VkDescriptorSetLayout descriptor_set_layout;
	VkPipelineLayout layout;
	VkPipeline handle;
	VkPipelineCache cache;
};

// export import Vulkan.Instance;
// import Vulkan.Instance.Impl;