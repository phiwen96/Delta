module;
#include <stdio.h>
#include <vulkan/vulkan_core.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <thread>
module App:Type.Implementation;

import :Type.Interface;

app::app (auto&& title) noexcept {
	constexpr auto width = 640;
	constexpr auto height = 480;
	glfwInit ();
	glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
#if defined (MACOS) 
	glfwWindowHint (GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_NO_API);
#endif
	// auto * window = glfwCreateWindow (width, height, "", nullptr, nullptr);
}

auto app::run () noexcept -> bool {
	
	// auto * window2 = glfwCreateWindow (width, height, "", nullptr, nullptr);
	auto keepRunning = true;
	// std::thread {[&] {
	while (keepRunning) {
		glfwPollEvents ();
	}
		// while ((not glfwWindowShouldClose (window)) or (not glfwWindowShouldClose (window2))) {
			// glfwPollEvents ();
			/* code */
		// }
	// }}.join ();
	
	return true;
}

auto app::new_window () noexcept -> window {
	return {};
}

// static_assert (App <app>, "App <app> == false");