#pragma once

#include "defines.h"
#include "core/asserts.h"
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#define VK_CHECK(expr)                                              \
{                                                                   \
    VkResult result = expr;                                         \
    HM_ASSERT_MSG(expr == VK_SUCCESS, string_VkResult(result));     \
}

typedef struct VulkanDevice {
    VkPhysicalDevice physical_device;
    VkDevice device;
} VulkanDevice;

typedef struct VulkanContext {
    VkInstance instance;
    VkAllocationCallbacks *allocation_callbacks;
    VulkanDevice device;
    VkSurfaceKHR surface;

#if DEBUG == TRUE
    VkDebugUtilsMessengerEXT debug_messenger;
#endif

} VulkanContext;
