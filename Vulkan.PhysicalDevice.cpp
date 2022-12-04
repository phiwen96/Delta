module;
#include <iostream>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
export module Vulkan.PhysicalDevice;

import Vulkan.Instance;

export struct vPhysicalDevice {
	// vPhysicalDevice (vInstance& h) : handle {h.devices().back()} {}
	vPhysicalDevice (VkPhysicalDevice const& h) : handle {h} {}
	auto properties () const -> VkPhysicalDeviceProperties {
		auto res = VkPhysicalDeviceProperties {};
		vkGetPhysicalDeviceProperties (handle, &res);
		return res;
	}
	auto features () const -> VkPhysicalDeviceFeatures {
		auto res = VkPhysicalDeviceFeatures {};
		vkGetPhysicalDeviceFeatures (handle, &res);
		return res;
	}
	auto memory_properties () noexcept -> VkPhysicalDeviceMemoryProperties {
		auto res = VkPhysicalDeviceMemoryProperties {};
		vkGetPhysicalDeviceMemoryProperties (handle, &res);
		return res;
	}
	auto queue_family_properties () noexcept -> std::vector <VkQueueFamilyProperties> {
		auto count = uint32_t {0};
		vkGetPhysicalDeviceQueueFamilyProperties (handle, &count, nullptr);
		auto res = std::vector <VkQueueFamilyProperties> {count};
		vkGetPhysicalDeviceQueueFamilyProperties (handle, &count, res.data());
		return res;
	}
	operator VkDevice () && {
		return {};
	}
private:
	VkPhysicalDevice handle;
};