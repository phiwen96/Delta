module;
#include <GLFW/glfw3.h>
#include <algorithm>
module Window:Implementation; 

// import :Interface;

// window::window () noexcept : window {640, 480, ""} {

// }

// // window::window (int width, int height, char const* title) noexcept : handle {glfwCreateWindow (width, height, title, nullptr, nullptr)} {
// // 	glfwSetMouseButtonCallback (handle, mouse_button_callback);
// // }

// window::window (window&& other) noexcept : handle {nullptr}{
// 	std::swap (handle, other.handle);
// }

// window::~window () {
// 	glfwDestroyWindow (handle);
// }