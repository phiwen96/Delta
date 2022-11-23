module;
#include <iostream>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
export module Vulkan.LogicalDevice;

import Vulkan.PhysicalDevice;
import Vulkan.QueueFamily;

export struct vLogicalDevice {
	vLogicalDevice (VkDevice&& h) : handle {(VkDevice&&) h} {}

private:
	VkDevice handle;
};