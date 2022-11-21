// import Ja;
// #include <vulkan/vulkan_core.h>
// import Vulkan;
#include <iostream>
#include <vector>
import Vulkan;
// import Standard;

auto main (int argc, char** argv) -> int {
	auto instance = vInstance {{"VK_LAYER_KHRONOS_validation"}, {"VK_KHR_surface"}};
	auto physicalDevice = vPhysicalDevice {instance};
	// auto instance = vInstance {{"VK_LAYER_KHRONOS_validation"}, {"VK_KHR_surface"}};
	std::cout << "hello world" << std::endl;
	// std::vector <int> baj;
	// VkInstance inst;
	return 0;
}