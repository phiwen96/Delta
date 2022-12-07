module;
#include <iostream>
#include <vulkan/vulkan_core.h>
export module Vulkan;

// export import Vulkan.Instance;
// export import Vulkan.PhysicalDevice;
// export import Vulkan.Window;
export import Vulkan.App;

export auto operator << (std::ostream& os, VkExtensionProperties const& p) noexcept -> std::ostream& {
	os << p.extensionName;
	return os;
}

// export import Vulkan.Instance;
// import Vulkan.Instance.Impl;