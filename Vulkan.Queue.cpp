module;
#include <iostream>
#include <vector>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
export module Vulkan.Queue;

export struct vQueue {
	vQueue (VkQueue&& h) : handle {(VkQueue&&) h} {}

private:
	VkQueue handle;
};