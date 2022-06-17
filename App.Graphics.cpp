#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// #include <vulkan/vulkan.h>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <optional>
import Delta;

#ifdef DEBUG
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		// Message is important enough to show
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
	}

    return VK_FALSE;
}
#endif

auto main(int argc, char **argv) -> int
{
	GLFWwindow *window;

	/* Initialize the library */
	if (!glfwInit())
		return EXIT_FAILURE;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	VkApplicationInfo appInfo{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Hello Triangle",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "No Engine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_2
	};

	VkInstanceCreateInfo createInfo {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo
	};

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	auto extensions = std::vector<const char*> {glfwExtensions, glfwExtensions + glfwExtensionCount};
	
#ifdef DEBUG
	extensions.push_back (VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			std::cout << "error >> required layers not found" << std::endl;
			return EXIT_FAILURE;
		}
	}

	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

	// uint32_t extensionCount = 0;
	// vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	// std::vector<VkExtensionProperties> extensions(extensionCount);

	// vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	// std::cout << "available extensions:\n";

	// for (const auto& extension : extensions) {
	// 	std::cout << '\t' << extension.extensionName << '\n';
	// }
#else
	createInfo.enabledLayerCount = 0;
#endif

	createInfo.enabledExtensionCount = static_cast<uint32_t> (extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();
	
	VkInstance instance;

	switch (vkCreateInstance(&createInfo, nullptr, &instance)) {
		case VK_SUCCESS:
			break;

		case VK_ERROR_EXTENSION_NOT_PRESENT:
			std::cout << "error >> extensions not present" << std::endl;
			return EXIT_FAILURE;

		case VK_ERROR_LAYER_NOT_PRESENT:
			std::cout << "error >> layers not present" << std::endl;
			return EXIT_FAILURE;


		case VK_ERROR_INCOMPATIBLE_DRIVER:
			std::cout << "error >> incompatible driver" << std::endl;
			return EXIT_FAILURE;
		
		default:
			std::cout << "error >> could not create vulkan instance" << std::endl;
			return EXIT_FAILURE;
	}

#ifdef DEBUG
	VkDebugUtilsMessengerEXT debugMessenger;

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pfnUserCallback = debugCallback,
		.pUserData = nullptr // Optional
	};

	auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr (instance, "vkCreateDebugUtilsMessengerEXT");

    if (func != nullptr) {
        func (instance, &debugCreateInfo, nullptr /*pAllocayot*/, &debugMessenger);
    } else {
		std::cout << "error >> failed to set up debug messenger" << std::endl;
        // return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
#endif

	VkSurfaceKHR surface;
	
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        std::cout << "error >> failed to create window surface" << std::endl;
		return EXIT_FAILURE;
    }

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices (instance, &deviceCount, nullptr);
	
	if (deviceCount == 0) {
    	std::cout << "error >> failed to find GPUs with Vulkan support" << std::endl;
		return EXIT_FAILURE;
	}

	auto devices = std::vector <VkPhysicalDevice> {deviceCount};
	vkEnumeratePhysicalDevices (instance, &deviceCount, devices.data());

	auto& physicalDevice = devices.front();

	VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);

	switch (deviceProperties.deviceType) {

		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			break;

		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			break;
		
		default:
			std::cout << "error >> suitable gpu device not found" << std::endl;
			return EXIT_FAILURE;
	}

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	auto queueFamilies = std::vector<VkQueueFamilyProperties> {queueFamilyCount};
	vkGetPhysicalDeviceQueueFamilyProperties (physicalDevice, &queueFamilyCount, queueFamilies.data());

	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) graphicsFamily = i;
		

		VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR (physicalDevice, i, surface, &presentSupport);

		if (presentSupport) presentFamily = i;
        
		if (presentSupport and graphicsFamily.has_value()) break;

		i++;
	}

	if (! graphicsFamily.has_value()) {
		std::cout << "error >> could not find any graphics queue family" << std::endl;
		return EXIT_FAILURE;
	} else if (! presentFamily.has_value()) {
		std::cout << "error >> could not find any present queue family" << std::endl;
		return EXIT_FAILURE;
	}

	float queuePriority = 1.0f;

	VkDeviceQueueCreateInfo queueCreateInfo {
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = graphicsFamily.value(),
		.queueCount = 1,
		.pQueuePriorities = &queuePriority
	};

	uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties (physicalDevice, nullptr, &extensionCount, nullptr);
	auto availableExtensions = std::vector <VkExtensionProperties> {extensionCount};
	vkEnumerateDeviceExtensionProperties (physicalDevice, nullptr, &extensionCount, availableExtensions.data());

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR (physicalDevice, surface, &formatCount, nullptr);
	if (formatCount == 0) {
		std::cout << "error" << std::endl;
		return EXIT_FAILURE;
	}
	auto formats = std::vector<VkSurfaceFormatKHR> {formatCount};
	vkGetPhysicalDeviceSurfaceFormatsKHR (physicalDevice, surface, &formatCount, formats.data());

	// VkSurfaceCapabilitiesKHR capabilities;
	// vkGetPhysicalDeviceSurfaceFormatsKHR (physicalDevice, surface, &formatCount, &capabilities);
	// for (auto const& i : availableExtensions) {
	// 	if (strcmp(i.extensionName, layerProperties.layerName) == 0) {

	// 	}
	// }

	VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
	// std::cout << memProperties.memoryHeaps[0].size  << " bytes" << std::endl;

	std::vector<const char*> deviceExtensions = {
		"VK_KHR_swapchain",
		"VK_KHR_portability_subset"
	};

	VkDeviceCreateInfo logicalDeviceCreateInfo {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pQueueCreateInfos = &queueCreateInfo,
		.queueCreateInfoCount = 1,
		.pEnabledFeatures = &deviceFeatures,
		.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
		.ppEnabledExtensionNames = deviceExtensions.data(),
		.enabledLayerCount = 0
	};

	VkDevice device;

	if (vkCreateDevice (physicalDevice, &logicalDeviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
		std::cout << "error >> failed to create logical device" << std::endl;
		return EXIT_FAILURE;
	}

	// specify which queue families you would like to use

	// a handle to the graphics queue
	VkQueue graphicsQueue;

	vkGetDeviceQueue (device, graphicsFamily.value(), 0, &graphicsQueue);

	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		// glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		// glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	vkDestroyDevice(device, nullptr);

#ifdef DEBUG
	auto func2 = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func2 != nullptr) {
        func2(instance, debugMessenger, nullptr/*pAllocator*/);
    }
#endif

	vkDestroySurfaceKHR (instance, surface, nullptr);

	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);

	glfwTerminate();

	return EXIT_SUCCESS;
}