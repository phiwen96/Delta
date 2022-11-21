module;
#include <iostream>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
export module Vulkan.PhysicalDevice;

import Vulkan.Instance;

export struct vPhysicalDevice {
	vPhysicalDevice (vInstance& h) : handle {h.devices().back()} {

	}
	vPhysicalDevice (VkPhysicalDevice&& h) : handle {(VkPhysicalDevice&&) h} {

	}
private:
	VkPhysicalDevice handle;
};