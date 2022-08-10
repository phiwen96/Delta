#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"
import Delta;


template <typename T, typename U>
concept Bigger_than = sizeof (T) > sizeof (U);

// color depth
auto choose_surface_format (VkPhysicalDevice const & device, VkSurfaceKHR const & surface) noexcept -> VkSurfaceFormatKHR { 
	
	for (auto const & i : get_surface_formats (device, surface)) {
		if (i.format == VK_FORMAT_B8G8R8A8_SRGB and i.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return i;
		}
	}

	std::cout << "error >> failed to find a suitable surface format" << std::endl;
	exit (-1);
};

auto choose_surface_presentation_mode (VkPhysicalDevice const & device, VkSurfaceKHR const & surface) noexcept -> VkPresentModeKHR { 
	
	for (auto const & i : get_surface_presentation_modes (device, surface)) {
		if (i == VK_PRESENT_MODE_MAILBOX_KHR) {
			return i;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
};

auto choose_surface_extent (VkPhysicalDevice const & device, VkSurfaceKHR const & surface, GLFWwindow * window) noexcept -> VkExtent2D { 
	
	auto const capabilities = get_surface_capabilities (device, surface);

	auto width = int {};
	auto height = int {};

	glfwGetFramebufferSize (window, &width, &height);

	return {(uint32_t) width, (uint32_t) height};

	std::cout << "error >> failed to find a suitable surface extent" << std::endl;
	exit (-1);
};

auto window_resized = false;

auto resize_callback (GLFWwindow* window, int width, int height) noexcept -> void {	
	window_resized = true;
	// std::cout << "whaat" << std::endl;
	// reinterpret_cast <Window*> (glfwGetWindowUserPointer (window)) -> resized = true;
}

auto printNexit (char const* txt) noexcept -> void {
	std::cout << txt << std::endl;
	exit (-1);
}

struct TextureBitmap {
	uint32_t width;
	uint32_t height;
	unsigned char * buffer;
};

auto main (int argc, char** argv) -> int {



	TextureBitmap font_bitmap;
	{
		auto const columns = 15;
		auto const rows = 20;
		auto const padding = 2;
		
		auto const font_size = int {48};
		
		auto lib = FT_Library {};

		if (FT_Init_FreeType (&lib)) printNexit ("error >> failed to initialize freetype lib");
		
		auto face = FT_Face {};
		// "/Impact.ttf"
		if (FT_New_Face (lib, (std::string {FONTS_DIR} + "/Charter.ttc").c_str (), 0, &face)) printNexit ("error >> failed to load font file");

		if (FT_Set_Pixel_Sizes (face, 0, font_size)) printNexit ("error >> failed to set pixel sizes");

		uint32_t image_width = (font_size + padding) * columns; 
		uint32_t image_height = (font_size + padding) * rows;
		auto * buffer = new unsigned char [image_width * image_height * 4];
		auto * widths = new int [255];

		auto max_under_baseline = int {0};

		auto glyph_index = FT_UInt {};

		for (auto i = 32; i < 255; ++i) {
			glyph_index = FT_Get_Char_Index (face, i);  
			if (FT_Load_Glyph (face, glyph_index, FT_LOAD_DEFAULT)) printNexit ("error >> failed to load glyph");
			auto const & glyph_metrics = face->glyph->metrics;
			auto glyph_hang = (glyph_metrics.horiBearingY - glyph_metrics.height) / 64;
			if (glyph_hang < max_under_baseline) max_under_baseline = glyph_hang;
		}



		for (auto i = 32; i < 255; ++i) {
			glyph_index = FT_Get_Char_Index (face, i); 
			if (FT_Load_Glyph (face, glyph_index, FT_LOAD_DEFAULT)) printNexit ("error >> failed to load glyph");
			if (FT_Render_Glyph (face->glyph, FT_RENDER_MODE_NORMAL)) printNexit ("error >> failed to render glyph");
			widths[i] = face->glyph->metrics.width/64;

			auto x = ((i - 33) % columns) * (font_size + padding);
			auto y = ((i - 33 )/ columns) * (font_size + padding);
			x += 1;
			y += (font_size + padding) - face->glyph->bitmap_top + max_under_baseline - 1;
			
			auto const & bitmap = face -> glyph -> bitmap;
			// font_bitmap.buffer = bitmap.buffer;
			// font_bitmap.width = bitmap.width;
			// font_bitmap.height = bitmap.rows;
			// break;
			for (auto xx = 0; xx < bitmap.width; ++xx) {
				for (auto yy = 0; yy < bitmap.rows; ++yy) {
					auto r = bitmap.buffer [(yy * (bitmap.width) + xx)];
					buffer [(y + yy) * image_width * 4 - (x + xx) * 4 + 0] = r;
					buffer [(y + yy) * image_width * 4 - (x + xx) * 4 + 1] = r;
					buffer [(y + yy) * image_width * 4 - (x + xx) * 4 + 2] = r;
					buffer [(y + yy) * image_width * 4 - (x + xx) * 4 + 3] = 255;
				}
			}

			// if (i == 50) break;
		}

		delete [] widths;
		font_bitmap.buffer = buffer;
		font_bitmap.width = image_width;
		font_bitmap.height = image_height;
	}

	// return 0;

	glfwInit ();

	// auto const framebuffer_resize_callback = []

	auto window = Details <Window> {
		.extent = {
			.width = font_bitmap.width,//640,
			.height = font_bitmap.height},//480},
		.hints = {
			{GLFW_CLIENT_API, GLFW_NO_API}
			// {GLFW_RESIZABLE, GLFW_FALSE}
		}
	} ();

	window.set_resize_callback (resize_callback);

	// std::cout << make_names (get_instance_extension_properties ()) << std::endl;
	// std::cout << make_names (get_instance_layer_properties ()) << std::endl << std::endl;;

	auto const instance = Details <VkInstance> {
		.layers = {"VK_LAYER_KHRONOS_validation"/*, "VK_LAYER_LUNARG_api_dump"*/},
		.extensions = {"VK_KHR_portability_enumeration", "VK_KHR_surface", "VK_EXT_metal_surface", "VK_MVK_macos_surface", "VK_KHR_get_physical_device_properties2"}
	} ();

	auto const surface = Details <VkSurfaceKHR> {
		.instance = instance,
		.window = window.handle
	} ();

	auto const physical_device = Details <PhysicalDevice> {
		.instance = instance,
		.picker = [] (std::vector <VkPhysicalDevice> const & devices) -> VkPhysicalDevice {
			return devices.front ();
		}
	} ();

	// std::cout << make_names (get_layer_properties (physical_device.handle)) << std::endl;
	// VkAccelerationStructureGeometryTrianglesDataKHR ddd {};
	// vkCmdBuildAccelerationStructuresKHR ();
	auto device = Details <LogicalDevice> {
		.physical_device = physical_device,
		.surface = surface,
		.extensions = {"VK_KHR_swapchain", "VK_KHR_portability_subset", "VK_KHR_dynamic_rendering", "VK_KHR_depth_stencil_resolve", "VK_KHR_create_renderpass2"},
		.features = {.samplerAnisotropy = true},
		.layers = {}
	} ();

	auto render_pass_details = Details <RenderPass> {
		.device = device.handle,
		.attachment_descriptions = {
			{
				.format = VK_FORMAT_B8G8R8A8_SRGB,
				.samples = VK_SAMPLE_COUNT_1_BIT,
				.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
				.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
				.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
				.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
				.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
			}
		},
		.attachment_references = {
			{
				.attachment = 0,
				.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
			}
		},
		.subpass_descriptions = {
			{
				.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
				.colorAttachmentCount = 1,
			}
		},
		.subpass_dependencies = {
			{
				.srcSubpass = VK_SUBPASS_EXTERNAL,
				.dstSubpass = 0,
				.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				.srcAccessMask = 0,
				.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
			}
		}
	};

	render_pass_details.subpass_descriptions[0].pColorAttachments = &render_pass_details.attachment_references[0];

	auto render_pass = render_pass_details ();
	
	auto swapchain_details = Details <Swapchain> {
		.device = device.handle,
		.surface = surface,
		.render_pass = render_pass.handle,
		.image_count = get_surface_capabilities (physical_device.handle, surface).maxImageCount,
		.image_format = VK_FORMAT_B8G8R8A8_SRGB,
		.image_color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
		.image_extent = window.get_extent (),
		.image_usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.image_sharing_mode = VK_SHARING_MODE_EXCLUSIVE,
		.pre_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
		.composite_alpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.present_mode = VK_PRESENT_MODE_FIFO_KHR,
		.clipped = VK_TRUE,
		.view_type = VK_IMAGE_VIEW_TYPE_2D,
		.view_components {
			.r = VK_COMPONENT_SWIZZLE_IDENTITY, 
			.g = VK_COMPONENT_SWIZZLE_IDENTITY, 
			.b = VK_COMPONENT_SWIZZLE_IDENTITY, 
			.a = VK_COMPONENT_SWIZZLE_IDENTITY},
		.view_subresource_range {
			.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT, 
			.baseMipLevel = 0,
			.levelCount = 1, 
			.baseArrayLayer = 0, 
			.layerCount = 1}
	};

	auto swapchain = swapchain_details ();

	auto graphics_pipeline_details = Details <GraphicsPipeline> {
		.device = device,
		.render_pass = render_pass.handle,
		.image_format = VK_FORMAT_B8G8R8A8_SRGB,
		.shaders = {{"Graphics.Test.vert.spv", VK_SHADER_STAGE_VERTEX_BIT}, {"Graphics.Test.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT}},
		.vertex_binding_descriptions = {
			{
				.binding = 0,
				.stride = sizeof (Vertex),
				.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
			}},
		.vertex_attribute_descriptions = {
			{
				.binding = 0,
				.location = 0,
				.format = VK_FORMAT_R32G32B32_SFLOAT,
				.offset = offsetof (Vertex, pos)
			},
			{
				.binding = 0,
				.location = 1,
				.format = VK_FORMAT_R32G32B32_SFLOAT,
				.offset = offsetof (Vertex, color)
			},
			{
				.binding = 0,
				.location = 2,
				.format = VK_FORMAT_R32G32_SFLOAT,
				.offset = offsetof (Vertex, tex_coord)
			}},
		.input_assembly = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
			.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
			.primitiveRestartEnable = VK_FALSE},
		.viewports = {
			{
				.x = 0.0f,
				.y = 0.0f,
				.width = (float) swapchain.image_extent.width,
				.height = (float) swapchain.image_extent.height,
				.minDepth = 0.0f,
				.maxDepth = 1.0f
			}},
		.scissors = {
			{
				.offset = {0, 0},
				.extent = swapchain.image_extent
			}},
		.rasterization = {
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
			.depthBiasSlopeFactor = 0.0f},
		.multisample = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
			.sampleShadingEnable = VK_FALSE,
			.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
			.minSampleShading = 1.0f,
			.pSampleMask = nullptr,
			.alphaToCoverageEnable = VK_FALSE,
			.alphaToOneEnable = VK_FALSE},
		.color_blend_attachments = {
			{
				.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
				.blendEnable = VK_FALSE,
				.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
				.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
				.colorBlendOp = VK_BLEND_OP_ADD,
				.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
				.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
				.alphaBlendOp = VK_BLEND_OP_ADD
			}},
		.color_blend = {
			.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
			.logicOpEnable = VK_FALSE,
			.logicOp = VK_LOGIC_OP_COPY,
			.blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}},
		.dynamic_states = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_LINE_WIDTH},
		.descriptor_set_layout_bindings = {
			{
				.binding = 0,
				.descriptorCount = 1,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.pImmutableSamplers = nullptr,
				.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
			}},
		.push_constant_ranges = {
			{
				.offset = 0,
				.size = sizeof (mvp_push_constants),
				.stageFlags = VK_SHADER_STAGE_VERTEX_BIT
			}}
	};
	// auto hh = __builtin_choose_expr (0 == 0, true, false);
	// static_assert (Convertible_to <int, std::string>);
	// Convertible_to <int> auto ss = std::string {};
	auto graphics_pipeline = graphics_pipeline_details ();
	
	auto presentation_queue = [&] -> Queue {
		for (auto const & queue_family : device.queue_families) {
			if (queue_family.present_support) {
				return queue_family.handles.front ();
			}
		}
		std::cout << "error >> failed to find a presentation queue" << std::endl;
		exit (-1);
	} ();

	auto graphics_queue = [&] -> Queue {
		for (auto const & queue_family : device.queue_families) {
			if (queue_family.supports_graphics_operations ()) {
				return queue_family.handles.front ();
			}
		}
		std::cout << "error >> failed to find a graphics queue" << std::endl;
		exit (-1);
	} ();

	auto transfer_queue = [&] -> Queue {
		for (auto i = device.queue_families.size () - 1; i >= 0; --i){
			if (device.queue_families[i].supports_graphics_operations ()) {
				return device.queue_families[i].handles.front ();
			}
		}
		std::cout << "error >> failed to find a graphics queue" << std::endl;
		exit (-1);
	} ();

	auto const max_frames_in_flight = swapchain.images.size ();

	auto current_frame = 0;

	auto draw_command_buffer = [&] -> std::vector <CommandBuffer> {
		auto buffers = std::vector <CommandBuffer> {max_frames_in_flight};

		for (auto const & queue_family : device.queue_families) {
			if (queue_family.supports_graphics_operations ()) {
				for (auto & buffer : buffers) {
					buffer = queue_family.command_pool.make_command_buffer (VK_COMMAND_BUFFER_LEVEL_PRIMARY, device.handle);
				}
				return buffers;
			}
		}
		std::cout << "error >> failed to find a graphics queue" << std::endl;
		exit (-1);
	} ();
	
	// device.make_graphics_command_buffer (VK_COMMAND_BUFFER_LEVEL_PRIMARY);

	auto pushConstants = mvp_push_constants {
		.model = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},//glm::rotate (glm::mat4 (1.0f), time * glm::radians (90.0f), glm::vec3 (0.0f, 0.0f, 1.0f)),
		.view = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},//glm::lookAt (glm::vec3 (2.0f, 2.0f, 2.0f), glm::vec3 (0.0f,0.0f, 0.0f), glm::vec3 (0.0f, 0.0f, 1.0f)),
		.proj = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}},//glm::perspective (glm::radians (45.0f), surfaceExtent.width / (float) surfaceExtent.height, 0.1f, 10.0f)
	};

	auto vertices = std::vector <Vertex> {
		// {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		// {{1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		// {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		// {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		// {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		// {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}

		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}

		// {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		// {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		// {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		// {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		// {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		// {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}
	};

		

	auto device_local_vertex_buffer = Details <Buffer> {
		.device = device,
		.size = sizeof (vertices [0]) * vertices.size (),
		.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		.sharing_mode = VK_SHARING_MODE_EXCLUSIVE
	} ();

	auto device_local_vertex_buffer_memory = Details <DeviceMemory> {
		.physical_device = physical_device,
		.device = device,
		.size = device_local_vertex_buffer.get_memory_requirements ().size,
		.flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	} ();

	device_local_vertex_buffer_memory.bind (device_local_vertex_buffer);

	{
		auto host_visible_vertex_buffer = Details <Buffer> {
			.device = device,
			.size = sizeof (vertices [0]) * vertices.size (),
			.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			.sharing_mode = VK_SHARING_MODE_EXCLUSIVE
		} ();

		auto host_visible_vertex_buffer_memory = Details <DeviceMemory> {
			.physical_device = physical_device,
			.device = device,
			.size = host_visible_vertex_buffer.get_memory_requirements ().size,
			.flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		} ();

		host_visible_vertex_buffer_memory.bind (host_visible_vertex_buffer).paste (vertices.data(), host_visible_vertex_buffer.get_memory_requirements ().size);
		device_local_vertex_buffer.copy_from (host_visible_vertex_buffer);

		host_visible_vertex_buffer.destroy ();
		host_visible_vertex_buffer_memory.destroy ();
	}

	// auto texture_image = Details <Image> {
	// 	.physical_device = physical_device,
	// 	.device = device,
	// 	.format = VK_FORMAT_R8G8B8A8_SRGB,
	// 	.path = "Graphics.Triangle.Texture.Lion.jpeg"
	// } ();

	auto texture_image = Details_image2 {
		.physical_device = physical_device,
		.device = device,
		.format = VK_FORMAT_R8G8B8A8_SRGB,
		.tex_width = font_bitmap.width,
		.tex_height = font_bitmap.height,
		.pixels = font_bitmap.buffer
	} ();

	auto texture_sampler = Details <Sampler> {
		.device = device
	} ();

	auto descriptor_pool = Details <DescriptorPool> {
		.device = device,
		.pool_sizes = {
			{
				.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = (uint32_t) max_frames_in_flight
			}
		},
		.max_sets = (uint32_t) max_frames_in_flight
	} ();


	auto descriptor_sets = descriptor_pool.allocate ({max_frames_in_flight, graphics_pipeline.descriptor_set_layout});
	{
		auto const image_info = VkDescriptorImageInfo {
			.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			.imageView = texture_image.view,
			.sampler = texture_sampler.handle
		};

		auto descriptor_writes = std::vector <VkWriteDescriptorSet> {
			{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.dstBinding = 0,
				.dstArrayElement = 0,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = 1,
				.pImageInfo = &image_info,
			}
		};

		for (auto const & descriptor_set : descriptor_sets){
			descriptor_writes[0].dstSet = descriptor_set;
			vkUpdateDescriptorSets (device.handle, static_cast <uint32_t> (descriptor_writes.size()), descriptor_writes.data(), 0, nullptr);
		}
	}

	auto const rendering_tape = [&] (uint32_t image_index) noexcept -> void {

		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float,std::chrono::seconds::period>(currentTime -startTime).count();
		pushConstants.model = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};//glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),glm::vec3(0.0f, 0.0f, 1.0f));
		pushConstants.view =  {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};//glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		pushConstants.proj = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};//glm::perspective(glm::radians(45.0f),swapchain.image_extent.width / (float) swapchain.image_extent.height, 0.1f,10.0f);
		// pushConstants.proj[1][1] *= -1;
		auto const clear_value = VkClearValue {
			{{0.0f, 0.0f, 0.0f, 1.0f}}
		};

		auto const render_pass_begin_info = VkRenderPassBeginInfo {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.renderPass = render_pass.handle,
			.framebuffer = swapchain.framebuffers [image_index],
			.renderArea = {
				.offset = {0, 0},
				.extent = swapchain.image_extent
			},
			.clearValueCount = 1,
			.pClearValues = &clear_value
		};

		vkCmdBeginRenderPass (draw_command_buffer [current_frame].handle, &render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline (draw_command_buffer [current_frame].handle, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline.handle);
		draw_command_buffer [current_frame].set_viewport (
			VkViewport {
				.x = 0.0f,
				.y = 0.0f,
				.width = (float) swapchain.image_extent.width,
				.height = (float) swapchain.image_extent.height,
				.minDepth = 0.0f,
				.maxDepth = 1.0f
			}
		);
		draw_command_buffer [current_frame].set_scissor (
			VkRect2D {
				.offset = {0, 0},
				.extent = swapchain.image_extent
			}
		);
		VkBuffer vertex_buffers[] = {device_local_vertex_buffer.handle};
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers (draw_command_buffer [current_frame].handle, 0, 1, vertex_buffers, offsets);

		vkCmdPushConstants (draw_command_buffer [current_frame].handle, graphics_pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof (mvp_push_constants), &pushConstants);
		
		vkCmdBindDescriptorSets(draw_command_buffer [current_frame].handle, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline.layout, 0, 1, &descriptor_sets [current_frame], 0, nullptr);
		vkCmdDraw (draw_command_buffer [current_frame].handle, static_cast<uint32_t> (vertices.size()), 1, 0, 0);
		vkCmdEndRenderPass (draw_command_buffer [current_frame].handle);
	};

	auto image_ready_for_rendering = Details <std::vector <VkSemaphore>> {.device = device.handle, .count = max_frames_in_flight} ();
	auto image_finished_rendering = Details <std::vector <VkSemaphore>> {.device = device.handle, .count = max_frames_in_flight} ();

	auto i = 0;
	// auto wait_semaphores = std::vector <VkSemaphore> {
	// 	image_ready_for_rendering
	// };
	// auto done_semaphores = std::vector <VkSemaphore> {
	// 	image_finished_rendering
	// };
	// auto wait_stages = std::vector <VkPipelineStageFlags> {
	// 	VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
	// };

	auto command_buffer_in_use = Details <std::vector <VkFence>> {.device = device.handle, .flags = VK_FENCE_CREATE_SIGNALED_BIT, .count = max_frames_in_flight} ();

	// // main loop
	while (not window.should_close ()) {
		glfwPollEvents ();
		// auto image_index = AcquireNextImageIndex {
		// 	.swapchain = swapchain,
		// 	.signal_when_done = {
		// 		.semaphore = image_ready_for_rendering
		// 	}
		// } ();
		
		if (window_resized) {
			device.wait_idle ();
			swapchain_details.image_extent = window.get_extent ();
			swapchain_details.recreate (swapchain);
			window_resized = false;
		}
		
		auto const image_index = swapchain.acquire_next_image (image_ready_for_rendering [current_frame]);

		
		
		device.wait_for_fence (command_buffer_in_use [current_frame]);
		device.reset_fence (command_buffer_in_use [current_frame]);

		draw_command_buffer [current_frame].reset().record (rendering_tape, image_index);

		VkPipelineStageFlags wait_stages [] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

		auto const submit_info = VkSubmitInfo {
			.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &image_ready_for_rendering [current_frame],
			.signalSemaphoreCount = 1,
			.pSignalSemaphores = &image_finished_rendering [current_frame],
			.pWaitDstStageMask = wait_stages,
			.commandBufferCount = 1,
			.pCommandBuffers = &draw_command_buffer [current_frame].handle,
		};
		vkQueueSubmit (graphics_queue.handle, 1, &submit_info, command_buffer_in_use [current_frame]);
		// auto const wait_semaphore_infos = std::array <VkSemaphoreSubmitInfo, 1> {
		// 	VkSemaphoreSubmitInfo {
		// 		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
		// 		.semaphore = image_ready_for_rendering [current_frame],
		// 		.stageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		// 	}
		// };

		// auto const signal_semaphore_infos = std::array <VkSemaphoreSubmitInfo, 1> {
		// 	VkSemaphoreSubmitInfo {
		// 		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_SUBMIT_INFO,
		// 		.semaphore = image_finished_rendering [current_frame],
		// 		.stageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		// 	}
		// };

		// auto const command_buffer_infos = std::array <VkCommandBufferSubmitInfo, 1> {
		// 	VkCommandBufferSubmitInfo {
		// 		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
		// 		.commandBuffer = draw_command_buffer [current_frame].handle
		// 	}
		// };

		// auto const submit_info = VkSubmitInfo2 {
		// 	.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		// 	.waitSemaphoreInfoCount = (uint32_t) wait_semaphore_infos.size(),
		// 	.pWaitSemaphoreInfos = wait_semaphore_infos.data(),
		// 	.signalSemaphoreInfoCount = (uint32_t) signal_semaphore_infos.size(),
		// 	.pSignalSemaphoreInfos = signal_semaphore_infos.data(),
		// 	.commandBufferInfoCount = (uint32_t) command_buffer_infos.size(),
		// 	.pCommandBufferInfos = command_buffer_infos.data(),
		// };
		
		
		// graphics_queue.submit (submit_info, command_buffer_in_use [current_frame]);
		

		auto const present_info = VkPresentInfoKHR {
			.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &image_finished_rendering [current_frame],
			.swapchainCount = 1,
			.pSwapchains = &swapchain.handle,
			.pImageIndices = &image_index,
			.pResults = nullptr
		};

		presentation_queue.present (present_info);

		current_frame = (current_frame + 1) % max_frames_in_flight;
		// break;
		// ++i;
		// if (i == 3) break;
	}
	device.wait_idle ();

	auto const destroy = [&] <typename T> (T a) noexcept -> void {
		if constexpr (requires {a.destroy ();}) {
			a.destroy ();
		} else if constexpr (std::is_same_v <T, VkInstance>) {
			std::cout << "cleanup >> instance" << std::endl;
			vkDestroyInstance (instance, nullptr);
		} else if constexpr (std::is_same_v <T, LogicalDevice>) {
			std::cout << "cleanup >> command pools" << std::endl;
			for (auto & i : a.queue_families) {
				vkDestroyCommandPool (device.handle, i.command_pool.handle, nullptr);
			}
			std::cout << "cleanup >> device" << std::endl;
			vkDeviceWaitIdle (device.handle);
			vkDestroyDevice (device.handle, nullptr);
		} else if constexpr (std::is_same_v <T, std::vector <VkCommandPool>>) {
			std::cout << "cleanup >> command pools" << std::endl;
			for (auto const & i : a) {
				vkDestroyCommandPool (device.handle, i, nullptr);
			}
		} else if constexpr (std::is_same_v <T, GLFWwindow*>) {
			std::cout << "cleanup >> window" << std::endl;
			glfwDestroyWindow (a);
		} else if constexpr (std::is_same_v <T, VkSurfaceKHR>) {
			std::cout << "cleanup >> window surface" << std::endl;
			vkDestroySurfaceKHR (instance, a, nullptr);
		} else if constexpr (std::is_same_v <T, Swapchain>) {
			std::cout << "cleanup >> frame buffers" << std::endl;
			std::cout << "cleanup >> image views" << std::endl;

			for (auto i = 0; i < a.framebuffers.size (); ++i) {
				vkDestroyFramebuffer (device.handle, a.framebuffers [i], nullptr);
				vkDestroyImageView (device.handle, a.views [i], nullptr);
			}
			std::cout << "cleanup >> swapchain" << std::endl;
			vkDestroySwapchainKHR (device.handle, a.handle, nullptr);
		} else if constexpr (std::is_same_v <T, std::vector <VkImageView>>) {
			std::cout << "cleanup >> image views" << std::endl;
			for (auto & i : a)
			vkDestroyImageView (device.handle, i, nullptr);
		} else if constexpr (std::is_same_v <T, VkRenderPass>) {
			std::cout << "cleanup >> render pass" << std::endl;
			vkDestroyRenderPass (device.handle, a, nullptr);
		} else if constexpr (std::is_same_v <T, VkPipelineLayout>) {
			std::cout << "cleanup >> pipeline layout" << std::endl;
			vkDestroyPipelineLayout (device.handle, a, nullptr);
		} else if constexpr (std::is_same_v <T, VkPipeline>) {
			std::cout << "cleanup >> pipeline" << std::endl;
			vkDestroyPipeline (device.handle, a, nullptr);
		} else if constexpr (std::is_same_v <T, GraphicsPipeline>) {
			std::cout << "cleanup >> pipeline layout" << std::endl;
			vkDestroyPipelineLayout (device.handle, a.layout, nullptr);
			std::cout << "cleanup >> pipeline" << std::endl;
			vkDestroyPipeline (device.handle, a.handle, nullptr);
		} else if constexpr (std::is_same_v <T, VkSemaphore>) {
			std::cout << "cleanup >> semaphore" << std::endl;
			vkDestroySemaphore (device.handle, a, nullptr);
		} else if constexpr (std::is_same_v <T, VkFence>) {
			std::cout << "cleanup >> fence" << std::endl;
			vkDestroyFence (device.handle, a, nullptr);
		} else if constexpr (std::is_same_v <T, std::vector <VkSemaphore>>) {
			for (auto & sem : a) {
				std::cout << "cleanup >> semaphore" << std::endl;
				vkDestroySemaphore (device.handle, sem, nullptr);
			}
		} else if constexpr (std::is_same_v <T, std::vector <VkFence>>) {
			for (auto & fence : a) {
				std::cout << "cleanup >> fence" << std::endl;
				vkDestroyFence (device.handle, fence, nullptr);
			}
		}
	};

	destroy (command_buffer_in_use);
	destroy (image_ready_for_rendering);
	destroy (image_finished_rendering);

	destroy (graphics_pipeline);
	// destroy (graphics_pipeline_layout);
	destroy (render_pass);
	// destroy (command_pools);
	// free (mem);
	// destroy (views);
	device_local_vertex_buffer.destroy ();
	device_local_vertex_buffer_memory.destroy ();
	texture_sampler.destroy ();
	texture_image.destroy ();
	descriptor_pool.destroy ();
	// texture_image_memory.destroy ();
	destroy (swapchain);
	destroy (device);
	destroy (surface);

	destroy (instance);
	window.destroy ();

	glfwTerminate ();


	return 0;
}