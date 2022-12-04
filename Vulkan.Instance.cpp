module;
#include <iostream>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#include <utility>
export module Vulkan.Instance;

export struct vInstance {
	vInstance (std::vector <char const*> && layers, std::vector <char const*> && extensions) {
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
			.flags = 0,//VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
			.pApplicationInfo = &app_info,
			.enabledLayerCount = static_cast <uint32_t> (layers.size()),
			.ppEnabledLayerNames = layers.data(),
			.enabledExtensionCount = static_cast <uint32_t> (extensions.size()),
			.ppEnabledExtensionNames = extensions.data()
		};
		

		if (vkCreateInstance (/*VkInstanceCreateInfo const * pCreateInfo */&createInfo, /*VkAllocationCallbacks const * pAllocator */nullptr, /*VkInstance* pInstance*/&handle) != VK_SUCCESS) {
			std::cout << "error >> failed to create instance" << std::endl;
			exit (-1);
		}
	}
	~vInstance () {
		std::cout << "~vInstance ()" << std::endl;
 		if (handle) {
			vkDestroyInstance (handle, nullptr); 
		}
	}
	vInstance (vInstance&& o) noexcept : handle {VK_NULL_HANDLE} {
		std::swap (handle, o.handle);
	}
	vInstance (vInstance const&) noexcept = delete;

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

private:
	VkInstance handle;
};