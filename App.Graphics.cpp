#include <iostream>
// #define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <cstdlib>
#include <vector>

import Delta;

auto main(int argc, char **argv) -> int
{
	GLFWwindow *window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	VkApplicationInfo appInfo{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Hello Triangle",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "No Engine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_0
	};

	VkInstanceCreateInfo createInfo {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo
	};

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	createInfo.enabledExtensionCount = glfwExtensionCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;
	createInfo.enabledLayerCount = 0;
	VkInstance instance;

	// vkCreateInstance(&createInfo, nullptr, &instance);

	auto res = vkCreateInstance(&createInfo, nullptr, &instance);
	switch (res)
	{
	case VK_SUCCESS:
		/* code */
		break;

	case VK_ERROR_EXTENSION_NOT_PRESENT:
		std::cout << "error >> extensions not present" << std::endl;
		return -1;

	case VK_ERROR_LAYER_NOT_PRESENT:
		std::cout << "error >> layers not present" << std::endl;
		return -1;


	case VK_ERROR_INCOMPATIBLE_DRIVER:
		std::cout << "error >> incompatible driver" << std::endl;
		return -1;
	
	default:
		std::cout << "error" << std::endl;
		return -1;
	}
	// if () throw std::runtime_error("failed to create instance!");
	
	// retrieve a list of supported extensions before creating an instance
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> extensions(extensionCount);

	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

	std::cout << "available extensions:\n";

	for (const auto& extension : extensions) {
		std::cout << '\t' << extension.extensionName << '\n';
	}
	
	/* Make the window's context current */
	// glfwMakeContextCurrent(window);

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

	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);

	glfwTerminate();
	std::cout << "hi" << std::endl;
	return 0;
}