module;
#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
// #define GLM_FORCE_RADIANS
// #define GLM_FORCE_DEPTH_ZERO_TO_ONE
// #include <glm/vec2.hpp>
// #include <glm/vec3.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"
// #include <ft2build.h>
// #include FT_FREETYPE_H
// // #include <iostream>
// #include <vector>
// #include <tuple>
// #include <string>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <vector>
export module Graphics;
// import <vector>;

// export import Delta.Graphics.Font;
// import Delta.Range;
// import std;
// import <iostream>;
// import <vector>;
// import <tuple>;
// import <string>;
// import <fstream>;
export import Graphics.Window;

// using std::experimental::coroutine_handle;
// using std::experimental::suspend_always;

export auto printNexit (char const* txt) noexcept -> void {
	// std::cout << txt << std::endl;
	printf (txt);
	printf ("\n");
	exit (-1);
}

// export auto get_resolution () noexcept -> VkExtent2D {
// 	auto const * mode = glfwGetVideoMode (glfwGetPrimaryMonitor ());
// 	return {.width = (uint32_t) mode -> width, .height = (uint32_t) mode -> height};
// }



// export auto get_surface_presentation_modes (VkPhysicalDevice const & device, VkSurfaceKHR const & surface) noexcept -> std::vector <VkPresentModeKHR> {
	
// 	auto count = uint32_t {};
	
// 	vkGetPhysicalDeviceSurfacePresentModesKHR (device, surface, &count, nullptr);
// 	auto modes = std::vector <VkPresentModeKHR> {};
// 	if (count != 0) {
// 		modes.resize (count);
// 		vkGetPhysicalDeviceSurfacePresentModesKHR (device, surface, &count, modes.data());
// 	} 
// 	return modes;
// }

// export auto get_surface_formats (VkPhysicalDevice const & device, VkSurfaceKHR const & surface) noexcept -> std::vector <VkSurfaceFormatKHR> {
	
// 	auto count = uint32_t {};
	
// 	vkGetPhysicalDeviceSurfaceFormatsKHR (device, surface, &count, nullptr);

// 	if (count != 0) {
// 		auto formats = std::vector <VkSurfaceFormatKHR> {count};
// 		vkGetPhysicalDeviceSurfaceFormatsKHR (device, surface, &count, formats.data ());
// 		return formats;
// 	} else {
// 		return {};
// 	}
// }

// // export auto make_render_pass_with_framebuffers (VkExtent2D const & extent, VkFormat const & format, VkDevice const & device) noexcept -> std::tuple <>





// /*
// 	get layers available to instances
// */
// export auto get_instance_layer_properties (
// 	) noexcept -> std::vector <VkLayerProperties> {
// 	auto count = uint32_t {0};
// 	vkEnumerateInstanceLayerProperties (&count, nullptr);
// 	auto layer_properties = std::vector <VkLayerProperties> {count};
// 	vkEnumerateInstanceLayerProperties (&count, layer_properties.data());
// 	return layer_properties;
// }

// /*
// 	Get layer properties available to a physical device
// */
// export auto get_layer_properties (
// 	VkPhysicalDevice const& device) noexcept -> std::vector <VkLayerProperties> {
// 	auto count = uint32_t {0};
// 	vkEnumerateDeviceLayerProperties (device, &count, nullptr);
// 	auto layer_properties = std::vector <VkLayerProperties> {count};
// 	vkEnumerateDeviceLayerProperties (device, &count, layer_properties.data());
// 	return layer_properties;
// }

// /*
// 	Get available extensions to instances
// */
// export auto get_instance_extension_properties (
// 	) noexcept -> std::vector <VkExtensionProperties> {
// 	auto count = uint32_t {0};
// 	vkEnumerateInstanceExtensionProperties (nullptr, &count, nullptr);
// 	auto extensions_properties = std::vector <VkExtensionProperties> {count};
// 	vkEnumerateInstanceExtensionProperties (nullptr, &count, extensions_properties.data());
// 	return extensions_properties;
// }

// /*
// 	get extension properties available to a physical device
// */
// export auto get_extension_properties (
// 	VkPhysicalDevice const& device) noexcept -> std::vector <VkExtensionProperties> {
// 	auto count = uint32_t {0};
// 	vkEnumerateDeviceExtensionProperties (device, nullptr, &count, nullptr);
// 	auto extension_properties = std::vector <VkExtensionProperties> {count};
// 	vkEnumerateDeviceExtensionProperties (device, nullptr, &count, extension_properties.data());
// 	return extension_properties;
// }

// using swapchain_details_t = std::tuple <VkSurfaceCapabilitiesKHR, std::vector <VkSurfaceFormatKHR>, std::vector <VkPresentModeKHR>>;




// export auto get_swapchain_details (
// 	VkPhysicalDevice const & device,
// 	VkSurfaceKHR const & surface) noexcept -> swapchain_details_t {
		
// 		auto details = swapchain_details_t {};

// 		auto & [capabilities, formats, present_modes] = details;

// 		vkGetPhysicalDeviceSurfaceCapabilitiesKHR (device, surface, &capabilities);


// 		auto count = uint32_t {};


// 		vkGetPhysicalDeviceSurfaceFormatsKHR (device, surface, &count, nullptr);

// 		if (count != 0) {
// 			formats.resize (count);
// 			vkGetPhysicalDeviceSurfaceFormatsKHR (device, surface, &count, formats.data ());
// 		}


// 		vkGetPhysicalDeviceSurfacePresentModesKHR (device, surface, &count, nullptr);

// 		if (count != 0) {
// 			present_modes.resize (count);
// 			vkGetPhysicalDeviceSurfacePresentModesKHR (device, surface, &count, present_modes.data ());
// 		}


// 		return details;
// }

// /*
// 	Takes a vector of layer properties and makes a vector of layer names out of it
// */
// export auto make_names (
// 	std::vector <VkLayerProperties> const& layers_properties) noexcept -> std::vector <char const*> {
// 	auto layer_names = std::vector <char const *> {layers_properties.size()};
// 	for (auto i = 0; i < layer_names.size(); ++i) {
// 		layer_names [i] = layers_properties [i].layerName;
// 	}
// 	return layer_names;
// }

// /*
// 	Takes a vector of extension properties and makes a vector of extension names out of it
// */
// export auto make_names (
// 	std::vector <VkExtensionProperties> const& extension_properties) noexcept -> std::vector <char const*> {
// 	auto extension_names = std::vector <char const *> {extension_properties.size()};
// 	for (auto i = 0; i < extension_names.size(); ++i) {
// 		extension_names [i] = extension_properties [i].extensionName;
// 	}
// 	return extension_names;
// }


// export auto make_names (auto const& arg, auto (*fun) (decltype (arg))) 
// {
// 	return make_names (fun (arg));
// };

// export auto make_names (auto const& arg, auto (*fun) (decltype (arg)), auto... rest) //-> decltype ()
// {
// 	return std::make_tuple (make_names (arg, fun), make_names (arg, rest)...);
// };

// export auto make_app_info (
// 	char const * app_name = "app", 
// 	uint32_t app_version = VK_MAKE_VERSION (1, 0, 0),
// 	char const* engine_name = "engine", 
// 	uint32_t engine_version = VK_MAKE_VERSION (1, 0, 0), 
// 	uint32_t api_version = VK_MAKE_VERSION (1, 3, 0)) noexcept -> VkApplicationInfo {

// 	return {
// 		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
// 		.pNext = nullptr,
// 		.pApplicationName = app_name,
// 		.applicationVersion = app_version,
// 		.pEngineName = engine_name,
// 		.engineVersion = engine_version,
// 		.apiVersion = api_version
// 	};
// }






// export struct Window {
// 	GLFWwindow * handle;

// 	struct {
// 		void (*position) (GLFWwindow* window, int x, int y) noexcept = nullptr;
// 		void (*resize) (GLFWwindow* window, int width, int height) noexcept = nullptr;
// 		void (*scroll) (GLFWwindow* window, double xoffset, double yoffset) noexcept = nullptr;
// 		void (*mouse_button) (GLFWwindow* window, int button, int action, int mods) noexcept = nullptr;
// 		void (*key) (GLFWwindow* window, int key, int scancode, int action, int mods) noexcept = nullptr;
// 		void (*cursor_position) (GLFWwindow* window, double xpos, double ypos) noexcept = nullptr;
// 		void (*cursor_enter) (GLFWwindow* window, int entered) noexcept = nullptr;
// 		void (*drop) (GLFWwindow* window, int count, const char** paths) noexcept = nullptr;
// 	} callbacks;

// 	struct Event {
// 		struct Position {int x, y;};
// 		struct Resize {int width, height;};
// 		struct Scroll {double x, y;};
// 		struct MouseButton {int button, action, mods;};
// 		struct Key {int key, scancode, action, mods;};
// 		struct CursorPosition {double x, y;};
// 		struct CursorEnter {int entered;};
// 		struct Drop {char const ** paths; int count;};

// 		enum struct Type {
// 			RESIZE,
// 			SCROLL,
// 			MOUSE_BUTTON,
// 			KEY,
// 			CURSOR_POSITION,
// 			CURSOR_ENTER,
// 			DROP,
// 			NADA
// 		};

// 		struct State {
// 			union {
// 				Resize resize;
// 				Scroll scroll;
// 				MouseButton mouse_button;
// 				Key key;
// 				CursorPosition cursor_position;
// 				CursorEnter cursor_enter;
// 				Drop drop;
// 			} as;

// 			Type type;
// 		};
// 	};

	

	
// 	auto register_callbacks () noexcept -> Window & {
// 		if (callbacks.position != nullptr) {
// 			glfwSetWindowPosCallback (handle, callbacks.position);
// 		}
// 		if (callbacks.resize != nullptr) {
// 			glfwSetFramebufferSizeCallback (handle, callbacks.resize);
// 		}
// 		if (callbacks.scroll != nullptr) {
// 			glfwSetScrollCallback (handle, callbacks.scroll);
// 		}
// 		if (callbacks.mouse_button != nullptr) {
// 			glfwSetMouseButtonCallback (handle, callbacks.mouse_button);
// 		}
// 		if (callbacks.key != nullptr) {
// 			glfwSetKeyCallback (handle, callbacks.key);
// 		}
// 		if (callbacks.cursor_position != nullptr) {
// 			glfwSetCursorPosCallback (handle, callbacks.cursor_position);
// 		}
// 		if (callbacks.cursor_enter != nullptr) {
// 			glfwSetCursorEnterCallback (handle, callbacks.cursor_enter);
// 		}
// 		if (callbacks.drop != nullptr) {
// 			glfwSetDropCallback (handle, callbacks.drop);
// 		}

// 		return *this;
// 	}

// 	auto get_cursor_position () const noexcept -> VkOffset2D {
// 		double x, y;
// 		glfwGetCursorPos (handle, &x, &y);

// 		return {
// 			.x = static_cast <int32_t> (x),
// 			.y = static_cast <int32_t> (y)
// 		};
// 	}

// 	auto get_size () const noexcept -> VkExtent2D {
// 		int width, height;
// 		glfwGetWindowSize(handle, &width, &height);

// 		return {
// 			.width = static_cast <uint32_t> (width),
// 			.height = static_cast <uint32_t> (height)
// 		};
// 	}

// 	auto get_position () const noexcept -> VkOffset2D {
// 		int x, y;
// 		glfwGetWindowPos (handle, &x, &y);

// 		return {
// 			.x = static_cast <int32_t> (x),
// 			.y = static_cast <int32_t> (y)
// 		};
// 	}

// 	auto get_left_mouse_button () const noexcept -> int {
// 		return glfwGetMouseButton (handle, GLFW_MOUSE_BUTTON_LEFT);
// 	}
	
	// auto get_extent () const noexcept -> VkExtent2D {
	// 	int width, height;
	// 	glfwGetFramebufferSize(handle, &width, &height);

	// 	return {
	// 		.width = static_cast <uint32_t> (width),
	// 		.height = static_cast <uint32_t> (height)
	// 	};
	// }
// 	auto should_close () const noexcept -> bool {
// 		return glfwWindowShouldClose (handle);
// 	}
// 	auto disable_cursor () const noexcept -> void {
// 		glfwSetInputMode (handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
// 	}
// 	auto hide_cursor () const noexcept -> void {
// 		glfwSetInputMode (handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
// 	}
// 	auto enable_cursor () const noexcept -> void {
// 		glfwSetInputMode (handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
// 	}
// 	auto destroy () noexcept -> void {
// 		glfwDestroyWindow (handle);
// 	}

// 	auto set_user_pointer (auto * p) noexcept -> Window & {
// 		glfwSetWindowUserPointer (handle, p);
// 		return *this;
// 	}

// 	auto set_resize_callback (void (*callback) (GLFWwindow* window, int width, int height)) noexcept -> Window & {
// 		glfwSetFramebufferSizeCallback (handle, callback);
// 		return *this;
// 	}
	
// 	auto set_scroll_callback (void (*callback) (GLFWwindow* window, double xoffset, double yoffset)) noexcept -> Window & {
// 		glfwSetScrollCallback (handle, callback);
// 		return *this;
// 	}

// 	auto set_mouse_button_callback (void (*callback) (GLFWwindow* window, int button, int action, int mods)) noexcept -> Window & {
// 		glfwSetMouseButtonCallback (handle, callback);
// 		return *this;
// 	}

// 	auto set_key_callback (void (*callback) (GLFWwindow* window, int key, int scancode, int action, int mods)) noexcept -> Window & {
// 		glfwSetKeyCallback (handle, callback);
// 		return *this;
// 	}

// 	auto set_cursor_position_callback (void (*callback) (GLFWwindow* window, double xpos, double ypos)) noexcept -> Window & {
// 		glfwSetCursorPosCallback (handle, callback);
// 		return *this;
// 	}

// 	auto set_cursor_enter_callback (void (*callback) (GLFWwindow* window, int entered)) noexcept -> Window & {
// 		glfwSetCursorEnterCallback (handle, callback);
// 		return *this;
// 	}

// 	auto set_drop_callback (void (*callback) (GLFWwindow* window, int count, const char** paths)) noexcept -> Window & {
// 		glfwSetDropCallback (handle, callback);
// 		return *this;
// 	}

	


// export template <>
// struct Details <Window> {
// 	VkExtent2D const & extent;
// 	char const * title = "";
// 	std::vector <std::tuple <int, int>> hints;

// 	auto operator () () noexcept -> Window {
// 		for (auto const [type, value] : hints) {
// 			glfwWindowHint (type, value);
// 		}
	
// 		return {.handle = glfwCreateWindow (extent.width, extent.height, title, nullptr, nullptr)};
// 	}
// };







// export struct FontBitmap {
// 	uint32_t width;
// 	uint32_t height;
// 	unsigned char * buffer;
// 	int * widths;
// 	int * heights;
// 	int * xs;
// 	int * ys;
// 	int * hang;

// 	auto destroy () noexcept -> void {
// 		delete [] buffer;
// 	}
// };




// export namespace font {

// enum opcode : uint_fast8_t {
// 	OP_DRAW,
// 	OP_SKIP,
// 	OP_DELETE,
// 	OP_UNDO
// };

// struct value {
// 	union {
// 		char character;


// 	} as;
// 	enum {
// 		VAL_CHAR
// 	} type;
// };

// struct chunk {
// 	std::vector <uint_fast8_t> code;
// 	std::vector <value> values;

// 	auto add_instruction (uint_fast8_t opcode) noexcept -> auto & {
// 		code.push_back (opcode);
// 		return *this;
// 	}
// 	auto add_constant (value v) noexcept -> uint_fast8_t {
// 		values.push_back (v);
// 		return values.size()-1;
// 	}
// };


// template <int max_stack>
// struct vm {
// 	std::vector <Vertex> & vertices;
// 	Buffer & device_local_vertex_buffer;
// 	DeviceMemory & device_local_vertex_buffer_memory;
// 	Buffer & host_visible_vertex_buffer;
// 	DeviceMemory & host_visible_vertex_buffer_memory;
// 	font::value stack [max_stack];
// 	font::value * top = stack;

// 	template <uint32_t width, uint32_t height>
// 	auto interpret (font::chunk& chunk, FontBitmap3 <width, height> & bitmap) noexcept -> void {
// 		auto * ip = chunk.code.data();

// 		for (;;) {
// 			auto instruction = *ip++;
// 			switch (instruction) {
// 				case OP_DRAW: {
// 					auto constant = pop ();
// 					break;
// 				}
				
// 				default: {
// 					break;
// 				}
// 			}
// 		}
// 	}

// 	auto push (font::value v) noexcept -> void {
// 		*top = v;
// 		++top;
// 	}

// 	auto pop () noexcept -> font::value {
// 		--top;
// 		return *top;
// 	}
// };

// }




// export template <>
// struct Details <FontBitmap> {
// 	char const * file;
// 	unsigned int font_size;
// 	unsigned int padding;
// 	unsigned int columns;
// 	unsigned int rows;
// 	unsigned int ascii_begin;
// 	unsigned int ascii_end;

// 	auto operator () () const noexcept -> FontBitmap {
// 		// auto const columns = 25;
// 		// auto const rows = 10;
// 		// auto const padding = 2;
		
// 		// auto const font_size = int {48};
		
// 		auto lib = FT_Library {};

// 		if (FT_Init_FreeType (&lib)) printNexit ("error >> failed to initialize freetype lib");
		
// 		auto face = FT_Face {};
// 		// "/Impact.ttf"
// 		if (FT_New_Face (lib, (std::string {FONTS_DIR} + "/Charter.ttc").c_str (), 0, &face)) printNexit ("error >> failed to load font file");

// 		if (FT_Set_Pixel_Sizes (face, 0, font_size)) printNexit ("error >> failed to set pixel sizes");

// 		uint32_t image_width = (font_size + padding) * columns; 
// 		uint32_t image_height = (font_size + padding) * rows;
// 		auto * buffer = new unsigned char [image_width * image_height * 4];
// 		auto * widths = new int [ascii_end];

// 		auto max_under_baseline = int {0};

// 		auto glyph_index = FT_UInt {};

// 		for (auto i = ascii_begin; i < ascii_end - 1; ++i) {
// 			glyph_index = FT_Get_Char_Index (face, i);  
// 			if (FT_Load_Glyph (face, glyph_index, FT_LOAD_DEFAULT)) printNexit ("error >> failed to load glyph");
// 			auto const & glyph_metrics = face->glyph->metrics;
// 			auto glyph_hang = (glyph_metrics.horiBearingY - glyph_metrics.height) / 64;
// 			if (glyph_hang < max_under_baseline) max_under_baseline = glyph_hang;
// 		}



// 		for (auto i = ascii_begin + 1; i < ascii_end - 1; ++i) {
// 			glyph_index = FT_Get_Char_Index (face, i); 
// 			if (FT_Load_Glyph (face, glyph_index, FT_LOAD_DEFAULT)) printNexit ("error >> failed to load glyph");
// 			if (FT_Render_Glyph (face->glyph, FT_RENDER_MODE_NORMAL)) printNexit ("error >> failed to render glyph");
// 			widths[i] = face->glyph->metrics.width/64;
// 			// std::cout << (char) i << std::endl;
// 			auto x = ((i - ascii_begin - 1) % columns) * (font_size + padding);
// 			auto y = ((i - ascii_begin - 1) / columns) * (font_size + padding);
// 			x += 1;
// 			y += (font_size + padding) - face->glyph->bitmap_top + max_under_baseline - 1;
// 			// std::cout << x << " : " << y << " : " << face->glyph->metrics.width/64 << std::endl;
// 			auto const & bitmap = face -> glyph -> bitmap;
// 			// font_bitmap.buffer = bitmap.buffer;
// 			// font_bitmap.width = bitmap.width;
// 			// font_bitmap.height = bitmap.rows;
// 			// break;
// 			// std::cout << (y) * image_width * 4 - (x) * 4 << std::endl;
// 			// std::cout << (y + bitmap.rows) * image_width * 4 - (x + bitmap.width) * 4 << std::endl;
// 			// auto xstart = x;
// 			// auto ystart = y;
// 			// std::cout << x << " : " << y << std::endl;
// 			for (auto xx = 0; xx < bitmap.width; ++xx) {
// 				for (auto yy = 0; yy < bitmap.rows; ++yy) {
// 					auto r = bitmap.buffer [(yy * (bitmap.width) + xx)];
// 					buffer [(y + yy) * image_width * 4 - (x + xx) * 4 + 0] = r;
// 					buffer [(y + yy) * image_width * 4 - (x + xx) * 4 + 1] = r;
// 					buffer [(y + yy) * image_width * 4 - (x + xx) * 4 + 2] = r;
// 					buffer [(y + yy) * image_width * 4 - (x + xx) * 4 + 3] = 255;
// 				}
// 			}

// 			// if (i == 50) break;
// 		}
		
// 		delete [] widths;

// 		if (FT_Done_FreeType (lib)) printNexit ("error >> failed to shut down freetype library");

// 		return {.width = image_width, .height = image_height, .buffer = buffer};
// 	}
// };

// export struct FontBitmap2 {
// 	uint32_t width;
// 	uint32_t height;
// 	unsigned char * buffer;

// 	auto destroy () noexcept -> void {
// 		delete [] buffer;
// 	}
// };

// export template <>
// struct Details <FontBitmap2> {
// 	char const * file;
// 	unsigned int font_size;
// 	unsigned int padding;
// 	unsigned int columns;
// 	unsigned int rows;
// 	unsigned int ascii;

// 	auto operator () () const noexcept -> FontBitmap2 {
// 		// auto const columns = 25;
// 		// auto const rows = 10;
// 		// auto const padding = 2;
		
// 		// auto const font_size = int {48};
		
// 		auto lib = FT_Library {};

// 		if (FT_Init_FreeType (&lib)) printNexit ("error >> failed to initialize freetype lib");
		
// 		auto face = FT_Face {};
// 		// "/Impact.ttf"
// 		if (FT_New_Face (lib, (std::string {FONTS_DIR} + "/Charter.ttc").c_str (), 0, &face)) printNexit ("error >> failed to load font file");

// 		if (FT_Set_Pixel_Sizes (face, 0, font_size)) printNexit ("error >> failed to set pixel sizes");

// 		uint32_t image_width = (font_size + padding) * columns; 
// 		uint32_t image_height = (font_size + padding) * rows;
// 		auto * buffer = new unsigned char [image_width * image_height * 4];
// 		// auto * widths = new int [ascii_end];

// 		auto max_under_baseline = int {0};

// 		auto glyph_index = FT_UInt {};

// 		// for (auto i = ascii_begin; i < ascii_end - 1; ++i) {
// 		// 	glyph_index = FT_Get_Char_Index (face, i);  
// 		// 	if (FT_Load_Glyph (face, glyph_index, FT_LOAD_DEFAULT)) printNexit ("error >> failed to load glyph");
// 		// 	auto const & glyph_metrics = face->glyph->metrics;
// 		// 	auto glyph_hang = (glyph_metrics.horiBearingY - glyph_metrics.height) / 64;
// 		// 	if (glyph_hang < max_under_baseline) max_under_baseline = glyph_hang;
// 		// }



// 		glyph_index = FT_Get_Char_Index (face, ascii); 
// 		if (FT_Load_Glyph (face, glyph_index, FT_LOAD_DEFAULT)) printNexit ("error >> failed to load glyph");
// 		if (FT_Render_Glyph (face->glyph, FT_RENDER_MODE_NORMAL)) printNexit ("error >> failed to render glyph");
// 		// widths[i] = face->glyph->metrics.width/64;

// 		auto x = /*((i - ascii - 1) % columns) * */(font_size + padding);
// 		auto y = /*((i - ascii - 1) / columns) * */(font_size + padding);
// 		x += 1;
// 		y += (font_size + padding) - face->glyph->bitmap_top /*+ max_under_baseline*/ - 1;
// 		x = 1;
// 		y = 1;
// 		auto const & bitmap = face -> glyph -> bitmap;
// 			// font_bitmap.buffer = bitmap.buffer;
// 			// font_bitmap.width = bitmap.width;
// 			// font_bitmap.height = bitmap.rows;
// 			// break;

		
// 		for (auto xx = 0; xx < bitmap.width; ++xx) {
// 			for (auto yy = 0; yy < bitmap.rows; ++yy) {
// 				auto r = bitmap.buffer [(yy * (bitmap.width) + xx)];
// 				buffer [(y + yy) * image_width * 4 - (x + xx) * 4 + 0] = r;
// 				buffer [(y + yy) * image_width * 4 - (x + xx) * 4 + 1] = r;
// 				buffer [(y + yy) * image_width * 4 - (x + xx) * 4 + 2] = r;
// 				buffer [(y + yy) * image_width * 4 - (x + xx) * 4 + 3] = 255;
// 				// std::cout << 
// 			}
// 		}

// 			// if (i == 50) break;
		
		
// 		//delete [] widths;

// 		if (FT_Done_FreeType (lib)) printNexit ("error >> failed to shut down freetype library");

// 		return {.width = image_width, .height = image_height, .buffer = buffer};
// 	}
// };


// export struct DrawEvent {
// 	struct UpdatedText {};
// };


// struct State {

// };

export template <typename T>
concept Graphics = requires (T t) {
	t.draw_frame ();
	t.process ();
};

export struct graphics_t {
	auto windows_open () const noexcept -> int {
		auto count = 0;
		return windows.size ();
		// printf ("count: %i\n", count);
		// for (auto& w : windows) {
		// 	// printf ("loop\n");
		// 	if (w.open ()) {
		// 		// printf ("open!\n");	
		// 		++count;
		// 	} else {
		// 		// printf ("closed!\n");
		// 	}
		// }
		// printf ("count: %i\n", count);
		return count;
	}

	auto make_window () noexcept -> window & {
		windows.push_back ({&windows, instance, physical_device, 640, 480, ""});
		return windows.back ();
	}
	auto draw_frame () noexcept -> void {
		for (auto & w : windows) {
			w.draw_frame ();
		}
	}
	auto process () noexcept -> void {
		glfwPollEvents ();
	}
	graphics_t () noexcept : windows {0} {
		constexpr auto width = 640;
		constexpr auto height = 480;
		glfwInit ();
		// glfwWindowHint (GLFW_VISIBLE, GLFW_FALSE);
		glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
	#if defined (MACOS)
		glfwWindowHint (GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_NO_API);
	#endif

		VkApplicationInfo app_info {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = nullptr,
			.pApplicationName = "",
			.applicationVersion = VK_MAKE_VERSION (1, 0, 0),
			.pEngineName = "",
			.engineVersion = VK_MAKE_VERSION (1, 0, 0),
			.apiVersion = VK_MAKE_VERSION (1, 3, 0)
		};
		auto layers = std::vector <char const*> {"VK_LAYER_KHRONOS_validation"/*, "VK_LAYER_LUNARG_api_dump"*/};
		auto extensions = std::vector <char const*> {"VK_KHR_portability_enumeration", "VK_KHR_surface", "VK_EXT_metal_surface", "VK_MVK_macos_surface", "VK_KHR_get_physical_device_properties2"};

		auto const createInfo = VkInstanceCreateInfo {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = nullptr,
			 .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
			.pApplicationInfo = &app_info,
			.enabledLayerCount = static_cast <uint32_t> (layers.size()),
			.ppEnabledLayerNames = layers.data(),
			.enabledExtensionCount = static_cast <uint32_t> (extensions.size()),
			.ppEnabledExtensionNames = extensions.data()
		};

		if (vkCreateInstance (/*VkInstanceCreateInfo const * pCreateInfo */&createInfo, /*VkAllocationCallbacks const * pAllocator */nullptr, /*VkInstance* pInstance*/&instance) != VK_SUCCESS) {
			printf ("error >> failed to create instance");
			exit (-1);
		}

		physical_device = Details <PhysicalDevice> {
			.instance = instance,
			.picker = [] (std::vector <VkPhysicalDevice> const & devices) -> VkPhysicalDevice {
				return devices.back ();
			}
		} ();

		int max_under_baseline = 0;
		int max_height = 0;
		constexpr auto font_size = 128;
		constexpr auto padding = 2;
		// auto bitmap = FontBitmap3 <(font_size+padding)*16, (font_size+padding)*8> {};

		// {
			

		// 	auto lib = FT_Library {};

		// 	if (FT_Init_FreeType (&lib)) printNexit ("error >> failed to initialize freetype lib");
			
		// 	auto face = FT_Face {};
		// 	// "/Impact.ttf"
		// 	if (FT_New_Face (lib, (std::string {FONTS_DIR} + "/Charter.ttc").c_str (), 0, &face)) printNexit ("error >> failed to load font file");

		// 	if (FT_Set_Pixel_Sizes (face, 0, font_size)) printNexit ("error >> failed to set pixel sizes");

		// 	// create bitmap font
		// 	// int image_width = (font_size+2)*16;
		// 	// int image_height = (font_size+2)*8;

		// 	// create a buffer for the bitmap
		// 	// unsigned char* buffer = new unsigned char[image_width*image_height*4];
		// 	// memset( buffer , 0 , image_width*image_height*4 );

		// 	// create an array to save the character widths
		// 	// int* widths = new int[128];
		// 	// int* heights = new int[128];
		// 	// int* xs = new int[128];
		// 	// int* ys = new int[128];
		// 	// int* hang = new int[128];

		// 	// we need to find the character that goes below the baseline by the biggest value
		// 	int maxUnderBaseline = 0;
		// 	for ( int i = 32 ; i < 127 ; ++i )
		// 	{
		// 		// get the glyph index from character code
		// 		auto glyphIndex = FT_Get_Char_Index( face , i );

		// 		// load the glyph image into the slot
		// 		if (FT_Load_Glyph( face , glyphIndex , FT_LOAD_DEFAULT )) printNexit ("BitmapFontGenerator > failed to load glyph");
				

		// 		// get the glyph metrics
		// 		const FT_Glyph_Metrics& glyphMetrics = face->glyph->metrics;
				
		// 		// find the character that reaches below the baseline by the biggest value
		// 		int glyphHang = (glyphMetrics.horiBearingY-glyphMetrics.height)/64;
		// 		// std::cout << (char) i << " " << glyphHang << std::endl;
		// 		bitmap.hang [i] = glyphHang;
		// 		if ((glyphMetrics.height/64) + glyphHang > max_height) {
		// 			max_height = (glyphMetrics.height/64) + glyphHang;
		// 		}
				
		// 		if( glyphHang < maxUnderBaseline )
		// 		{
		// 			maxUnderBaseline = glyphHang;
		// 		}
		// 	}

		// 	max_under_baseline = maxUnderBaseline;

		// 	// draw all characters
		// 	for ( int i = 0 ; i < 128 ; ++i )
		// 	{
		// 		// get the glyph index from character code
		// 		auto glyphIndex = FT_Get_Char_Index( face , i );

		// 		// load the glyph image into the slot
		// 		if (FT_Load_Glyph( face , glyphIndex , FT_LOAD_DEFAULT )) printNexit ("BitmapFontGenerator > failed to load glyph, error code: ");

		// 		// convert to an anti-aliased bitmap
		// 		if (FT_Render_Glyph( face->glyph , FT_RENDER_MODE_NORMAL )) printNexit ("BitmapFontGenerator > failed to render glyph, error code: ");
			

		// 		// save the character width
		// 		bitmap.width [i] = face->glyph->metrics.width/64;

		// 		// find the tile position where we have to draw the character
		// 		int x = (i%16)*(font_size+padding);
		// 		int y = (i/16)*(font_size+padding);
				
		// 		x += 1; // 1 pixel padding from the left side of the tile
		// 		y += (font_size+padding) - face->glyph->bitmap_top + maxUnderBaseline - 1;

		// 		bitmap.x0 [i] = x;
		// 		bitmap.y0 [i] = y;

		// 		// draw the character
		// 		const FT_Bitmap& _bitmap = face->glyph->bitmap;
		// 		bitmap.height [i] = _bitmap.rows;
		// 		for ( int xx = 0 ; xx < _bitmap.width ; ++xx )
		// 		{
		// 			for ( int yy = 0 ; yy < _bitmap.rows ; ++yy )
		// 			{
		// 			unsigned char r = _bitmap.buffer[(yy*(_bitmap.width)+xx)];
		// 			bitmap.buffer[(y+yy)*bitmap.image_width*4+(x+xx)*4+0] = r;
		// 			bitmap.buffer[(y+yy)*bitmap.image_width*4+(x+xx)*4+1] = r;
		// 			bitmap.buffer[(y+yy)*bitmap.image_width*4+(x+xx)*4+2] = r;
		// 			bitmap.buffer[(y+yy)*bitmap.image_width*4+(x+xx)*4+3] = 255;
		// 			}
		// 		}
		// 	}

		// 	// bitm.buffer = buffer;
		// 	// bitm.width = image_width;
		// 	// bitm.height = image_height;
		// 	// bitm.widths = widths;
		// 	// bitm.heights = heights;
		// 	// bitm.xs = xs;
		// 	// bitm.ys = ys;
		// 	// bitm.hang = hang;
		// }
	}
	~graphics_t () {
		glfwTerminate ();
	}

private:
	std::vector <window> windows;
	VkInstance instance;
	PhysicalDevice physical_device;
};	