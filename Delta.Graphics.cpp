#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
export module Delta.Graphics;
import std;

template <typename T, typename U>
concept Convertible_to = std::is_convertible_v <T, U>;

export auto get_surface_capabilities (VkPhysicalDevice const & device, VkSurfaceKHR const & surface) noexcept -> VkSurfaceCapabilitiesKHR {
	
	auto capabilities = VkSurfaceCapabilitiesKHR {};

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR (device, surface, &capabilities);

	return capabilities;
}

export auto get_surface_presentation_modes (VkPhysicalDevice const & device, VkSurfaceKHR const & surface) noexcept -> std::vector <VkPresentModeKHR> {
	
	auto count = uint32_t {};
	
	vkGetPhysicalDeviceSurfacePresentModesKHR (device, surface, &count, nullptr);

	if (count != 0) {
		auto modes = std::vector <VkPresentModeKHR> {count};
		vkGetPhysicalDeviceSurfacePresentModesKHR (device, surface, &count, modes.data ());
		return modes;
	} else {
		return {};
	}
}

export auto get_surface_formats (VkPhysicalDevice const & device, VkSurfaceKHR const & surface) noexcept -> std::vector <VkSurfaceFormatKHR> {
	
	auto count = uint32_t {};
	
	vkGetPhysicalDeviceSurfaceFormatsKHR (device, surface, &count, nullptr);

	if (count != 0) {
		auto formats = std::vector <VkSurfaceFormatKHR> {count};
		vkGetPhysicalDeviceSurfaceFormatsKHR (device, surface, &count, formats.data ());
		return formats;
	} else {
		return {};
	}
}

export auto make_render_pass (VkFormat const & format, VkDevice const & device) noexcept -> VkRenderPass {
	
	auto const attachments = std::vector <VkAttachmentDescription> {
		{ 	// color attachment
			.format = format,
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
		}
	};

	// attachments from which subpasses outputs are written
	auto const color_attachments_ref = std::vector <VkAttachmentReference> {
		{
			.attachment = 0,
			.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
		}
	};

	// attachments from which subpasses inputs are read
	auto const input_attachments_ref = std::vector <VkAttachmentReference> {};


	// for (; i < input_attachments.size (); ++i) {
	// 	attachments
	// }
	
	

	auto const subpasses = std::vector <VkSubpassDescription> {
		VkSubpassDescription {
			.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
			.inputAttachmentCount = (uint32_t) input_attachments_ref.size (),
			.pInputAttachments = input_attachments_ref.data (),
			.colorAttachmentCount = (uint32_t) color_attachments_ref.size (),
			.pColorAttachments = color_attachments_ref.data (),
			.pResolveAttachments = nullptr,
			.pDepthStencilAttachment = nullptr,
			.preserveAttachmentCount = 0,
			.pPreserveAttachments = nullptr
		}
	};

	auto create_info = VkRenderPassCreateInfo {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.attachmentCount = (uint32_t) attachments.size (),
		.pAttachments = attachments.data (),
		.subpassCount = (uint32_t) subpasses.size (),
		.pSubpasses = subpasses.data ()
	};

	auto render_pass = VkRenderPass {};

	if (vkCreateRenderPass (device, &create_info, nullptr, &render_pass) != VK_SUCCESS) {
		std::cout << "error >> failed to create render pass" << std::endl;
		exit (-1);
	}

	return render_pass;
}

export auto make_shader_module (std::vector <char> && code, VkDevice const & device) noexcept -> VkShaderModule {
	auto create_info = VkShaderModuleCreateInfo {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = code.size (),
		.pCode = reinterpret_cast <uint32_t const *> (code.data ())
	};

	auto shader_module = VkShaderModule {};	

	if (vkCreateShaderModule (device, &create_info, nullptr, &shader_module) != VK_SUCCESS) {
		std::cout << "error >> failed to create shader module" << std::endl;
		exit (-1);
	}

	return shader_module;
}

export auto read_byte_file (char const * path) noexcept -> std::vector <char> {
	auto file = std::ifstream {path, std::ios::ate | std::ios::binary};

	if (!file.is_open ()) {
		std::cout << "error >> failed to open file" << std::endl;
		exit (-1);
	}

	auto file_size = file.tellg ();
	auto buffer = std::vector <char> {};
	buffer.resize (file_size);
	file.seekg (0);
	file.read (buffer.data (), file_size);
	file.close ();
	return buffer;
}

export auto make_shader_module (char const * path, VkDevice const & device) noexcept -> VkShaderModule {
	return make_shader_module (read_byte_file (path), device);
}

export struct graphics_pipeline_details {
	VkShaderModule && vert_shader_module;
	VkShaderModule && frag_shader_module;
	VkPipelineInputAssemblyStateCreateInfo const input_assembly_state {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE};
	VkPipelineVertexInputStateCreateInfo const vertex_input_state {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 0,
		.pVertexBindingDescriptions = nullptr,
		.vertexAttributeDescriptionCount = 0,
		.pVertexAttributeDescriptions = nullptr};
	VkViewport viewport {
		.x = 0.0f,
		.y = 0.0f,
		// .width = ?,
		// .height = ?,
		.minDepth = 0.0f,
		.maxDepth = 1.0f};
	VkRect2D scissor {
		// .extent = ?
		.offset = {0, 0}};
	VkPipelineViewportStateCreateInfo const viewport_state {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.pViewports = &viewport,
		.scissorCount = 1,
		.pScissors = &scissor};
	VkPipelineRasterizationStateCreateInfo const rasterization_state {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.lineWidth = 1.0f,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_CLOCKWISE,
		.depthBiasEnable = VK_FALSE,
		.depthBiasConstantFactor = 0.0f,
		.depthBiasClamp = 0.0f,
		.depthBiasSlopeFactor = 0.0f};
	VkPipelineMultisampleStateCreateInfo const multisample_state {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.sampleShadingEnable = VK_FALSE,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
		.minSampleShading = 1.0f,
		.pSampleMask = nullptr,
		.alphaToCoverageEnable = VK_FALSE,
		.alphaToOneEnable = VK_FALSE};
	VkPipelineColorBlendAttachmentState color_blend_attachment_state {
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
		.blendEnable = VK_FALSE,
		.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
		.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
		.colorBlendOp = VK_BLEND_OP_ADD,
		.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
		.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
		.alphaBlendOp = VK_BLEND_OP_ADD};
	VkPipelineColorBlendStateCreateInfo const color_blend_state {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = 1,
		.pAttachments = & color_blend_attachment_state,
		.blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}};
	std::vector <VkDynamicState> const dynamic_states {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH};
	VkPipelineDynamicStateCreateInfo const dynamic_state {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = (uint32_t) dynamic_states.size (),
		.pDynamicStates = dynamic_states.data ()};
	VkPipelineLayoutCreateInfo const layout {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 0,
		.pSetLayouts = nullptr,
		.pushConstantRangeCount = 0,
		.pPushConstantRanges = nullptr};
};

export auto make (graphics_pipeline_details && details, VkRenderPass const & render_pass, VkDevice const & device) noexcept -> std::tuple <VkPipeline, VkPipelineLayout> {
	
	// auto && vert_shader_module = std::move (details.vert_shader_module);
	// auto && frag_shader_module = std::move (details.frag_shader_module);

	auto const shader_stage_create_infos = std::array <VkPipelineShaderStageCreateInfo, 2> {

		VkPipelineShaderStageCreateInfo {
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			{},
			VK_SHADER_STAGE_FRAGMENT_BIT,
			details.frag_shader_module,
			"main"
		},

		VkPipelineShaderStageCreateInfo {
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			{},
			VK_SHADER_STAGE_VERTEX_BIT,
			details.vert_shader_module,
			"main"
		}
	};

	auto layout = VkPipelineLayout {};

	if (vkCreatePipelineLayout (device, &details.layout, nullptr, &layout) != VK_SUCCESS) {
		std::cout << "error >> failed to create pipeline layout" << std::endl;
		exit (-1);
	}
	
	auto const create_info = VkGraphicsPipelineCreateInfo {
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.stageCount = (uint32_t) shader_stage_create_infos.size (),
		.pStages = shader_stage_create_infos.data (),
		.pVertexInputState = &details.vertex_input_state,
		.pInputAssemblyState = &details.input_assembly_state,
		.pViewportState = &details.viewport_state,
		.pRasterizationState = &details.rasterization_state,
		.pMultisampleState = &details.multisample_state,
		.pDepthStencilState = nullptr,
		.pColorBlendState = &details.color_blend_state,
		.pDynamicState = &details.dynamic_state,
		.layout = layout,
		.renderPass = render_pass,
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = -1
	};

	auto pipeline = VkPipeline {};

	if (vkCreateGraphicsPipelines (device, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline) != VK_SUCCESS) {
		std::cout << "error >> failed to create graphics pipeline" << std::endl;
		exit (-1);
	}

	// for (auto & i : shader_stage_create_infos) {
	// 	vkDestroyShaderModule (device, i.)
	// }

	vkDestroyShaderModule (device, details.frag_shader_module, nullptr);
	vkDestroyShaderModule (device, details.vert_shader_module, nullptr);

	return {pipeline, layout};
}

export auto make (VkExtent2D const & extent, graphics_pipeline_details && details, auto && ... rest) noexcept -> auto {
	details.viewport.width = extent.width;
	details.viewport.height = extent.height;
	details.scissor.extent = extent;
	return make (std::move (details), std::forward <decltype (rest)> (rest)...);
}

export template <
	VkPrimitiveTopology topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
	VkBool32 primitive_restart_enable = VK_FALSE
>
auto make_graphics_pipeline (VkExtent2D const & extent, VkDevice const & device) noexcept -> VkPipeline {

	auto && vert_shader_module = make_shader_module (read_byte_file ("App.Graphics.Info.vert.spv"), device);
	auto && frag_shader_module = make_shader_module (read_byte_file ("App.Graphics.Info.frag.spv"), device);

	auto const shader_stage_create_infos = std::array <VkPipelineShaderStageCreateInfo, 2> {

		VkPipelineShaderStageCreateInfo {
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			{},
			VK_SHADER_STAGE_VERTEX_BIT,
			std::move (vert_shader_module),
			"main"
		},

		VkPipelineShaderStageCreateInfo {
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			{},
			VK_SHADER_STAGE_FRAGMENT_BIT,
			std::move (frag_shader_module),
			"main"
		}
	};

	auto const vertex_input_state_create_info = VkPipelineVertexInputStateCreateInfo {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 0,
		.pVertexBindingDescriptions = nullptr,
		.vertexAttributeDescriptionCount = 0,
		.pVertexAttributeDescriptions = nullptr
	};

	auto const input_assembly_state_create_info = VkPipelineInputAssemblyStateCreateInfo {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = topology,
		.primitiveRestartEnable = primitive_restart_enable
	};
	

	auto const viewport = VkViewport {
		.x = 0.0f,
		.y = 0.0f,
		.width = (float) extent.width,
		.height = (float) extent.height,
		.minDepth = 0.0f,
		.maxDepth = 1.0f
	};

	auto const scissor = VkRect2D {
		.offset = {0, 0},
		.extent = extent
	};

	auto const viewport_state_create_info = VkPipelineViewportStateCreateInfo {
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.pViewports = &viewport,
		.scissorCount = 1,
		.pScissors = &scissor
	};

	vkDestroyShaderModule (device, frag_shader_module, nullptr);
	vkDestroyShaderModule (device, vert_shader_module, nullptr);
}

export template <
	VkImageViewType view_type = VK_IMAGE_VIEW_TYPE_2D,
	VkComponentMapping components = VkComponentMapping {.r = VK_COMPONENT_SWIZZLE_IDENTITY, .g = VK_COMPONENT_SWIZZLE_IDENTITY, .b = VK_COMPONENT_SWIZZLE_IDENTITY, .a = VK_COMPONENT_SWIZZLE_IDENTITY},
	VkImageSubresourceRange subresource_range = VkImageSubresourceRange {.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, .baseMipLevel = 0, .levelCount = 1, .baseArrayLayer = 0, .layerCount = 1}
>
auto make_views (
	VkDevice const & device,
	std::vector <VkImage> const & images,
	VkFormat const & format
) noexcept -> std::vector <VkImageView> {

	auto views = std::vector <VkImageView> {images.size ()};
	
	for (auto i = 0; i < views.size (); ++i) {

		auto const create_info = VkImageViewCreateInfo {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = images [i],
			.viewType = view_type,
			.format = format,
			.components = components,
			.subresourceRange = subresource_range
		};

		if (vkCreateImageView (device, &create_info, nullptr, & views [i]) != VK_SUCCESS) {
			std::cout << "error >> failed to create image view" << std::endl;
			exit (-1);
		}
	}

	return views;
}

export template <
	VkImageUsageFlags image_usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
	VkCompositeAlphaFlagBitsKHR composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR
>
auto make_swapchain (VkDevice const & device, 
							VkSurfaceKHR const & surface,
							std::vector <uint32_t> && queue_indices, // queues with which the images will be used with
							VkSurfaceCapabilitiesKHR const & capabilities,
							VkSurfaceFormatKHR const & format, 
							VkExtent2D const & extent,
							VkPresentModeKHR const & presentation_mode,
							VkSwapchainKHR const & old_swapchain = VK_NULL_HANDLE) noexcept -> std::tuple <VkSwapchainKHR, std::vector <VkImage>, VkFormat, VkExtent2D> {
								
	uint32_t const & nr_of_images = capabilities.maxImageCount;

	

	auto const create_info = VkSwapchainCreateInfoKHR {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surface,
		.minImageCount = nr_of_images,
		.imageFormat = format.format,
		.imageColorSpace = format.colorSpace,
		.imageExtent = extent,
		.imageArrayLayers = 1,
		.imageUsage = image_usage,
		.imageSharingMode = VK_SHARING_MODE_CONCURRENT,
		.queueFamilyIndexCount = (uint32_t) queue_indices.size (),
		.pQueueFamilyIndices = queue_indices.data (),
		.preTransform = capabilities.currentTransform,
		.compositeAlpha = composite_alpha,
		.presentMode = presentation_mode,
		.clipped = VK_TRUE,
		.oldSwapchain = old_swapchain
	};

	auto swapchain = VkSwapchainKHR {};

	if (vkCreateSwapchainKHR (device, &create_info, nullptr, &swapchain) != VK_SUCCESS) {
		std::cout << "error >> failed to create swapchain" << std::endl;
		exit (-1);
	}

	auto count = uint32_t {};
	vkGetSwapchainImagesKHR (device, swapchain, &count, nullptr);
	auto images = std::vector <VkImage> {count};
	vkGetSwapchainImagesKHR (device, swapchain, &count, images.data ());

	return {swapchain, images, format.format, extent};
}

export auto make_window 
	(uint32_t const width, 
	uint32_t const height, 
	char const * title = "") noexcept -> GLFWwindow * {
	
	return glfwCreateWindow (width, height, title, nullptr, nullptr);
}

/*
	get layers available to instances
*/
export auto get_instance_layer_properties (
	) noexcept -> std::vector <VkLayerProperties> {
	auto count = uint32_t {0};
	vkEnumerateInstanceLayerProperties (&count, nullptr);
	auto layer_properties = std::vector <VkLayerProperties> {count};
	vkEnumerateInstanceLayerProperties (&count, layer_properties.data());
	return layer_properties;
}

/*
	Get layer properties available to a physical device
*/
export auto get_layer_properties (
	VkPhysicalDevice const& device) noexcept -> std::vector <VkLayerProperties> {
	auto count = uint32_t {0};
	vkEnumerateDeviceLayerProperties (device, &count, nullptr);
	auto layer_properties = std::vector <VkLayerProperties> {count};
	vkEnumerateDeviceLayerProperties (device, &count, layer_properties.data());
	return layer_properties;
}

/*
	Get available extensions to instances
*/
export auto get_instance_extension_properties (
	) noexcept -> std::vector <VkExtensionProperties> {
	auto count = uint32_t {0};
	vkEnumerateInstanceExtensionProperties (nullptr, &count, nullptr);
	auto extensions_properties = std::vector <VkExtensionProperties> {count};
	vkEnumerateInstanceExtensionProperties (nullptr, &count, extensions_properties.data());
	return extensions_properties;
}

/*
	get extension properties available to a physical device
*/
export auto get_extension_properties (
	VkPhysicalDevice const& device) noexcept -> std::vector <VkExtensionProperties> {
	auto count = uint32_t {0};
	vkEnumerateDeviceExtensionProperties (device, nullptr, &count, nullptr);
	auto extension_properties = std::vector <VkExtensionProperties> {count};
	vkEnumerateDeviceExtensionProperties (device, nullptr, &count, extension_properties.data());
	return extension_properties;
}

using swapchain_details_t = std::tuple <VkSurfaceCapabilitiesKHR, std::vector <VkSurfaceFormatKHR>, std::vector <VkPresentModeKHR>>;




export auto get_swapchain_details (
	VkPhysicalDevice const & device,
	VkSurfaceKHR const & surface) noexcept -> swapchain_details_t {
		
		auto details = swapchain_details_t {};

		auto & [capabilities, formats, present_modes] = details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR (device, surface, &capabilities);


		auto count = uint32_t {};


		vkGetPhysicalDeviceSurfaceFormatsKHR (device, surface, &count, nullptr);

		if (count != 0) {
			formats.resize (count);
			vkGetPhysicalDeviceSurfaceFormatsKHR (device, surface, &count, formats.data ());
		}


		vkGetPhysicalDeviceSurfacePresentModesKHR (device, surface, &count, nullptr);

		if (count != 0) {
			present_modes.resize (count);
			vkGetPhysicalDeviceSurfacePresentModesKHR (device, surface, &count, present_modes.data ());
		}


		return details;
}

/*
	Takes a vector of layer properties and makes a vector of layer names out of it
*/
export auto make_names (
	std::vector <VkLayerProperties> const& layers_properties) noexcept -> std::vector <char const*> {
	auto layer_names = std::vector <char const *> {layers_properties.size()};
	for (auto i = 0; i < layer_names.size(); ++i) {
		layer_names [i] = layers_properties [i].layerName;
	}
	return layer_names;
}

/*
	Takes a vector of extension properties and makes a vector of extension names out of it
*/
export auto make_names (
	std::vector <VkExtensionProperties> const& extension_properties) noexcept -> std::vector <char const*> {
	auto extension_names = std::vector <char const *> {extension_properties.size()};
	for (auto i = 0; i < extension_names.size(); ++i) {
		extension_names [i] = extension_properties [i].extensionName;
	}
	return extension_names;
}


export auto make_names (auto const& arg, auto (*fun) (decltype (arg))) 
{
	return make_names (fun (arg));
};

export auto make_names (auto const& arg, auto (*fun) (decltype (arg)), auto... rest) //-> decltype ()
{
	return std::make_tuple (make_names (arg, fun), make_names (arg, rest)...);
};

export auto make_app_info (
	char const * app_name = "app", 
	uint32_t app_version = VK_MAKE_VERSION (1, 0, 0),
	char const* engine_name = "engine", 
	uint32_t engine_version = VK_MAKE_VERSION (1, 0, 0), 
	uint32_t api_version = VK_MAKE_VERSION (1, 2, 0)) noexcept -> VkApplicationInfo {

	return {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pNext = nullptr,
		.pApplicationName = app_name,
		.applicationVersion = app_version,
		.pEngineName = engine_name,
		.engineVersion = engine_version,
		.apiVersion = api_version
	};
}

export auto make_instance (
	// char const * const * layer_names = nullptr,
	// uint32_t layer_count = 0,
	// char const * const * extension_names = nullptr,
	// uint32_t extension_count = 0,
	std::vector <char const*> const & layers,
	std::vector <char const*> const & extensions,
	VkApplicationInfo const & app_info = make_app_info ()) noexcept -> VkInstance {

	auto const createInfo = VkInstanceCreateInfo {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.pApplicationInfo = &app_info,
		.enabledLayerCount = static_cast <uint32_t> (layers.size()),
		.ppEnabledLayerNames = layers.data(),
		.enabledExtensionCount = static_cast <uint32_t> (extensions.size()),
		.ppEnabledExtensionNames = extensions.data()
	};

	auto instance = VkInstance {};

	if (vkCreateInstance (/*VkInstanceCreateInfo const * pCreateInfo */&createInfo, /*VkAllocationCallbacks const * pAllocator */nullptr, /*VkInstance* pInstance*/&instance) != VK_SUCCESS) {
		std::cout << "error >> failed to create instance" << std::endl;
		exit (-1);
	}

	return instance;
}

export auto make_surface (
	VkInstance const& instance,
	GLFWwindow* window) noexcept -> VkSurfaceKHR {
	
	auto surface = VkSurfaceKHR {};

	if (glfwCreateWindowSurface (instance, window, nullptr, &surface) != VK_SUCCESS) {
		std::cout << "error >> failed to create window surface" << std::endl;
		exit (-1);
	}

	return surface;
}

export auto get_devices (
	VkInstance const & instance ) noexcept -> std::vector <VkPhysicalDevice> {
		// std::cout << "yo" << std::endl;
	auto count = uint32_t {0};
	vkEnumeratePhysicalDevices (instance, &count, nullptr);
	if (count > 0) {
		auto devices = std::vector <VkPhysicalDevice> {count};
		vkEnumeratePhysicalDevices (instance, &count, devices.data());
		return devices;
	} else {
		std::cout << "error >> failed to find any physical devices" << std::endl;
		exit (-1);
		// return std::vector <VkPhysicalDevice> {0};
	}
}

export auto get_properties (
	VkPhysicalDevice const& device) noexcept -> VkPhysicalDeviceProperties {
	auto properties = VkPhysicalDeviceProperties {};
	vkGetPhysicalDeviceProperties (device, &properties);
	return properties;
}

export auto get_features (
	VkPhysicalDevice const& device) noexcept -> VkPhysicalDeviceFeatures {
	auto features = VkPhysicalDeviceFeatures {};
	vkGetPhysicalDeviceFeatures (device, &features);
	return features;
}

export auto get_memory_properties (
	VkPhysicalDevice const& device) noexcept -> VkPhysicalDeviceMemoryProperties {
	auto properties = VkPhysicalDeviceMemoryProperties {};
	vkGetPhysicalDeviceMemoryProperties (device, &properties);
	return properties;
}

export auto get_queue_family_properties (
	VkPhysicalDevice const& device) noexcept -> std::vector <VkQueueFamilyProperties> {
	auto count = uint32_t {0};
	vkGetPhysicalDeviceQueueFamilyProperties (device, &count, nullptr);
	auto queue_family_properties = std::vector <VkQueueFamilyProperties> {count};
	vkGetPhysicalDeviceQueueFamilyProperties (device, &count, queue_family_properties.data());
	return queue_family_properties;
}

export auto make_queue_create_infos (
	std::vector <VkQueueFamilyProperties> const& queue_family_properties) noexcept -> std::vector <VkDeviceQueueCreateInfo> {
		auto queue_create_infos = std::vector <VkDeviceQueueCreateInfo> {queue_family_properties.size()};
		// queue_create_infos.reserve (queue_family_properties.size());
		auto priority = float {1};
		for (auto i = 0; i < queue_family_properties.size(); ++i) {
			queue_create_infos [i] = {
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.flags = 0,
				.queueFamilyIndex = static_cast <uint32_t> (i),
				.queueCount = queue_family_properties [i].queueCount,
				.pQueuePriorities = &priority
			};
		}
		// std::cout << queue_create_infos.size () << std::endl;
		return queue_create_infos;
}

using indexed_queues_t = std::vector <std::tuple <VkQueueFlags, std::vector <VkQueue>>>;

export template <
	VkCommandPoolCreateFlags flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT
>
auto make_command_pools (VkDevice const& device, indexed_queues_t const& queues) noexcept -> std::vector <VkCommandPool> {

	auto command_pool_create_infos = std::vector <VkCommandPoolCreateInfo> {queues.size()};
	auto indexed_command_pools = std::vector <VkCommandPool> {queues.size()};

	for (auto i = 0; i < queues.size(); ++i) {
		// auto & info = command_pool_create_infos [i];
		auto const info = VkCommandPoolCreateInfo {
			.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
			.pNext = nullptr,
			.flags = flags,
			.queueFamilyIndex = static_cast <uint32_t> (i)
		};

		if (vkCreateCommandPool (device, &info, nullptr, &indexed_command_pools [i]) != VK_SUCCESS) {
			std::cout << "error >> failed to create command pool" << std::endl;
			exit (-1);
		}
	}
	return indexed_command_pools;
}

/*
	makes device, indexed queues and indexed command pools
*/
export auto make_device (
	VkPhysicalDevice const & physical_device,
	std::vector <char const*> const & layers,
	std::vector <char const*> const & extensions,
	VkPhysicalDeviceFeatures const & features) noexcept -> std::tuple <VkDevice, indexed_queues_t, std::vector <VkCommandPool>> {
	
	auto queue_family_properties = get_queue_family_properties (physical_device);
	
	auto queue_create_infos = std::vector <VkDeviceQueueCreateInfo> {queue_family_properties.size()};
	auto indexed_queues = indexed_queues_t {queue_family_properties.size()};

	auto const priority = float {1};
	for (auto i = 0; i < queue_family_properties.size(); ++i) {
		queue_create_infos [i] = {
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.flags = 0,
			.queueFamilyIndex = static_cast <uint32_t> (i),
			.queueCount = queue_family_properties [i].queueCount,
			.pQueuePriorities = &priority
		};
		auto& [flags, family_queues] = indexed_queues [i];
		flags = queue_family_properties [i].queueFlags;		
		family_queues.resize (queue_family_properties [i].queueCount);
		// std::get <0> (indexed_queues [i]) = queue_family_properties [i].queueFlags;
		// std::get <1> ()
	}
	
	auto const createInfo = VkDeviceCreateInfo {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.queueCreateInfoCount = static_cast <uint32_t> (queue_create_infos.size()),
		.pQueueCreateInfos = queue_create_infos.data(),
		.enabledLayerCount = static_cast <uint32_t> (layers.size()),
		.ppEnabledLayerNames = layers.data(),
		.enabledExtensionCount = static_cast <uint32_t> (extensions.size()),
		.ppEnabledExtensionNames = extensions.data(),
		.pEnabledFeatures = &features
	};

	auto device = VkDevice {};

	

	if (vkCreateDevice (physical_device, &createInfo, nullptr, &device) != VK_SUCCESS) {
		std::cout << "error >> failed to create device" << std::endl;
		exit (-1);
	}

	for (auto i = 0; i < indexed_queues.size(); ++i) {
		auto& [flags, queues] = indexed_queues [i];
		for (auto j = 0; j < queues.size(); ++j) {
			vkGetDeviceQueue (device, i, j, &queues [j]);
		}
	}

	return {device, indexed_queues, make_command_pools (device, indexed_queues)};
}



// export auto make_device2 (
// 	VkPhysicalDevice const& physical_device) {
	
// 	auto [layer_names, extension_names] = make_names (physical_device, get_layer_properties, get_extension_properties);
// 	auto queue_create_infos = make_queue_create_infos (get_queue_family_properties (physical_device));
// 	// auto queue_family_properties = get_queue_family_properties_fun (physical_device);
// 	auto features = get_features (physical_device);
// 	return make_device (physical_device, queue_create_infos, layer_names, extension_names, features);
// }



/*
	Destroy instance
*/
// export auto destroy (
// 	VkInstance& instance,
// 	VkAllocationCallbacks const * allocator = nullptr) noexcept -> void {
// 	vkDestroyInstance (instance, allocator);
// }

/*
	Destroy device
*/
// export auto destroy (
// 	VkDevice& device,
// 	VkAllocationCallbacks const * allocator = nullptr) noexcept -> void {
// 	// ensure there is no work executing on device
// 	vkDeviceWaitIdle (device);
// 	vkDestroyDevice (device, allocator);
// }

export auto get_layer_properties2 (
	VkPhysicalDevice const& device) noexcept -> std::vector <VkLayerProperties> {
	auto count = uint32_t {0};
	vkEnumerateDeviceLayerProperties (device, &count, nullptr);
	auto layer_properties = std::vector <VkLayerProperties> {count};
	vkEnumerateDeviceLayerProperties (device, &count, layer_properties.data());
	return layer_properties;
}



// export auto make_names (auto const& a, auto (*fun) (decltype (a)), auto (*) (decltype (a)), auto (*) (decltype (a))) {

// };

export auto make_physical_devices_getter (VkInstance& instance) {
	static auto devices = get_devices (instance);
	return [&d = devices] () -> auto & {
		return d;
	};
}


// auto make_instance_creater = [] (VkInstance& instance) {
// 	return [&instance] () {

// 	};
// };






