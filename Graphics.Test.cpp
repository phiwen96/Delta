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

constexpr auto font_size = 128;
constexpr auto padding = 2;
auto bitmap = FontBitmap3 <(font_size+padding)*16, (font_size+padding)*8> {};


template <typename... T>
struct overload : T... {
	overload (T... t) noexcept : T {t}... {}
};





auto main (int argc, char** argv) -> int {
	

	auto bitm = FontBitmap {}; 
	int max_under_baseline = 0;
	int max_height = 0;
	
	
	{
		

		auto lib = FT_Library {};

		if (FT_Init_FreeType (&lib)) printNexit ("error >> failed to initialize freetype lib");
		
		auto face = FT_Face {};
		// "/Impact.ttf"
		if (FT_New_Face (lib, (std::string {FONTS_DIR} + "/Charter.ttc").c_str (), 0, &face)) printNexit ("error >> failed to load font file");

		if (FT_Set_Pixel_Sizes (face, 0, font_size)) printNexit ("error >> failed to set pixel sizes");

		// create bitmap font
		// int image_width = (font_size+2)*16;
		// int image_height = (font_size+2)*8;

		// create a buffer for the bitmap
		// unsigned char* buffer = new unsigned char[image_width*image_height*4];
		// memset( buffer , 0 , image_width*image_height*4 );

		// create an array to save the character widths
		// int* widths = new int[128];
		// int* heights = new int[128];
		// int* xs = new int[128];
		// int* ys = new int[128];
		// int* hang = new int[128];

		// we need to find the character that goes below the baseline by the biggest value
		int maxUnderBaseline = 0;
		for ( int i = 32 ; i < 127 ; ++i )
		{
			// get the glyph index from character code
			auto glyphIndex = FT_Get_Char_Index( face , i );

			// load the glyph image into the slot
			if (FT_Load_Glyph( face , glyphIndex , FT_LOAD_DEFAULT )) printNexit ("BitmapFontGenerator > failed to load glyph");
			

			// get the glyph metrics
			const FT_Glyph_Metrics& glyphMetrics = face->glyph->metrics;
			
			// find the character that reaches below the baseline by the biggest value
			int glyphHang = (glyphMetrics.horiBearingY-glyphMetrics.height)/64;
			// std::cout << (char) i << " " << glyphHang << std::endl;
			bitmap.hang [i] = glyphHang;
			if ((glyphMetrics.height/64) + glyphHang > max_height) {
				max_height = (glyphMetrics.height/64) + glyphHang;
			}
			
			if( glyphHang < maxUnderBaseline )
			{
				maxUnderBaseline = glyphHang;
			}
		}

		max_under_baseline = maxUnderBaseline;

		// draw all characters
		for ( int i = 0 ; i < 128 ; ++i )
		{
			// get the glyph index from character code
			auto glyphIndex = FT_Get_Char_Index( face , i );

			// load the glyph image into the slot
			if (FT_Load_Glyph( face , glyphIndex , FT_LOAD_DEFAULT )) printNexit ("BitmapFontGenerator > failed to load glyph, error code: ");

			// convert to an anti-aliased bitmap
			if (FT_Render_Glyph( face->glyph , FT_RENDER_MODE_NORMAL )) printNexit ("BitmapFontGenerator > failed to render glyph, error code: ");
		

			// save the character width
			bitmap.width [i] = face->glyph->metrics.width/64;

			// find the tile position where we have to draw the character
			int x = (i%16)*(font_size+padding);
			int y = (i/16)*(font_size+padding);
			
			x += 1; // 1 pixel padding from the left side of the tile
			y += (font_size+padding) - face->glyph->bitmap_top + maxUnderBaseline - 1;

			bitmap.x0 [i] = x;
			bitmap.y0 [i] = y;

			// draw the character
			const FT_Bitmap& _bitmap = face->glyph->bitmap;
			bitmap.height [i] = _bitmap.rows;
			for ( int xx = 0 ; xx < _bitmap.width ; ++xx )
			{
				for ( int yy = 0 ; yy < _bitmap.rows ; ++yy )
				{
				unsigned char r = _bitmap.buffer[(yy*(_bitmap.width)+xx)];
				bitmap.buffer[(y+yy)*bitmap.image_width*4+(x+xx)*4+0] = r;
				bitmap.buffer[(y+yy)*bitmap.image_width*4+(x+xx)*4+1] = r;
				bitmap.buffer[(y+yy)*bitmap.image_width*4+(x+xx)*4+2] = r;
				bitmap.buffer[(y+yy)*bitmap.image_width*4+(x+xx)*4+3] = 255;
				}
			}
		}

		// bitm.buffer = buffer;
		// bitm.width = image_width;
		// bitm.height = image_height;
		// bitm.widths = widths;
		// bitm.heights = heights;
		// bitm.xs = xs;
		// bitm.ys = ys;
		// bitm.hang = hang;
	}

	// std::cout << max_under_baseline << std::endl;
	// std::cout << max_height << std::endl;

	// return 0;

	// for (auto i = 0; i < 128; ++i) std::cout << bitm.ys [i] << std::endl;

	// return 0;
	// auto ba = bajs <int> {.odd = 5};

	
	// auto font_bitmaps = std::unordered_map <char, FontBitmap2> {};
	
	// {
	// 	auto details = Details <FontBitmap2> {
	// 		.file = "/System/Library/Fonts/Supplemental/Impact.ttf",
	// 		.font_size = 96,
	// 		.padding = 2,
	// 		.rows = 1,
	// 		.columns = 1
	// 	};

	// 	for (auto i = 'a'; i <= 'z'; ++i) {
	// 		details.ascii = (uint32_t) i;
	// 		font_bitmaps [i] = details ();

	// 		details.ascii = (uint32_t) toupper (i);
	// 		font_bitmaps [toupper (i)] = details ();
	// 	}

		
	// }
	
	// auto font_bitmap = font_bitmaps ['s'];

	// return 0;

	

	// return 0;

	glfwInit ();

	// auto const framebuffer_resize_callback = []
	auto const resolution = get_resolution ();

	auto nr_of_chars = 93;

	// return 0;
	// (2 + width) * count = resolution.width;

	auto font_bitmap = Details <FontBitmap> {
		.file = "/System/Library/Fonts/Supplemental/Impact.ttf",
		.font_size = 48,
		.padding = 2,
		.rows = 10,
		.columns = 20,
		.ascii_begin = 33,
		.ascii_end = 33 + 95
	} ();

	// std::cout << font_bitmap.width << " " << font_bitmap.height << std::endl;

	// std::cout << resolution.width << " " << resolution.height << std::endl;
	
	auto window = Details <Window> {
		.extent = {
			// .width = font_bitmap.width,//640,
			// .height = font_bitmap.height},//480},
			.width = resolution.width,
			.height = resolution.height},
			// .width = resolution.width,
			// .height = resolution.height},
		.hints = {
			{GLFW_CLIENT_API, GLFW_NO_API}
			// {GLFW_RESIZABLE, GLFW_FALSE}
		}
	} ();

	glfwSetInputMode (window.handle, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

	// auto f_width = ((float) font_bitmap.width) / ((float) resolution.width);
	// auto f_height = ((float) font_bitmap.height) / ((float) resolution.height);
	// auto f_padding_w = 2.0f / ((float) resolution.width); 
	// auto f_padding_h = 2.0f / ((float) resolution.height); 

	// auto v0 = glm::vec3 {0.0f, 0.0f, 0.0f};
	// auto v1 = glm::vec3 {f_width, 0.0f, 0.0f};
	// auto v2 = glm::vec3 {f_width, f_height, 0.0f};
	// auto v3 = glm::vec3 {f_width, f_height, 0.0f};
	// auto v4 = glm::vec3 {0.0f, f_height, 0.0f};
	// auto v5 = glm::vec3 {0.0f, 0.0f, 0.0f};

	// auto v6 = glm::vec3 {v1.x + 0.0f, 0.0f, 0.0f};
	// auto v7 = glm::vec3 {v1.x + f_width, 0.0f, 0.0f};
	// auto v8 = glm::vec3 {v1.x + f_width, f_height, 0.0f};
	// auto v9 = glm::vec3 {v1.x + f_width, f_height, 0.0f};
	// auto v10 = glm::vec3 {v1.x, f_height, 0.0f};
	// auto v11 = glm::vec3 {v1.x, 0.0f, 0.0f};

	auto text_vertices = std::vector <Vertex> {
		// {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		// {{1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		// {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		// {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		// {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		// {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}

		// {v6, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		// {v7, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		// {v8, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		// {v9, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		// {v10, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		// {v11, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}

		// {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		// {{1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
		// {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		// {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
		// {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
		// {{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}
	};

	

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

	auto physical_device = Details <PhysicalDevice> {
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
			},
			{
				.binding = 0,
				.location = 3,
				.format = VK_FORMAT_R32G32_SFLOAT,
				.offset = offsetof (Vertex, as_texture)
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
			},
			{
				.offset = 0,
				.size = sizeof (texture_push_constants),
				.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
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

	auto texturePushConstants = texture_push_constants {

	};
	



	auto device_local_vertex_buffer_details = Details <Buffer> {
		.device = device,
		.size = sizeof (text_vertices [0]) * 2048,
		.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		.sharing_mode = VK_SHARING_MODE_EXCLUSIVE
	};

	auto device_local_vertex_buffer = device_local_vertex_buffer_details ();

	auto device_local_vertex_buffer_memory_details = Details <DeviceMemory> {
		.physical_device = physical_device,
		.device = device,
		.size = device_local_vertex_buffer.get_memory_requirements ().size,
		.flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	};

	auto device_local_vertex_buffer_memory = device_local_vertex_buffer_memory_details ();

	device_local_vertex_buffer_memory.bind (device_local_vertex_buffer);

	auto host_visible_vertex_buffer_details = Details <Buffer> {
		.device = device,
		.size = sizeof (text_vertices [0]) * 2048,
		.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		.sharing_mode = VK_SHARING_MODE_EXCLUSIVE
	};

	auto host_visible_vertex_buffer = host_visible_vertex_buffer_details ();

	auto host_visible_vertex_buffer_memory_details = Details <DeviceMemory> {
		.physical_device = physical_device,
		.device = device,
		.size = host_visible_vertex_buffer.get_memory_requirements ().size,
		.flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	};

	auto host_visible_vertex_buffer_memory = host_visible_vertex_buffer_memory_details ();

	host_visible_vertex_buffer_memory.bind (host_visible_vertex_buffer);

	auto const dublicate_vertex_buffers = [&] {
		device.wait_idle ();
		host_visible_vertex_buffer.destroy ();
		host_visible_vertex_buffer_memory.destroy ();
		device_local_vertex_buffer.destroy ();
		device_local_vertex_buffer_memory.destroy ();

		device_local_vertex_buffer_details.size *= 2;
		device_local_vertex_buffer = device_local_vertex_buffer_details ();

		device_local_vertex_buffer_memory_details.size *= 2;
		device_local_vertex_buffer_memory = device_local_vertex_buffer_memory_details ();
		device_local_vertex_buffer_memory.bind (device_local_vertex_buffer);

		host_visible_vertex_buffer_details.size *= 2;
		host_visible_vertex_buffer = host_visible_vertex_buffer_details ();

		host_visible_vertex_buffer_memory_details.size *= 2;
		host_visible_vertex_buffer_memory = host_visible_vertex_buffer_memory_details ();
		host_visible_vertex_buffer_memory.bind (host_visible_vertex_buffer);
	};

		// host_visible_vertex_buffer_memory.bind (host_visible_vertex_buffer).paste (text_vertices.data(), host_visible_vertex_buffer.get_memory_requirements ().size);
		// device_local_vertex_buffer.copy_from (host_visible_vertex_buffer);

		// host_visible_vertex_buffer.destroy ();
		// host_visible_vertex_buffer_memory.destroy ();
		
	// std::cout << "start" << std::endl;

	// device_local_vertex_buffer_memory.bind (device_local_vertex_buffer);
	// std::cout << "start" << std::endl;
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
		.tex_width = bitmap.image_width,
		.tex_height = bitmap.image_height,
		.pixels = bitmap.buffer
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


	constexpr auto TEXTURE_ARRAY_SIZE = uint32_t {2};

	auto descriptor_sets = descriptor_pool.allocate ({max_frames_in_flight, graphics_pipeline.descriptor_set_layout});
	{
		auto const sampler_info = VkDescriptorImageInfo {
			.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			.imageView = texture_image.view,
			.sampler = texture_sampler.handle
		};

		auto const sampler_info2 = VkDescriptorImageInfo {
			// .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			// .imageView = texture_image.view,
			.sampler = texture_sampler.handle
		};

		auto descriptor_writes = std::vector <VkWriteDescriptorSet> {
			{
				.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
				.dstBinding = 0,
				.dstArrayElement = 0,
				.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
				.descriptorCount = 1,
				.pImageInfo = &sampler_info,
			},
			// {
			// 	.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			// 	.dstBinding = 1,
			// 	.dstArrayElement = 0,
			// 	.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER,
			// 	.descriptorCount = TEXTURE_ARRAY_SIZE,
			// 	.pImageInfo = &sampler_info,
			// }
		};

		for (auto const & descriptor_set : descriptor_sets){
			descriptor_writes[0].dstSet = descriptor_set;
			vkUpdateDescriptorSets (device.handle, static_cast <uint32_t> (descriptor_writes.size()), descriptor_writes.data(), 0, nullptr);
		}
	}

	auto text = std::string ("");

	

	auto const rendering_tape = [&] (uint32_t image_index) noexcept -> void {

		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float,std::chrono::seconds::period>(currentTime -startTime).count();
		// pushConstants.model = glm::translate (glm::mat4 (1.0f), glm::vec3 (0.0f, 0.0f, 0.0f));//{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};//glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),glm::vec3(0.0f, 0.0f, 1.0f));
		pushConstants.model = glm::scale (pushConstants.model, glm::vec3 (1.0f, 1.0f, 1.0f));
		// pushConstants.view =  {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};//glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f,0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		// pushConstants.proj = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};//glm::perspective(glm::radians(45.0f),swapchain.image_extent.width / (float) swapchain.image_extent.height, 0.1f,10.0f);
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
		// text_vertices.clear();
		// auto v11 = glm::vec3 {0.0f, 0.0f, 0.0f}; 
		// for (auto c : text) {
		// 	int ascii = c - 32;
		// 	std::cout << ascii << std::endl;
			// auto v0 = glm::vec3 {v11.x + 0.0f, 0.0f, 0.0f};
			// auto v1 = glm::vec3 {v11.x + f_width, 0.0f, 0.0f};
			// auto v2 = glm::vec3 {v11.x + f_width, f_height, 0.0f};
			// auto v3 = glm::vec3 {v11.x + f_width, f_height, 0.0f};
			// auto v4 = glm::vec3 {v11.x, f_height, 0.0f};
			// auto v5 = glm::vec3 {v11.x, 0.0f, 0.0f};

			// text_vertices.push_back ({v0, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}});
			// text_vertices.push_back ({v1, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}});
			// text_vertices.push_back ({v2, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}});
			// text_vertices.push_back ({v3, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}});
			// text_vertices.push_back ({v4, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}});
			// text_vertices.push_back ({v5, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}});
		// 	v11.x += v1.x;
		// }
		// host_visible_vertex_buffer_memory.paste (text_vertices.data(), host_visible_vertex_buffer.get_memory_requirements ().size);
		// device_local_vertex_buffer.copy_from (host_visible_vertex_buffer);
		
		vkCmdBindVertexBuffers (draw_command_buffer [current_frame].handle, 0, 1, vertex_buffers, offsets);

		vkCmdPushConstants (draw_command_buffer [current_frame].handle, graphics_pipeline.layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof (mvp_push_constants), &pushConstants);
		
		vkCmdBindDescriptorSets(draw_command_buffer [current_frame].handle, VK_PIPELINE_BIND_POINT_GRAPHICS, graphics_pipeline.layout, 0, 1, &descriptor_sets [current_frame], 0, nullptr);
		vkCmdDraw (draw_command_buffer [current_frame].handle, static_cast<uint32_t> (text_vertices.size()), 1, 0, 0);
		// std::cout << text_vertices.size() << std::endl;
		
		// for (auto i = 0; i < text_vertices.size(); ++i) {
			// vkCmdDraw (draw_command_buffer [current_frame].handle, 6, 1, i * 6, 0);
		// }

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
	

	// static auto vm = font::vm <256> {.text_vertices = text_vertices, .device_local_vertex_buffer = device_local_vertex_buffer, .device_local_vertex_buffer_memory = device_local_vertex_buffer_memory, .host_visible_vertex_buffer = host_visible_vertex_buffer, .host_visible_vertex_buffer_memory = host_visible_vertex_buffer_memory};
		// static auto chunk = font::chunk {};
		

	static auto const xpad = (10/(float)bitmap.image_width);
	static auto const ypad = (10/(float)bitmap.image_height);
	static auto yoffset = -0.9f + ypad;
	static auto xoffset = -0.9f + xpad;
		// static auto old_yoffset = -1.0f;
		// static auto old_xoffset = -1.0f;
	static auto const space_width = bitmap.width ['j'] / (float) bitmap.image_width;
	static auto const enter_height = 1.5f * max_height / (float) bitmap.image_height;

	// auto inputState = Window::Event::State {.type = Window::Event::Type::NADA};

	struct {
		float x = 0.0f; 
		float y = 0.0f;
	} scrollOffset;


	struct {
		Window::Event::State before {.type = Window::Event::Type::NADA};
		Window::Event::State current {.type = Window::Event::Type::NADA};
	} inputState;

	auto inputEventHandler = overload {
		[&](Window::Event::Resize event){
			device.wait_idle ();
			swapchain_details.image_extent = window.get_extent ();
			swapchain_details.recreate (swapchain);
		}, 
		[&](Window::Event::Scroll event){
			pushConstants.model = glm::translate (pushConstants.model, glm::vec3 (event.x/10, event.y/10, 0.0f));//{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};//glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),glm::vec3(0.0f, 0.0f, 1.0f));
			scrollOffset.x += event.x/10;
			scrollOffset.y += event.y/10;
		},
		[&](Window::Event::MouseButton event){
			
		}, 
		[&](Window::Event::Key event){
			std::cout << event.key << std::endl;

			
			if (event.action == GLFW_PRESS or event.action == GLFW_REPEAT) {
				if (event.key == GLFW_KEY_ENTER) {
					// undos.push_back ([](std::vector <Vertex> & v)->undo_return_t{return {v[v.size()-5].pos.x + xpad, yoffset-enter_height};});
					yoffset += enter_height;
					xoffset = -1.0f;
					// state = State::ENTER;
					
				} else if (event.key == GLFW_KEY_SPACE ) {
					
					// undos.push_back ([](std::vector <Vertex> & v)->undo_return_t{
					// 	return {xoffset, yoffset};
					// });
					xoffset += space_width;
					// state = State::SPACE;
					return;

				} else if (event.key == GLFW_KEY_BACKSPACE) {
					// snapshot
					// snapshot = 
					host_visible_vertex_buffer_memory.paste (text_vertices.data(), host_visible_vertex_buffer.get_memory_requirements ().size);
					device_local_vertex_buffer.copy_from (host_visible_vertex_buffer);
					return;

					// snapshot.undo ()
					// snapshot.record ([](Snapshot& prev){prev.undo();})
					// if (state == State::BEGIN) {
					// 	goto END;
					// } else if (state == State::BACK_SPACE) {
						
					// 	undos.pop_back();
					// 	auto [new_xoffset, new_yoffset] = undos.back()(text_vertices);
					// 	undos.pop_back ();

					// 	xoffset = new_xoffset;
					// 	yoffset = new_yoffset;
					// 	host_visible_vertex_buffer_memory.paste (text_vertices.data(), host_visible_vertex_buffer.get_memory_requirements ().size);
					// 	device_local_vertex_buffer.copy_from (host_visible_vertex_buffer);
					// } else if (state == State::SPACE) {
					// 	// xoffset -= space_width;
					// 	undos.back()(text_vertices);
					// 	undos.pop_back();
						
						
					// } else if (state == State::ENTER) {

					// 	auto [new_xoffset, new_yoffset] = undos.back()(text_vertices);
					// 	undos.pop_back();
						
					// 	xoffset = new_xoffset;
					// 	yoffset = new_yoffset;
						
					// } else if (state == State::CHAR) {
					// 	// undos.push_back ([](std::vector <Vertex> & v)->std::tuple <float, float>{
					// 	// 	return {xoffset + space_width, yoffset};
					// 	// });
						
					// 	auto [new_xoffset, new_yoffset] = undos.back()(text_vertices);
					// 	undos.pop_back();
						
					// 	xoffset = new_xoffset;
					// 	yoffset = new_yoffset;


					// 	host_visible_vertex_buffer_memory.paste (text_vertices.data(), host_visible_vertex_buffer.get_memory_requirements ().size);
					// 	device_local_vertex_buffer.copy_from (host_visible_vertex_buffer);
					// 	// state = State::BACK_SPACE;
					// }
					// text_vertices.erase (text_vertices.end() - 6, text_vertices.end());
					// state = State::BACK_SPACE;
					// undos.push_back ([](std::vector <Vertex> & v)->undo_return_t{
					// 	return {xoffset + space_width, yoffset};
					// });
					

				} else if (event.key >= 65 and event.key <= 90) { // bokstav

					if (event.mods & GLFW_MOD_SUPER) {
						if (event.key == 'Z') {
							// auto [new_xoffset, new_yoffset] = undos.back()(text_vertices);
							// undos.pop_back();
							// xoffset = new_xoffset;
							// yoffset = new_yoffset;
							host_visible_vertex_buffer_memory.paste (text_vertices.data(), host_visible_vertex_buffer.get_memory_requirements ().size);
							device_local_vertex_buffer.copy_from (host_visible_vertex_buffer);

						} 

						
						// std::cout << "super" << std::endl;
						goto END;
					}
					auto stor = event.mods & GLFW_MOD_CAPS_LOCK ? (event.mods & GLFW_MOD_SHIFT ? false : true) : (event.mods & GLFW_MOD_SHIFT ? true : false);
		
					if (not stor) {
						event.key += 32;
					}



					
					// state = State::CHAR;
					// yoffset = 0.0f;
				} else if (MACOS and event.key == 45) { // +
					if (event.mods & GLFW_MOD_SUPER) { // zoom in
						pushConstants.model = glm::scale (pushConstants.model, glm::vec3 (1.2f, 1.2f, 1.0f));
						host_visible_vertex_buffer_memory.paste (text_vertices.data(), host_visible_vertex_buffer.get_memory_requirements ().size);
						device_local_vertex_buffer.copy_from (host_visible_vertex_buffer);
						return;
					} else if (event.mods & GLFW_MOD_SHIFT) { // ?
						event.key -= 12;
						// goto PRINT;
					} 
					else {
						event.key -= 2;
						// goto PRINT;
					}
				} else if (MACOS and event.key == 47)/* - */ {
					if (event.mods & GLFW_MOD_SUPER) {
						pushConstants.model = glm::scale (pushConstants.model, glm::vec3 (0.8f, 0.8f, 1.0f));
						host_visible_vertex_buffer_memory.paste (text_vertices.data(), host_visible_vertex_buffer.get_memory_requirements ().size);
						device_local_vertex_buffer.copy_from (host_visible_vertex_buffer);
						return;
					} 
				} 
				else if (event.key == GLFW_KEY_LEFT_SHIFT or event.key == GLFW_KEY_RIGHT_SHIFT) {
					if (event.key == '1') { // !
						event.key = 33;
					} else {
						return;
					}
					// event.key == 33;
				} else if (event.key < '0' or event.key > '9') {
					goto END;
				}
				// chunk.add_constant ();
				// chunk.add_instruction (font::OP_DRAW).add_constant (event.key);

				auto const asTexture = glm::vec2 {1.0f, 0.0f};

				auto yplus = (max_height - bitmap.height [event.key] - bitmap.hang [event.key]) / (float) bitmap.image_height;//(max_height / (float) bitm.height);//((bitm.hang [event.key]) / (float) bitm.height);
					auto color = glm::vec3 {1.0f, 0.0f, 0.0f};
					auto width = bitmap.width [event.key] / (float) bitmap.image_width;
					// width /= 2;
					auto height = bitmap.height [event.key] / (float) bitmap.image_height;
					// height /= 2;
					auto v0 = glm::vec3 {xoffset, yoffset + yplus, 0.0f};
					auto v1 = glm::vec3 {v0.x + width, v0.y, 0.0f};
					auto v2 = glm::vec3 {v1.x, v1.y + height, 0.0f};
					auto v3 = v2;
					auto v4 = glm::vec3 {v0.x, v3.y, 0.0f};
					auto v5 = v0;

					auto f0 = glm::vec2 {bitmap.x0 [event.key] / (float) bitmap.image_width, bitmap.y0 [event.key] / (float) bitmap.image_height};
					auto f1 = glm::vec2 {f0.x + (bitmap.width [event.key] / (float) bitmap.image_width), f0.y};
					auto f2 = glm::vec2 {f1.x, f1.y + (bitmap.height [event.key] / (float) bitmap.image_height)};
					auto f3 = f2;
					auto f4 = glm::vec2 {f0.x, f3.y};
					auto f5 = f0;

				

					
					text_vertices.push_back ({v0, color, f0, asTexture});
					text_vertices.push_back ({v1, color, f1, asTexture});
					text_vertices.push_back ({v2, color, f2, asTexture});
					text_vertices.push_back ({v3, color, f3, asTexture});
					text_vertices.push_back ({v4, color, f4, asTexture});
					text_vertices.push_back ({v5, color, f5, asTexture});

					host_visible_vertex_buffer_memory.paste (text_vertices.data(), host_visible_vertex_buffer.get_memory_requirements ().size);
					device_local_vertex_buffer.copy_from (host_visible_vertex_buffer);
					
					xoffset += width + xpad;
			}

		END:;
		},
		[&](Window::Event::CursorPosition event){
			if (window.get_left_mouse_button () == GLFW_PRESS) {
				auto pos = window.get_cursor_position ();
				auto extent = window.get_size ();
				auto xpos = ((pos.x / (float) extent.width) - 0.5) * 2 - scrollOffset.x;
				auto ypos = ((pos.y / (float) extent.height) - 0.5) * 2 - scrollOffset.y;

				auto const asTexture = glm::vec2 {0.0f, 0.0f};
				
				auto color = glm::vec3 {1.0f, 1.0f, 1.0f};
				auto width = bitmap.width ['a'] / (float) bitmap.image_width;
						// width /= 2;
				auto height = bitmap.height ['a'] / (float) bitmap.image_height;
						// height /= 2;
				auto v0 = glm::vec3 {xpos, ypos, 0.0f};
				auto v1 = glm::vec3 {v0.x + 0.002f, v0.y, 0.0f};
				auto v2 = glm::vec3 {v1.x, v1.y + 0.002f, 0.0f};
				auto v3 = v2;
				auto v4 = glm::vec3 {v0.x, v3.y, 0.0f};
				auto v5 = v0;

				text_vertices.push_back ({.pos = v0, .color = color, .tex_coord = {}, .as_texture = asTexture});
				text_vertices.push_back ({v1, color, {}, asTexture});
				text_vertices.push_back ({v2, color, {}, asTexture});
				text_vertices.push_back ({v3, color, {}, asTexture});
				text_vertices.push_back ({v4, color, {}, asTexture});
				text_vertices.push_back ({v5, color, {}, asTexture});

				if (text_vertices.size() >= (device_local_vertex_buffer.get_memory_requirements().size / sizeof (text_vertices [0]))) {
					dublicate_vertex_buffers ();
				}

				host_visible_vertex_buffer_memory.paste (text_vertices.data(), host_visible_vertex_buffer.get_memory_requirements ().size);
				device_local_vertex_buffer.copy_from (host_visible_vertex_buffer);

				// std::cout << text_vertices.size() << std::endl;
			}
			// if (inputState.current.type == Window::Event::Type::MOUSE_BUTTON and inputState.current.as.mouse_button.button == GLFW_MOUSE_BUTTON_LEFT and inputState.current.as.mouse_button.action == GLFW_PRESS) {

			// 	std::cout << "yaay" << std::endl;
			// }
		}, 
		[&](Window::Event::CursorEnter event){
			std::cout << "scroll" << std::endl;
		}, 
		[&](Window::Event::Drop event){
			std::cout << "scroll" << std::endl;
		}
	};

	using EventHandler = decltype (inputEventHandler) *;


	window.callbacks = {
		.resize = [] (GLFWwindow* window, int width, int height) noexcept -> void {	
			// reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window)) -> operator () (Window::Event::Resize {width, height}, reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window)));
			(*reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window))) (Window::Event::Resize {width, height});
		},
		.scroll = [] (GLFWwindow* window, double xoffset, double yoffset) noexcept -> void {
			// reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window)) -> operator () (Window::Event::Scroll {xoffset, yoffset}, reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window)));
			(*reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window))) (Window::Event::Scroll {xoffset, yoffset});
		},
		.mouse_button = [] (GLFWwindow* window, int button, int action, int mods) noexcept -> void {
			// reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window)) -> operator () (Window::Event::MouseButton {button, action, mods}, reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window)));
			(*reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window))) (Window::Event::MouseButton {button, action, mods});
		},
		.key = [] (GLFWwindow* window, int key, int scancode, int action, int mods) noexcept -> void {
			// reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window)) -> operator () (Window::Event::Key {key, scancode, action, mods}, reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window)));
			(*reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window))) (Window::Event::Key {key, scancode, action, mods});
		},
		.cursor_position = [] (GLFWwindow* window, double xpos, double ypos) noexcept -> void {
			// reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window)) -> operator () (Window::Event::CursorPosition {xpos, ypos}, reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window)));
			(*reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window))) (Window::Event::CursorPosition {xpos, ypos});
		},
		.cursor_enter = [] (GLFWwindow* window, int entered) noexcept -> void {
			// reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window)) -> operator () (Window::Event::CursorEnter {entered}, reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window)));
			(*reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window))) (Window::Event::CursorEnter {entered});
		},
		.drop = [] (GLFWwindow* window, int count, const char** paths) noexcept -> void {
			// reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window)) -> operator () (Window::Event::Drop {paths, count}, reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window)));
			(*reinterpret_cast <EventHandler> (glfwGetWindowUserPointer (window))) (Window::Event::Drop {paths, count});
		}
	};
	// window.set_user_pointer (&event_handler).register_callbacks ();
	window.set_user_pointer (&inputEventHandler).register_callbacks ();
	// window.set_resize_callback (resize_callback);
	// glfwSetWindowUserPointer (window.handle, &event_handler);
	// aa.template operator () <int> ();
	// // main loop


	
	while (not window.should_close ()) {
		glfwPollEvents ();
		// auto image_index = AcquireNextImageIndex {
		// 	.swapchain = swapchain,
		// 	.signal_when_done = {
		// 		.semaphore = image_ready_for_rendering
		// 	}
		// } ();
		
		// if (window_resized) {
		// 	device.wait_idle ();
		// 	swapchain_details.image_extent = window.get_extent ();
		// 	swapchain_details.recreate (swapchain);
		// 	window_resized = false;
		// }
		
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
	host_visible_vertex_buffer.destroy ();
	host_visible_vertex_buffer_memory.destroy ();
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


/*

IDEAS


super + scroll -> jump back when fingers released
*/

