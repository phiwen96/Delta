export module Standard;

export import <vector>;
export import <array>;
export import <iostream>;
export import <tuple>;
export import <utility>;
export import <coroutine>;
export import <type_traits>;
export import <string>;
export import <algorithm>;

export import <vulkan/vulkan_core.h>;
export import <GLFW/glfw3.h>;


export constexpr auto vk_make_api_version (auto variant, auto major, auto minor, auto patch) {
	return ((((uint32_t)(variant)) << 29) | (((uint32_t)(major)) << 22) | (((uint32_t)(minor)) << 12) | ((uint32_t)(patch)));
}
    