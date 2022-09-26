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
module App:Implementation;

import :Interface;

app::app (auto&& title) noexcept {
	
}
auto app::run () noexcept -> bool {
	constexpr auto width = 640;
	constexpr auto height = 480;
	glfwInit ();
	glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
	auto * window = glfwCreateWindow (width, height, "", nullptr, nullptr);
	while (not glfwWindowShouldClose (window)) {
		glfwPollEvents ();
		/* code */
	}
	return true;
}

static_assert (App <app>, "App <app> == false");