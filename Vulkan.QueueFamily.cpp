module;
#include <iostream>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
export module Vulkan.QueueFamily;

import Vulkan.Queue;
import Vulkan.CommandPool;

export struct QueueFamily {
	std::vector <vQueue> handles;
	// CommandPool command_pool;
	// uint32_t index;
	// VkQueueFlags capabilities;
	// VkBool32 present_support;

	// auto supports_graphics_operations () const noexcept -> bool {
	// 	return capabilities & VK_QUEUE_GRAPHICS_BIT;
	// }
	// auto supports_compute_operations () const noexcept -> bool {
	// 	return capabilities & VK_QUEUE_COMPUTE_BIT;
	// }
	// auto supports_transfer_operations () const noexcept -> bool {
	// 	return capabilities & VK_QUEUE_TRANSFER_BIT;
	// }
	// auto supports_memory_binding_operations () const noexcept -> bool {
	// 	return capabilities & VK_QUEUE_SPARSE_BINDING_BIT;
	// }
};

