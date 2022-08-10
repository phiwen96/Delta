#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
// #include <vulkan/vulkan.h>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <optional>
#include <set>
#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp
#include <fstream>
#include <array>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <ft2build.h>
#include FT_FREETYPE_H

// import Delta;
import Delta.Graphics;



auto readByteFile (auto&& filename) -> auto
{
	auto file = std::ifstream {filename, std::ios::ate | std::ios::binary};
	if (not file.is_open ())
	{
		std::cout << "error >> failed to read shader" << std::endl;
		std::terminate ();
	}

	size_t fileSize = (size_t) file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read (buffer.data(), fileSize);
	file.close();

	return buffer;
}

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
	auto freetypeLibrary = FT_Library {};

	if (FT_Init_FreeType (&freetypeLibrary))
	{
		std::cout << "error >> failed to initialize freetype library" << std::endl;
		return EXIT_FAILURE;
	} 

	auto face = FT_Face {};

	if (FT_New_Face (freetypeLibrary, (std::string (FONTS_DIR) + "/Times new Roman.ttf").c_str (), 0, &face))
	{
		std::cout << "error >> failed to load font from file" << std::endl;
		return EXIT_FAILURE;
	}

	if (FT_Set_Pixel_Sizes (face, 0, 50))
	{
		std::cout << "error >> failed to set font size" << std::endl;
		return EXIT_FAILURE;
	}

	auto glyphIndex = FT_Get_Char_Index (face, 'F');

	if (FT_Load_Glyph (face, glyphIndex, FT_LOAD_DEFAULT))
	{
		std::cout << "error >> failed to load glyph" << std::endl;
		return EXIT_FAILURE;
	}

	if (FT_Render_Glyph (face->glyph, FT_RENDER_MODE_NORMAL))
	{
		std::cout << "error >> failed to render glyph" << std::endl;
		return EXIT_FAILURE;
	}

	auto fontWidth = face->glyph->bitmap.width;
	auto fontHeight = face->glyph->bitmap.rows;
	auto fontBufferSize = fontWidth * fontHeight * 4;
	auto* fontData = face->glyph->bitmap.buffer;
	

	// return 0;

	/* Initialize the library */
	if (!glfwInit())
		return EXIT_FAILURE;

	glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);

	glfwWindowHint (GLFW_RESIZABLE, GLFW_FALSE);

	/* Create a windowed mode window and its OpenGL context */
	auto* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	auto appInfo = VkApplicationInfo 
	{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Hello Triangle",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "No Engine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_2
	};

	auto instanceCreateInfo = VkInstanceCreateInfo 
	{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo
	};

	auto glfwExtensionCount = uint32_t {0};
	auto const ** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
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

	instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();

	// uint32_t extensionCount = 0;
	// vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	// std::vector<VkExtensionProperties> extensions(extensionCount);

	// vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	// std::cout << "available extensions:\n";

	// for (const auto& extension : extensions) {
	// 	std::cout << '\t' << extension.extensionName << '\n';
	// }
#else
	instanceCreateInfo.enabledLayerCount = 0;
#endif

	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t> (extensions.size());
	instanceCreateInfo.ppEnabledExtensionNames = extensions.data();
	
	auto instance = VkInstance {};

	switch (vkCreateInstance(&instanceCreateInfo, nullptr, &instance)) {
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

	auto debugCreateInfo = VkDebugUtilsMessengerCreateInfoEXT {
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

	auto surface = VkSurfaceKHR {};
	
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
        std::cout << "error >> failed to create window surface" << std::endl;
		return EXIT_FAILURE;
    }

	auto deviceCount = uint32_t {0};
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

	auto extensionCount = uint32_t {};
    vkEnumerateDeviceExtensionProperties (physicalDevice, nullptr, &extensionCount, nullptr);
	auto availableExtensions = std::vector <VkExtensionProperties> {extensionCount};
	vkEnumerateDeviceExtensionProperties (physicalDevice, nullptr, &extensionCount, availableExtensions.data());

	auto surfaceCapabilities = VkSurfaceCapabilitiesKHR {};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR (physicalDevice, surface, &surfaceCapabilities);

	auto surfaceExtent = VkExtent2D {};
	// choose swap extent
	if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) 
	{
		surfaceExtent = surfaceCapabilities.currentExtent;

	}  else 
	{
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		surfaceExtent = VkExtent2D 
		{
			static_cast <uint32_t> (width),
			static_cast <uint32_t> (height)
		};

		surfaceExtent.width = std::clamp(surfaceExtent.width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width);		
		surfaceExtent.height = std::clamp(surfaceExtent.height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height);
	}

	auto imageCount = uint32_t {surfaceCapabilities.minImageCount + 1};

	if (surfaceCapabilities.maxImageCount > 0 && imageCount > surfaceCapabilities.maxImageCount) {
		imageCount = surfaceCapabilities.maxImageCount;
		surfaceExtent = surfaceCapabilities.currentExtent;
	}

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR (physicalDevice, surface, &formatCount, nullptr);
	if (formatCount == 0) {
		std::cout << "error" << std::endl;
		return EXIT_FAILURE;
	}
	auto availableSurfaceFormats = std::vector <VkSurfaceFormatKHR> {formatCount};
	vkGetPhysicalDeviceSurfaceFormatsKHR (physicalDevice, surface, &formatCount, availableSurfaceFormats.data());

	VkSurfaceFormatKHR const* surfaceFormat = nullptr;

	for (auto const& availableSurfaceFormat : availableSurfaceFormats) {
		if (availableSurfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB and availableSurfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR){
			surfaceFormat = &availableSurfaceFormat;
			break;
		}
	}

	if (surfaceFormat == nullptr) {
		std::cout << "error >> could not find suitable surface format" << std::endl;
		return EXIT_FAILURE;
	}
	
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
	if (presentModeCount == 0) {
		std::cout << "error" << std::endl;
		return EXIT_FAILURE;
	}

	auto availablePresentModes = std::vector <VkPresentModeKHR> {presentModeCount};

	vkGetPhysicalDeviceSurfacePresentModesKHR (physicalDevice, surface, &presentModeCount, availablePresentModes.data());

	VkPresentModeKHR const* presentMode = nullptr;

	// auto*  
	for (auto const& availablePresentMode : availablePresentModes) {
		switch (availablePresentMode) {
			case VK_PRESENT_MODE_IMMEDIATE_KHR:
				break;
			case VK_PRESENT_MODE_FIFO_KHR:
				presentMode = &availablePresentMode;
				break;
			case VK_PRESENT_MODE_FIFO_RELAXED_KHR:
				break;
			case VK_PRESENT_MODE_MAILBOX_KHR:
				presentMode = &availablePresentMode;
				goto EXIT_LOOP;
			default:
				break;
		}
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentMode = &availablePresentMode;
		}
	}
	EXIT_LOOP:

	if (presentMode == nullptr) {
		std::cout << "error >> no available present mode" << std::endl;
		return EXIT_FAILURE;
		// std::cout << "warning >> performance present mode not available, defaulting to energy present mode" << std::endl; 
	}

	VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	if (queueFamilyCount == 0) {
		std::cout << "error >> queue families not found" << std::endl;
		return EXIT_FAILURE;
	}

	auto availableQueueFamilies = std::vector<VkQueueFamilyProperties> {queueFamilyCount};
	vkGetPhysicalDeviceQueueFamilyProperties (physicalDevice, &queueFamilyCount, availableQueueFamilies.data());

	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> transferFamily;
	std::optional<uint32_t> presentFamily;
	int i = 0;
	for (const auto& queueFamily : availableQueueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) graphicsFamily = i;
		if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) transferFamily = i;
		
		VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR (physicalDevice, i, surface, &presentSupport);

		if (presentSupport) presentFamily = i;
        
		if (presentSupport and graphicsFamily.has_value() and transferFamily.has_value()) break;

		i++;
	}

	if (! graphicsFamily.has_value()) {
		std::cout << "error >> could not find any graphics queue family" << std::endl;
		return EXIT_FAILURE;
	} else if (! transferFamily.has_value()) {
		std::cout << "error >> could not find any transfer queue family" << std::endl;
		return EXIT_FAILURE;
	} else if (! presentFamily.has_value()) {
		std::cout << "error >> could not find any present queue family" << std::endl;
		return EXIT_FAILURE;
	}

	auto queueCreateInfos = std::vector <VkDeviceQueueCreateInfo> {};
	auto uniqueQueueFamilies = std::set <uint32_t> {graphicsFamily.value(), presentFamily.value()};
	// uint32_t queueFamilyIndices[] = {graphicsFamily.value(), presentFamily.value()};
	auto queuePriority = float {1.0};

	for (auto const& queueFamily : uniqueQueueFamilies) {
		queueCreateInfos.push_back (
			VkDeviceQueueCreateInfo {
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.queueFamilyIndex = queueFamily,
				.queueCount = 1,
				.pQueuePriorities = &queuePriority
			}
		);
	}
	// std::cout << memProperties.memoryHeaps[0].size  << " bytes" << std::endl;

	auto deviceExtensions = std::vector <const char*>
	{
		"VK_KHR_swapchain",
		"VK_KHR_portability_subset"
	};

	auto logicalDeviceCreateInfo = VkDeviceCreateInfo 
	{
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pQueueCreateInfos = queueCreateInfos.data(),
		.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
		.pEnabledFeatures = &deviceFeatures,
		.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size()),
		.ppEnabledExtensionNames = deviceExtensions.data(),
		.enabledLayerCount = 0
	};

	auto device = VkDevice {};

	if (vkCreateDevice (physicalDevice, &logicalDeviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
		std::cout << "error >> failed to create logical device" << std::endl;
		return EXIT_FAILURE;
	}	

	// specify which queue families you would like to use

	// a handle to the graphics queue
	auto graphicsQueue = VkQueue {};

	vkGetDeviceQueue (device, graphicsFamily.value(), 0, &graphicsQueue);


	auto transferQueue = VkQueue {};

	vkGetDeviceQueue (device, transferFamily.value(), 0, &transferQueue);


	auto presentQueue = VkQueue {};

	vkGetDeviceQueue (device, presentFamily.value(), 0, &presentQueue);


	uint32_t queueFamilyIndices[] = {graphicsFamily.value(), transferFamily.value(), presentFamily.value()};

	auto swapchainCreateInfo = VkSwapchainCreateInfoKHR 
	{
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surface,
		.minImageCount = imageCount,
		.imageFormat = surfaceFormat -> format,
		.imageColorSpace = surfaceFormat -> colorSpace,
		.imageExtent = surfaceExtent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.preTransform = surfaceCapabilities.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = *presentMode,
		.clipped = VK_TRUE,
		.oldSwapchain = VK_NULL_HANDLE
	};

	if (graphicsFamily != presentFamily) {
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainCreateInfo.queueFamilyIndexCount = 2;
		swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
		// std::cout<<"tjo\n";
	} else {
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfo.queueFamilyIndexCount = 0; // Optional
		swapchainCreateInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	auto swapchain = VkSwapchainKHR {};
	
	if (vkCreateSwapchainKHR (device, &swapchainCreateInfo, nullptr, &swapchain) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create swapchain" << std::endl;
		return EXIT_FAILURE;
	}

	
	vkGetSwapchainImagesKHR (device, swapchain, &imageCount, nullptr);
	auto swapchainImages = std::vector <VkImage> {imageCount};
	vkGetSwapchainImagesKHR (device, swapchain, &imageCount, swapchainImages.data());

	auto swapchainImageViews = std::vector <VkImageView> {swapchainImages.size ()};
	
	for (auto i = 0; i < swapchainImages.size(); ++i)
	{
		auto imageViewCreateInfo = VkImageViewCreateInfo 
		{
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = swapchainImages [i],
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = surfaceFormat->format,
			.components.r = VK_COMPONENT_SWIZZLE_IDENTITY,
			.components.g = VK_COMPONENT_SWIZZLE_IDENTITY,
			.components.b = VK_COMPONENT_SWIZZLE_IDENTITY,
			.components.a = VK_COMPONENT_SWIZZLE_IDENTITY,
			.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
			.subresourceRange.baseMipLevel = 0,
			.subresourceRange.levelCount = 1,
			.subresourceRange.baseArrayLayer = 0,
			.subresourceRange.layerCount = 1
		};

		if (vkCreateImageView (device, &imageViewCreateInfo, nullptr, &swapchainImageViews [i]) != VK_SUCCESS)
		{
			std::cout << "error >> failed to create image views" << std::endl;
			return EXIT_FAILURE;
		}
	}

	auto colorAttachment = VkAttachmentDescription
	{
		.format = surfaceFormat -> format,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};

	auto colorAttachmentRef = VkAttachmentReference
	{
		.attachment = 0,
		.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

	auto subpass = VkSubpassDescription
	{
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachmentRef
	};	

	auto dependency = VkSubpassDependency
	{
		.srcSubpass = VK_SUBPASS_EXTERNAL,
		.dstSubpass = 0,
		.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		.srcAccessMask = 0,
		.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
	};

	auto renderPassCreateInfo = VkRenderPassCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.attachmentCount = 1,
		.pAttachments = &colorAttachment,
		.subpassCount = 1,
		.pSubpasses = &subpass,
		.dependencyCount = 1,
		.pDependencies = &dependency
	};

	auto renderPass = VkRenderPass {};

	if (vkCreateRenderPass (device, &renderPassCreateInfo, nullptr, &renderPass) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create render pass" << std::endl;
		return EXIT_FAILURE;
	}


	auto vertShaderCode = readByteFile ("Graphics.Triangle.vert.spv");

	auto vertShaderModuleCreateInfo = VkShaderModuleCreateInfo 
	{
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = vertShaderCode.size (),
		.pCode = reinterpret_cast <uint32_t const *> (vertShaderCode.data ())
	};

	auto vertShaderModule = VkShaderModule {};

	if (vkCreateShaderModule (device, &vertShaderModuleCreateInfo, nullptr, &vertShaderModule) != VK_SUCCESS) 
	{
		std::cout << "error >> failed to create vertex shader module" << std::endl;
		return EXIT_FAILURE;
	}

	auto fragShaderCode = readByteFile ("Graphics.Triangle.frag.spv");

	auto fragShaderModuleCreateInfo = VkShaderModuleCreateInfo 
	{
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = fragShaderCode.size (),
		.pCode = reinterpret_cast <uint32_t const *> (fragShaderCode.data ())
	};

	auto fragShaderModule = VkShaderModule {};

	if (vkCreateShaderModule (device, &fragShaderModuleCreateInfo, nullptr, &fragShaderModule) != VK_SUCCESS) 
	{
		std::cout << "error >> failed to create fragment shader module" << std::endl;
		return EXIT_FAILURE;
	}

	auto vertShaderStageCreateInfo = VkPipelineShaderStageCreateInfo 
	{
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		nullptr,
		{},
		VK_SHADER_STAGE_VERTEX_BIT,
		vertShaderModule,
		"main"
	};

	auto fragShaderStageCreateInfo = VkPipelineShaderStageCreateInfo 
	{
		VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		nullptr,
		{},
		VK_SHADER_STAGE_FRAGMENT_BIT,
		fragShaderModule,
		"main"
	};

	VkPipelineShaderStageCreateInfo shaderStageCreateInfos[] = {vertShaderStageCreateInfo, fragShaderStageCreateInfo};
	
	struct vertex
	{
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;
	};

	auto vertexBindingDescription = VkVertexInputBindingDescription
	{
		.binding = 0,
		.stride = sizeof (vertex),
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
	};

	auto vertexAttributeDescriptions = std::array <VkVertexInputAttributeDescription, 3>
	{
		VkVertexInputAttributeDescription{
			.binding = 0,
			.location = 0,
			.format = VK_FORMAT_R32G32B32_SFLOAT,
			.offset = offsetof (vertex, pos)
		},

		VkVertexInputAttributeDescription{
			.binding = 0,
			.location = 1,
			.format = VK_FORMAT_R32G32B32_SFLOAT,
			.offset = offsetof (vertex, color)
		},

		VkVertexInputAttributeDescription
		{
			.binding = 0,
			.location = 2,
			.format = VK_FORMAT_R32G32_SFLOAT,
			.offset = offsetof (vertex, texCoord)
		}
	};

	auto vertexInputCreateInfo = VkPipelineVertexInputStateCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &vertexBindingDescription, // Optional
		.vertexAttributeDescriptionCount = static_cast <uint32_t> (vertexAttributeDescriptions.size()),
		.pVertexAttributeDescriptions = vertexAttributeDescriptions.data() // Optional
	};

	auto inputAssemblyCreateInfo = VkPipelineInputAssemblyStateCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE
	};

	auto viewport = VkViewport 
	{
		.x = 0.0f,
		.y = 0.0f,
		.width = (float) surfaceExtent.width,
		.height = (float) surfaceExtent.height,
		.minDepth = 0.0f,
		.maxDepth = 1.0f
	};

	auto scissors = VkRect2D
	{
		.offset = {0, 0},
		.extent = surfaceExtent
	};

	auto viewportStateCreateInfo = VkPipelineViewportStateCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.pViewports = &viewport,
		.scissorCount = 1,
		.pScissors = &scissors
	};

	auto rasterizerCreateInfo = VkPipelineRasterizationStateCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthClampEnable = VK_FALSE,
		// .polygonMode = VK_POLYGON_MODE_FILL, // LINE or POINT
		.polygonMode = VK_POLYGON_MODE_FILL,
		.lineWidth = 1.0f,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_CLOCKWISE,
		.depthBiasEnable = VK_FALSE,
		.depthBiasConstantFactor = 0.0f,
		.depthBiasClamp = 0.0f,
		.depthBiasSlopeFactor = 0.0f
	};

	auto multisampleCreateInfo = VkPipelineMultisampleStateCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.sampleShadingEnable = VK_FALSE,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
		.minSampleShading = 1.0f,
		.pSampleMask = nullptr,
		.alphaToCoverageEnable = VK_FALSE,
		.alphaToOneEnable = VK_FALSE
	};

	auto colorBlendAttachmentCreateInfo = VkPipelineColorBlendAttachmentState
	{
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
		.blendEnable = VK_FALSE,
		.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
		.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
		.colorBlendOp = VK_BLEND_OP_ADD,
		.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
		.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
		.alphaBlendOp = VK_BLEND_OP_ADD
	};

	auto colorBlendCreateInfo = VkPipelineColorBlendStateCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = 1,
		.pAttachments = &colorBlendAttachmentCreateInfo,
		.blendConstants[0] = 0.0f,
		.blendConstants[1] = 0.0f,
		.blendConstants[2] = 0.0f,
		.blendConstants[3] = 0.0f
	};

	auto dynamicStates = std::array 
	{
		VK_DYNAMIC_STATE_VIEWPORT,
		// VK_DYNAMIC_STATE_LINE_WIDTH,
		VK_DYNAMIC_STATE_SCISSOR
	};

	auto dynamicStateCreateInfo = VkPipelineDynamicStateCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = static_cast <uint32_t> (dynamicStates.size()),
		.pDynamicStates = dynamicStates.data()
	};

	auto descriptorSetLayoutBinding = VkDescriptorSetLayoutBinding
	{
		.binding = 0,
		.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		.descriptorCount = 1,
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
		.pImmutableSamplers = nullptr,

	};

	auto samplerLayoutBinding = VkDescriptorSetLayoutBinding
	{
		.binding = 0,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.pImmutableSamplers = nullptr,
		.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
	};

	auto descriptorSetLayoutCreateInfo = VkDescriptorSetLayoutCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount = 1,
		.pBindings = &samplerLayoutBinding
	};

	auto descriptorSetLayout = VkDescriptorSetLayout {};

	if (vkCreateDescriptorSetLayout (device, &descriptorSetLayoutCreateInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create descriptor set layout" << std::endl;
		return EXIT_FAILURE;
	}

	auto pushConstantRange = VkPushConstantRange
	{
		.offset = 0,
		.size = sizeof (push_constants),
		.stageFlags = VK_SHADER_STAGE_VERTEX_BIT
	};

	auto layoutCreateInfo = VkPipelineLayoutCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 1,
		.pSetLayouts = &descriptorSetLayout,
		.pushConstantRangeCount = 1,
		.pPushConstantRanges = &pushConstantRange
	};

	auto layout = VkPipelineLayout {};

	if (vkCreatePipelineLayout (device, &layoutCreateInfo, nullptr, &layout) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create pipeline layout" << std::endl;
		return EXIT_FAILURE;
	}

	auto graphicsPipelineCreateInfo = VkGraphicsPipelineCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.stageCount = 2,
		.pStages = shaderStageCreateInfos,
		.pVertexInputState = &vertexInputCreateInfo,
		.pInputAssemblyState = &inputAssemblyCreateInfo,
		.pViewportState = &viewportStateCreateInfo,
		.pRasterizationState = &rasterizerCreateInfo,
		.pMultisampleState = &multisampleCreateInfo,
		.pDepthStencilState = nullptr, // Optional
		.pColorBlendState = &colorBlendCreateInfo,
		.pDynamicState = &dynamicStateCreateInfo, // Optional
		.layout = layout,
		.renderPass = renderPass,
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE, // Optional
	};

	auto graphicsPipeline = VkPipeline {};

	if (vkCreateGraphicsPipelines (device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create graphics pipeline" << std::endl;
		return EXIT_FAILURE;
	}

	vkDestroyShaderModule (device, vertShaderModule, nullptr);
	vkDestroyShaderModule (device, fragShaderModule, nullptr);

	auto swapchainFramebuffers = std::vector <VkFramebuffer> {swapchainImageViews.size ()};
	
	for (auto i = 0; i < swapchainImageViews.size (); ++i)
	{
		VkImageView attachments [] = 
		{
			swapchainImageViews [i]
		};

		auto framebufferCreateInfo = VkFramebufferCreateInfo
		{
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.renderPass = renderPass,
			.attachmentCount = 1,
			.pAttachments = attachments,
			// .pAttachments = &swapchainImageViews [i],
			.width = surfaceExtent.width,
			.height = surfaceExtent.height,
			.layers = 1
		};

		if (vkCreateFramebuffer (device, &framebufferCreateInfo, nullptr, &swapchainFramebuffers [i]) != VK_SUCCESS)
		{
			std::cout << "error >> failed to create framebuffers" << std::endl;
			return EXIT_FAILURE;
		}
	}




	auto vertices = std::vector <vertex>
	{
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},


		// {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		// {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		// {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		// {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
	};

	//////////////////////////////////////////////////////////////
	///	Create staging buffer for vertices
	//////////////////////////////////////////////////////////////

	auto verticesStagingBufferCreateInfo = VkBufferCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = sizeof (vertices[0]) * vertices.size(),
		.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE
	};

	auto verticesStagingBuffer = VkBuffer {};

	if (vkCreateBuffer (device, &verticesStagingBufferCreateInfo, nullptr, &verticesStagingBuffer) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create staging buffer for vertices" << std::endl;
		return EXIT_FAILURE;
	}

	auto verticesStagingBufferMemoryRequirements = VkMemoryRequirements {};

	vkGetBufferMemoryRequirements (device, verticesStagingBuffer, &verticesStagingBufferMemoryRequirements);

	auto verticesStagingBufferMemoryAllocateInfo = VkMemoryAllocateInfo
	{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = verticesStagingBufferMemoryRequirements.size,
	};

	auto verticesStagingBufferMemoryTypeFound = false;




	auto verticesBufferCreateInfo = VkBufferCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = sizeof (vertices[0]) * vertices.size(),
		.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE
	};

	auto verticesBuffer = VkBuffer {};

	if (vkCreateBuffer (device, &verticesBufferCreateInfo, nullptr, &verticesBuffer) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create buffer for vertices" << std::endl;
		return EXIT_FAILURE;
	}

	auto verticesBufferMemoryRequirements = VkMemoryRequirements {};

	vkGetBufferMemoryRequirements (device, verticesBuffer, &verticesBufferMemoryRequirements);

	auto verticesBufferMemoryAllocateInfo = VkMemoryAllocateInfo
	{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = verticesStagingBufferMemoryRequirements.size,
	};

	auto verticesBufferMemoryTypeFound = false;



	auto indices = std::vector <uint16_t> 
	{
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};

	auto indicesStagingBufferCreateInfo = VkBufferCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = sizeof (indices[0]) * indices.size(),
		.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE
	};

	auto indicesStagingBuffer = VkBuffer {};

	if (vkCreateBuffer (device, &indicesStagingBufferCreateInfo, nullptr, &indicesStagingBuffer) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create staging buffer for indices" << std::endl;
		return EXIT_FAILURE;
	}

	auto indicesStagingBufferMemoryRequirements = VkMemoryRequirements {};

	vkGetBufferMemoryRequirements (device, indicesStagingBuffer, &indicesStagingBufferMemoryRequirements);

	auto indicesStagingBufferMemoryAllocateInfo = VkMemoryAllocateInfo
	{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = indicesStagingBufferMemoryRequirements.size,
	};

	auto indicesStagingBufferMemoryTypeFound = false;



	auto indicesBufferCreateInfo = VkBufferCreateInfo 
	{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = sizeof (indices [0]) * indices.size (),
		.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE
	};

	auto indicesBuffer = VkBuffer {};

	if (vkCreateBuffer (device, &indicesBufferCreateInfo, nullptr, &indicesBuffer) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create buffer for indices" << std::endl;
		return EXIT_FAILURE;
	}

	auto indicesBufferMemoryRequirements = VkMemoryRequirements {};

	vkGetBufferMemoryRequirements (device, indicesBuffer, &indicesBufferMemoryRequirements);

	auto indicesBufferMemoryAllocateInfo = VkMemoryAllocateInfo
	{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = indicesStagingBufferMemoryRequirements.size,
	};

	auto indicesBufferMemoryTypeFound = false;




	int texWidth, texHeight, texChannels;

	stbi_uc* pixels = stbi_load ("Graphics.Triangle.Texture.Lion.jpeg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

 	if (!pixels) 
	{
		std::cout << "error >> failed to load texture image" << std::endl;
		return EXIT_FAILURE;
	}

	auto textureImageStagingBufferCreateInfo = VkBufferCreateInfo 
	{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		// .size = (VkDeviceSize) texWidth * texHeight * 4,
		.size = (VkDeviceSize) texWidth * texHeight * 4,
		.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE
	};

	auto textureImageStagingBuffer = VkBuffer {};

	if (vkCreateBuffer (device, &textureImageStagingBufferCreateInfo, nullptr, &textureImageStagingBuffer) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create buffer for textureImageStaging" << std::endl;
		return EXIT_FAILURE;
	}

	auto textureImageStagingBufferMemoryRequirements = VkMemoryRequirements {};

	vkGetBufferMemoryRequirements (device, textureImageStagingBuffer, &textureImageStagingBufferMemoryRequirements);

	auto textureImageStagingBufferMemoryAllocateInfo = VkMemoryAllocateInfo
	{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = textureImageStagingBufferMemoryRequirements.size,
	};

	auto textureImageStagingBufferMemoryTypeFound = false;





	auto textureImageCreateInfo = VkImageCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.imageType = VK_IMAGE_TYPE_2D,
		// .extent.width = static_cast <uint32_t> (texWidth),
		// .extent.height = static_cast <uint32_t> (texHeight),
		.extent.width = static_cast <uint32_t> (texWidth),
		.extent.height = static_cast <uint32_t> (texHeight),
		.extent.depth = 1,
		.mipLevels = 1,
		.arrayLayers = 1,
		.format = VK_FORMAT_R8G8B8A8_SRGB,
		.tiling = VK_IMAGE_TILING_OPTIMAL,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE
	};

	auto textureImage = VkImage {};

	if (vkCreateImage (device, &textureImageCreateInfo, nullptr, &textureImage) != VK_SUCCESS) 
	{
		std::cout << "error >> failed to create image" << std::endl;
	}

	auto textureImageMemoryRequirements = VkMemoryRequirements {};

	vkGetImageMemoryRequirements (device, textureImage, &textureImageMemoryRequirements);

	auto textureImageMemoryAllocateInfo = VkMemoryAllocateInfo
	{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = textureImageMemoryRequirements.size,
	};

	auto textureImageMemoryTypeFound = false;



	struct UniformBufferObject 
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	auto uniformBufferCreateInfo = VkBufferCreateInfo 
	{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = sizeof (UniformBufferObject) ,
		.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE
	};

	auto uniformBuffer = VkBuffer {};

	if (vkCreateBuffer (device, &uniformBufferCreateInfo, nullptr, &uniformBuffer) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create buffer for uniform" << std::endl;
		return EXIT_FAILURE;
	}

	auto uniformBufferMemoryRequirements = VkMemoryRequirements {};

	vkGetBufferMemoryRequirements (device, uniformBuffer, &uniformBufferMemoryRequirements);

	auto uniformBufferMemoryAllocateInfo = VkMemoryAllocateInfo
	{
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = uniformBufferMemoryRequirements.size,
	};

	auto uniformBufferMemoryTypeFound = false;



	auto physicalDeviceMemoryProperties = VkPhysicalDeviceMemoryProperties {};

	vkGetPhysicalDeviceMemoryProperties (physicalDevice, &physicalDeviceMemoryProperties);


	for (uint32_t i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; ++i)
	{
		if ((verticesStagingBufferMemoryRequirements.memoryTypeBits & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) == (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
		{
			verticesStagingBufferMemoryTypeFound = true;
			verticesStagingBufferMemoryAllocateInfo.memoryTypeIndex = i;
		}

		if ((verticesBufferMemoryRequirements.memoryTypeBits & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
		{
			verticesBufferMemoryTypeFound = true;
			verticesBufferMemoryAllocateInfo.memoryTypeIndex = i;
		}

		if ((indicesStagingBufferMemoryRequirements.memoryTypeBits & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) == (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
		{
			indicesStagingBufferMemoryTypeFound = true;
			indicesStagingBufferMemoryAllocateInfo.memoryTypeIndex = i;
		}

		if ((indicesBufferMemoryRequirements.memoryTypeBits & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
		{
			indicesBufferMemoryTypeFound = true;
			indicesBufferMemoryAllocateInfo.memoryTypeIndex = i;
		}

		if ((uniformBufferMemoryRequirements.memoryTypeBits & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) == (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
		{
			uniformBufferMemoryTypeFound = true;
			uniformBufferMemoryAllocateInfo.memoryTypeIndex = i;
		}

		if ((textureImageStagingBufferMemoryRequirements.memoryTypeBits & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) == (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
		{
			textureImageStagingBufferMemoryTypeFound = true;
			textureImageStagingBufferMemoryAllocateInfo.memoryTypeIndex = i;
		}

		if ((textureImageMemoryRequirements.memoryTypeBits & (1 << i)) && (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
		{
			textureImageMemoryTypeFound = true;
			textureImageMemoryAllocateInfo.memoryTypeIndex = i;
		}
	}

	if (not verticesStagingBufferMemoryTypeFound)
	{
		std::cout << "error >> could not find the right type of memory for vertices staging buffer" << std::endl;
		return EXIT_FAILURE;

	} else if (not verticesBufferMemoryTypeFound)
	{
		std::cout << "error >> could not find the right type of memory for vertices buffer" << std::endl;
		return EXIT_FAILURE;

	} else if (not indicesStagingBufferMemoryTypeFound)
	{
		std::cout << "error >> could not find the right type of memory for indices staging buffer" << std::endl;
		return EXIT_FAILURE;

	} else if (not indicesBufferMemoryTypeFound)
	{
		std::cout << "error >> could not find the right type of memory for indices buffer" << std::endl;
		return EXIT_FAILURE;
		
	} else if (not uniformBufferMemoryTypeFound)
	{
		std::cout << "error >> could not find the right type of memory for uniform buffer" << std::endl;
		return EXIT_FAILURE;
		
	} else if (not textureImageStagingBufferMemoryTypeFound)
	{
		std::cout << "error >> could not find the right type of memory for texture image staging buffer" << std::endl;
		return EXIT_FAILURE;
		
	} else if (not textureImageMemoryTypeFound)
	{
		std::cout << "error >> could not find the right type of memory for texture image" << std::endl;
		return EXIT_FAILURE;
		
	}

	auto verticesStagingBufferMemory = VkDeviceMemory {};

	if (vkAllocateMemory (device, &verticesStagingBufferMemoryAllocateInfo, nullptr, &verticesStagingBufferMemory) != VK_SUCCESS)
	{
		std::cout << "error >> failed to allocate memory for the vertices buffer" << std::endl;
		return EXIT_FAILURE;
	}

	vkBindBufferMemory (device, verticesStagingBuffer, verticesStagingBufferMemory, 0);



	auto verticesBufferMemory = VkDeviceMemory {};

	if (vkAllocateMemory (device, &verticesBufferMemoryAllocateInfo, nullptr, &verticesBufferMemory) != VK_SUCCESS)
	{
		std::cout << "error >> failed to allocate memory for the vertices buffer" << std::endl;
		return EXIT_FAILURE;
	}

	vkBindBufferMemory (device, verticesBuffer, verticesBufferMemory, 0);

	

	auto indicesStagingBufferMemory = VkDeviceMemory {};

	if (vkAllocateMemory (device, &indicesStagingBufferMemoryAllocateInfo, nullptr, &indicesStagingBufferMemory) != VK_SUCCESS)
	{
		std::cout << "error >> failed to allocate memory for the indices buffer" << std::endl;
		return EXIT_FAILURE;
	}

	vkBindBufferMemory (device, indicesStagingBuffer, indicesStagingBufferMemory, 0);




	auto indicesBufferMemory = VkDeviceMemory {};

	if (vkAllocateMemory (device, &indicesBufferMemoryAllocateInfo, nullptr, &indicesBufferMemory) != VK_SUCCESS)
	{
		std::cout << "error >> failed to allocate memory for the indices buffer" << std::endl;
		return EXIT_FAILURE;
	}

	vkBindBufferMemory (device, indicesBuffer, indicesBufferMemory, 0);




	auto textureImageStagingBufferMemory = VkDeviceMemory {};

	if (vkAllocateMemory (device, &textureImageStagingBufferMemoryAllocateInfo, nullptr, &textureImageStagingBufferMemory) != VK_SUCCESS)
	{
		std::cout << "error >> failed to allocate memory for the texture image buffer" << std::endl;
		return EXIT_FAILURE;
	}

	vkBindBufferMemory (device, textureImageStagingBuffer, textureImageStagingBufferMemory, 0);




	auto textureImageMemory = VkDeviceMemory {};

	if (vkAllocateMemory (device, &textureImageMemoryAllocateInfo, nullptr, &textureImageMemory) != VK_SUCCESS)
	{
		std::cout << "error >> failed to allocate memory for the texture image buffer" << std::endl;
		return EXIT_FAILURE;
	}

	vkBindImageMemory (device, textureImage, textureImageMemory, 0);



	auto uniformBufferMemory = VkDeviceMemory {};

	if (vkAllocateMemory (device, &uniformBufferMemoryAllocateInfo, nullptr, &uniformBufferMemory) != VK_SUCCESS)
	{
		std::cout << "error >> failed to allocate memory for the uniform buffer" << std::endl;
		return EXIT_FAILURE;
	}

	vkBindBufferMemory (device, uniformBuffer, uniformBufferMemory, 0);

	

	// copy vertices data to the staging buffer
	void * verticesData;
	vkMapMemory (device, verticesStagingBufferMemory, 0, verticesStagingBufferCreateInfo.size, 0, &verticesData);
	memcpy (verticesData, vertices.data(), (size_t) verticesStagingBufferCreateInfo.size);
	vkUnmapMemory (device, verticesStagingBufferMemory);

	// copy indices data to the staging buffer
	void * indicesData;
	vkMapMemory (device, indicesStagingBufferMemory, 0, indicesStagingBufferCreateInfo.size, 0, &indicesData);
	memcpy (indicesData, indices.data(), (size_t) indicesStagingBufferCreateInfo.size);
	vkUnmapMemory (device, indicesStagingBufferMemory);
	

	void * textureImageStagingData;
	vkMapMemory (device, textureImageStagingBufferMemory, 0, textureImageStagingBufferCreateInfo.size, 0, &textureImageStagingData);
	memcpy (textureImageStagingData, pixels, (size_t) textureImageStagingBufferCreateInfo.size);
	vkUnmapMemory (device, textureImageStagingBufferMemory);

	stbi_image_free (pixels);

	



	auto transferCommandPoolCreateInfo = VkCommandPoolCreateInfo 
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = transferFamily.value()
	};

	auto transferCommandPool = VkCommandPool {};

	if (vkCreateCommandPool(device, &transferCommandPoolCreateInfo, nullptr, &transferCommandPool) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create transfer command pool" << std::endl;
		return EXIT_FAILURE;
	}

	
	// copy data from staging buffer to device buffer

	// create a command buffer to execute memory transfer operations
	auto copyCommandBufferAllocateInfo = VkCommandBufferAllocateInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandPool = transferCommandPool,
		.commandBufferCount = 1
	};

	auto copyCommandBuffer = VkCommandBuffer {};

	vkAllocateCommandBuffers (device, &copyCommandBufferAllocateInfo, &copyCommandBuffer);

	auto copyCommandBufferBeginInfo = VkCommandBufferBeginInfo 
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
	};

	if (vkBeginCommandBuffer (copyCommandBuffer, &copyCommandBufferBeginInfo) != VK_SUCCESS)
	{
		std::cout << "error >> failed to begin recording graphics command buffer" << std::endl;
		return EXIT_FAILURE;
	}

	auto verticesCopyRegion = VkBufferCopy 
	{
		.srcOffset = 0,
		.dstOffset = 0,
		.size = verticesStagingBufferCreateInfo.size
	};

	vkCmdCopyBuffer (copyCommandBuffer, verticesStagingBuffer, verticesBuffer, 1, &verticesCopyRegion);

	auto indicesCopyRegion = VkBufferCopy 
	{
		.srcOffset = 0,
		.dstOffset = 0,
		.size = indicesStagingBufferCreateInfo.size
	};

	vkCmdCopyBuffer (copyCommandBuffer, indicesStagingBuffer, indicesBuffer, 1, &indicesCopyRegion);

	auto textureImageMemoryBarrier = VkImageMemoryBarrier
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
		.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
		.image = textureImage,
		.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.subresourceRange.baseMipLevel = 0,
		.subresourceRange.levelCount = 1,
		.subresourceRange.baseArrayLayer = 0,
		.subresourceRange.layerCount = 1,
		.srcAccessMask = 0,
		.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT
	};

	auto srcStage = VkPipelineStageFlags {VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT};
	auto dstStage = VkPipelineStageFlags {VK_PIPELINE_STAGE_TRANSFER_BIT};

	vkCmdPipelineBarrier (copyCommandBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &textureImageMemoryBarrier);

	auto textureImageCopyRegion = VkBufferImageCopy
	{
		.bufferOffset = 0,
		.bufferRowLength = 0,
		.bufferImageHeight = 0,
		.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.imageSubresource.mipLevel = 0,
		.imageSubresource.baseArrayLayer = 0,
		.imageSubresource.layerCount = 1,
		.imageOffset = {0, 0, 0},
		.imageExtent = 
		{
			static_cast <uint32_t> (texWidth),
			static_cast <uint32_t> (texHeight),
			1
		}
	};

	vkCmdCopyBufferToImage (copyCommandBuffer, textureImageStagingBuffer, textureImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &textureImageCopyRegion);

	textureImageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	textureImageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	textureImageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	textureImageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

	vkCmdPipelineBarrier (copyCommandBuffer, srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &textureImageMemoryBarrier);


	if (vkEndCommandBuffer(copyCommandBuffer) != VK_SUCCESS)
	{
		std::cout << "error >> failed to end recording copy command buffer" << std::endl;
		return EXIT_FAILURE;
	}

	auto copyBufferSubmitInfo = VkSubmitInfo
	{
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.commandBufferCount = 1,
		.pCommandBuffers = &copyCommandBuffer
	};

	vkQueueSubmit (transferQueue, 1, &copyBufferSubmitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle (transferQueue);

	vkFreeCommandBuffers (device, transferCommandPool, 1, &copyCommandBuffer);

	vkDestroyBuffer (device, textureImageStagingBuffer, nullptr);
	vkFreeMemory (device, textureImageStagingBufferMemory, nullptr);

	vkDestroyBuffer (device, verticesStagingBuffer, nullptr);
	vkFreeMemory (device, verticesStagingBufferMemory, nullptr);

	vkDestroyBuffer (device, indicesStagingBuffer, nullptr);
	vkFreeMemory (device, indicesStagingBufferMemory, nullptr);



	auto textureImageViewCreateInfo = VkImageViewCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image = textureImage,
		.viewType = VK_IMAGE_VIEW_TYPE_2D,
		.format = VK_FORMAT_R8G8B8A8_SRGB,
		.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.subresourceRange.baseMipLevel = 0,
		.subresourceRange.levelCount = 1,
		.subresourceRange.baseArrayLayer = 0,
		.subresourceRange.layerCount = 1
	};

	auto textureImageView = VkImageView {};

	if (vkCreateImageView (device, &textureImageViewCreateInfo, nullptr, &textureImageView) != VK_SUCCESS)
	{
		std::cout << "error >> failed to texture image view" << std::endl;
		return EXIT_FAILURE;
	}


	auto textureSamplerCreateInfo = VkSamplerCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.magFilter = VK_FILTER_LINEAR,
		.minFilter = VK_FILTER_LINEAR,
		.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		.anisotropyEnable = VK_TRUE, // or VK_FALSE
		.maxAnisotropy = deviceProperties.limits.maxSamplerAnisotropy, // or 1.0f
		.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE,
		.compareEnable = VK_FALSE,
		.compareOp = VK_COMPARE_OP_ALWAYS,
		.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		.mipLodBias = 0.0f,
		.minLod = 0.0f,
		.maxLod = 0.0f
	};

	auto textureSampler = VkSampler {};

	if (vkCreateSampler (device, &textureSamplerCreateInfo, nullptr, &textureSampler) != VK_SUCCESS) 
	{
		std::cout << "error >> failed to create texture sampler" << std::endl;
		return EXIT_FAILURE;
	}

	auto textureSamplerDescriptorPoolSize = VkDescriptorPoolSize
	{
		.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.descriptorCount = 1
	};

	auto textureSamplerDescriptorPoolCreateInfo = VkDescriptorPoolCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.poolSizeCount = 1,
		.pPoolSizes = &textureSamplerDescriptorPoolSize,
		.maxSets = 1
	};

	auto textureSamplerDescriptorPool = VkDescriptorPool {};

	if (vkCreateDescriptorPool (device, &textureSamplerDescriptorPoolCreateInfo, nullptr, &textureSamplerDescriptorPool) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create texture sampler descriptor pool" << std::endl;
		return EXIT_FAILURE;
	}

	auto textureSamplerDescriptorSetAllocateInfo = VkDescriptorSetAllocateInfo
	{
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = textureSamplerDescriptorPool,
		.descriptorSetCount = 1,
		.pSetLayouts = &descriptorSetLayout
	};

	auto textureSamplerDescriptorSet = VkDescriptorSet {};

	if (vkAllocateDescriptorSets (device, &textureSamplerDescriptorSetAllocateInfo, &textureSamplerDescriptorSet) != VK_SUCCESS)
	{
		std::cout << "error >> failed to allocate texture sampler descriptor set" << std::endl;
		return EXIT_FAILURE;
	}

	auto textureSamplerDescriptorImageCreateInfo = VkDescriptorImageInfo
	{
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
		.imageView = textureImageView,
		.sampler = textureSampler
	};

	auto textureSamplerWriteDescriptorSet = VkWriteDescriptorSet
	{
		.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet = textureSamplerDescriptorSet,
		.dstBinding = 0,
		.dstArrayElement = 0,
		.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.descriptorCount = 1,
		.pImageInfo = &textureSamplerDescriptorImageCreateInfo
	};

	vkUpdateDescriptorSets (device, 1, &textureSamplerWriteDescriptorSet, 0, nullptr);



	auto graphicsCommandPoolCreateInfo = VkCommandPoolCreateInfo 
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = graphicsFamily.value()
	};

	auto graphicsCommandPool = VkCommandPool {};

	if (vkCreateCommandPool(device, &graphicsCommandPoolCreateInfo, nullptr, &graphicsCommandPool) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create graphics command pool" << std::endl;
		return EXIT_FAILURE;
	}

	auto graphicsCommandBufferAllocateInfo = VkCommandBufferAllocateInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = graphicsCommandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1
	};

	auto graphicsCommandBuffer = VkCommandBuffer {};

	if (vkAllocateCommandBuffers (device, &graphicsCommandBufferAllocateInfo, &graphicsCommandBuffer) != VK_SUCCESS) 
	{
		std::cout << "error >> failed to allocate graphics command buffer" << std::endl;
		return EXIT_FAILURE;
	}

	auto imageAvailableSemaphore = VkSemaphore {};
	auto renderFinishedSemaphore = VkSemaphore {};
	auto inFlightFence = VkFence {};

	auto semaphoreCreateInfo = VkSemaphoreCreateInfo {.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

	if (vkCreateSemaphore (device, &semaphoreCreateInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create image available semaphore" << std::endl;
		return EXIT_FAILURE;
	} 
	
 	if (vkCreateSemaphore (device, &semaphoreCreateInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create render finished semaphore" << std::endl;
		return EXIT_FAILURE;
	}

	auto fenceCreateInfo = VkFenceCreateInfo 
	{
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT
	};
	
	 if (vkCreateFence (device, &fenceCreateInfo, nullptr, &inFlightFence) != VK_SUCCESS)
	{
		std::cout << "error >> failed to create render finished semaphore" << std::endl;
		return EXIT_FAILURE;
	}

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose (window))
	{
		glfwPollEvents ();
		vkWaitForFences (device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
		vkResetFences (device, 1, &inFlightFence);
		auto imageIndex = uint32_t {};

		//async
		vkAcquireNextImageKHR (device, swapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);



		vkResetCommandBuffer (graphicsCommandBuffer, 0);
		
		// record command buffer
		auto graphicsCommandBufferBeginInfo = VkCommandBufferBeginInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = 0,
			.pInheritanceInfo = nullptr
		};

		if (vkBeginCommandBuffer (graphicsCommandBuffer, &graphicsCommandBufferBeginInfo) != VK_SUCCESS)
		{
			std::cout << "error >> failed to begin recording graphics command buffer" << std::endl;
			return EXIT_FAILURE;
		}

		auto clearColor = VkClearValue {{{0.0f, 0.0f, 0.0f, 1.0f}}};

		auto renderPassBeginInfo = VkRenderPassBeginInfo
		{
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.renderPass = renderPass,
			.framebuffer = swapchainFramebuffers [imageIndex],
			.renderArea.offset = {0, 0},
			.renderArea.extent = surfaceExtent,
			.clearValueCount = 1,
			.pClearValues = &clearColor
		};

		vkCmdBeginRenderPass (graphicsCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline (graphicsCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
		auto viewport = VkViewport 
		{
			.x = 0.0f,
			.y = 0.0f,
			.width = (float) surfaceExtent.width,
			.height = (float) surfaceExtent.height,
			.minDepth = 0.0f,
			.maxDepth = 1.0f,
		};
		vkCmdSetViewport (graphicsCommandBuffer, 0, 1, &viewport);
		auto scissor = VkRect2D 
		{
			.offset = {0,0},
			.extent = surfaceExtent
		};
		vkCmdSetScissor (graphicsCommandBuffer, 0, 1, &scissor);
		VkBuffer verticesBuffers [] = {verticesBuffer};
		VkDeviceSize offsets [] = {0};
		vkCmdBindVertexBuffers (graphicsCommandBuffer, 0, 1, verticesBuffers, offsets);
		static auto const startTime = std::chrono::high_resolution_clock::now();
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration <float,std::chrono::seconds::period> (currentTime - startTime).count ();
		auto pushConstants = push_constants
		{
			.model = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},//glm::rotate (glm::mat4 (1.0f), time * glm::radians (90.0f), glm::vec3 (0.0f, 0.0f, 1.0f)),
			.view = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},//glm::lookAt (glm::vec3 (2.0f, 2.0f, 2.0f), glm::vec3 (0.0f,0.0f, 0.0f), glm::vec3 (0.0f, 0.0f, 1.0f)),
			.proj = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}//glm::perspective (glm::radians (45.0f), surfaceExtent.width / (float) surfaceExtent.height, 0.1f, 10.0f)
		};
		// pushConstants.proj[1][1] *= -1;
		vkCmdPushConstants (graphicsCommandBuffer, layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof (push_constants), &pushConstants);
		vkCmdBindDescriptorSets (graphicsCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, 1, &textureSamplerDescriptorSet, 0, nullptr);
		vkCmdBindIndexBuffer (graphicsCommandBuffer, indicesBuffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed (graphicsCommandBuffer, static_cast <uint32_t> (indices.size()), 1, 0, 0, 0);
		vkCmdEndRenderPass (graphicsCommandBuffer);

		if (vkEndCommandBuffer (graphicsCommandBuffer) != VK_SUCCESS)
		{
			std::cout << "error >> failed to record command buffer" << std::endl;
			return EXIT_FAILURE;
		}

		VkSemaphore waitSemaphores [] = {imageAvailableSemaphore};
		VkPipelineStageFlags waitStages [] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
		VkSemaphore signalSemaphores [] = {renderFinishedSemaphore};
		
		auto submitInfo = VkSubmitInfo
		{
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = waitSemaphores,
			.pWaitDstStageMask = waitStages,
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = signalSemaphores,
			.commandBufferCount = 1,
			.pCommandBuffers = &graphicsCommandBuffer
		};

		if (vkQueueSubmit (graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS)
		{
			std::cout << "error >> failed to submit draw command buffers" << std::endl;
			return EXIT_FAILURE;
		}

		VkSwapchainKHR swapchains [] = {swapchain};

		auto presentInfo = VkPresentInfoKHR
		{
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = signalSemaphores,
			.swapchainCount = 1,
			.pSwapchains = swapchains,
			.pImageIndices = &imageIndex,
			.pResults = nullptr
		};

		// async
		vkQueuePresentKHR (presentQueue, &presentInfo);
		// goto BAJS;
	}

	BAJS:

	vkDeviceWaitIdle (device);

	vkDestroySemaphore (device, imageAvailableSemaphore, nullptr);
	vkDestroySemaphore (device, renderFinishedSemaphore, nullptr);
	vkDestroyFence (device, inFlightFence, nullptr);

	vkDestroyCommandPool (device, graphicsCommandPool, nullptr);
	vkDestroyCommandPool (device, transferCommandPool, nullptr);

	for (auto framebuffer : swapchainFramebuffers)
	{
		vkDestroyFramebuffer (device, framebuffer, nullptr);
	}

	vkDestroyPipeline (device, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout (device, layout, nullptr);
	vkDestroyRenderPass (device, renderPass, nullptr);

	for (auto& imageView : swapchainImageViews)
	{
		vkDestroyImageView (device, imageView, nullptr);
	}

	vkDestroyBuffer (device, uniformBuffer, nullptr);
	vkFreeMemory (device, uniformBufferMemory, nullptr);

	vkDestroySwapchainKHR (device, swapchain, nullptr);

	vkDestroySampler (device, textureSampler, nullptr);
	vkDestroyImageView (device, textureImageView, nullptr);

	vkDestroyImage (device, textureImage, nullptr);
	vkFreeMemory (device, textureImageMemory, nullptr);

	vkDestroyDescriptorPool (device, textureSamplerDescriptorPool, nullptr);
	vkDestroyDescriptorSetLayout (device, descriptorSetLayout, nullptr);

	vkDestroyBuffer (device, indicesBuffer, nullptr);
	vkFreeMemory (device, indicesBufferMemory, nullptr);

	vkDestroyBuffer (device, verticesBuffer, nullptr);
	vkFreeMemory (device, verticesBufferMemory, nullptr);

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


/*
	NOTES

	window was resized ->  In that case the swap chain actually needs to be recreated from scratch and a reference to the old one must be specified 

	graphics AND compute shaders

	we need to record a command buffer for each possible image and select the right one at draw time. The alternative would be to record the command buffer again every frame, which is not as efficient.


*/