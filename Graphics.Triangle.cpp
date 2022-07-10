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

// import Delta;
// import Delta.Graphics;



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
	/* Initialize the library */
	if (!glfwInit())
		return EXIT_FAILURE;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	/* Create a windowed mode window and its OpenGL context */
	auto* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	auto appInfo = VkApplicationInfo {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Hello Triangle",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "No Engine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_2
	};

	auto instanceCreateInfo = VkInstanceCreateInfo {
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
	
	VkInstance instance;

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

	uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties (physicalDevice, nullptr, &extensionCount, nullptr);
	auto availableExtensions = std::vector <VkExtensionProperties> {extensionCount};
	vkEnumerateDeviceExtensionProperties (physicalDevice, nullptr, &extensionCount, availableExtensions.data());

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR (physicalDevice, surface, &surfaceCapabilities);

	VkExtent2D surfaceExtent;
	// choose swap extent
	if (surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		surfaceExtent = surfaceCapabilities.currentExtent;
	} else {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		surfaceExtent = VkExtent2D {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
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
	auto availableSurfaceFormats = std::vector<VkSurfaceFormatKHR> {formatCount};
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
		std::cout << "warning >> performance present mode not available, defaulting to energy present mode" << std::endl; 
	}

	VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	if (queueFamilyCount == 0) {
		std::cout << "error >> queue families not found" << std::endl;
		return EXIT_FAILURE;
	}

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

	std::vector<const char*> deviceExtensions = {
		"VK_KHR_swapchain",
		"VK_KHR_portability_subset"
	};

	VkDeviceCreateInfo logicalDeviceCreateInfo {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pQueueCreateInfos = queueCreateInfos.data(),
		.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
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

	VkQueue presentQueue;

	vkGetDeviceQueue (device, presentFamily.value(), 0, &presentQueue);

	uint32_t queueFamilyIndices[] = {graphicsFamily.value(), presentFamily.value()};

	auto swapchainCreateInfo = VkSwapchainCreateInfoKHR {
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

	auto swapchainImages = std::vector <VkImage> {};
	vkGetSwapchainImagesKHR (device, swapchain, &imageCount, nullptr);
	swapchainImages.resize (imageCount);
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

	auto renderPassCreateInfo = VkRenderPassCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.attachmentCount = 1,
		.pAttachments = &colorAttachment,
		.subpassCount = 1,
		.pSubpasses = &subpass
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
	
	auto vertexInputCreateInfo = VkPipelineVertexInputStateCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 0,
		.pVertexBindingDescriptions = nullptr, // Optional
		.vertexAttributeDescriptionCount = 0,
		.pVertexAttributeDescriptions = nullptr // Optional
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
		.polygonMode = VK_POLYGON_MODE_FILL, // LINE or POINT
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

	auto layoutCreateInfo = VkPipelineLayoutCreateInfo
	{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 0,
		.pSetLayouts = nullptr,
		.pushConstantRangeCount = 0,
		.pPushConstantRanges = nullptr
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
		.pDynamicState = nullptr, // Optional
		.layout = layout,
		.renderPass = renderPass,
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE, // Optional
		.basePipelineIndex = -1
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

	auto graphicsCommandBufferAllocInfo = VkCommandBufferAllocateInfo
	{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = graphicsCommandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1
	};

	auto graphicsCommandBuffer = VkCommandBuffer {};

	if (vkAllocateCommandBuffers (device, &graphicsCommandBufferAllocInfo, &graphicsCommandBuffer) != VK_SUCCESS) 
	{
		std::cout << "error >> failed to allocate graphics command buffer" << std::endl;
		return EXIT_FAILURE;
	}

	const auto recordGraphicsCommandBuffer = [&] (uint32_t imageIndex) noexcept -> void
	{
		auto graphicsCommandBufferBeginInfo = VkCommandBufferBeginInfo
		{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = 0,
			.pInheritanceInfo = nullptr
		};

		if (vkBeginCommandBuffer (graphicsCommandBuffer, &graphicsCommandBufferBeginInfo) != VK_SUCCESS)
		{
			std::cout << "error >> failed to begin recording graphics command buffer" << std::endl;
			std::terminate();
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
		vkCmdDraw (graphicsCommandBuffer, 3 /*vertexCount*/, 1 /*instanceCount*/, 0 /*firstVertex*/, 0 /*firstInstance*/);
		vkCmdEndRenderPass (graphicsCommandBuffer);
		if (vkEndCommandBuffer (graphicsCommandBuffer) != VK_SUCCESS)
		{
			std::cout << "error >> failed to record command buffer" << std::endl;
			std::terminate();
		}
	};

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

	auto fenceCreateInfo = VkFenceCreateInfo {.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
	
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
	}

	vkDestroySemaphore (device, imageAvailableSemaphore, nullptr);
	vkDestroySemaphore (device, renderFinishedSemaphore, nullptr);
	vkDestroyFence (device, inFlightFence, nullptr);

	vkDestroyCommandPool (device, graphicsCommandPool, nullptr);

	for (auto& framebuffer : swapchainFramebuffers)
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

	vkDestroySwapchainKHR (device, swapchain, nullptr);

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


*/