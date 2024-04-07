#pragma once

#include "defines.h"
#include "core/asserts.h"
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#define VK_CHECK(expr)                                        \
{                                                             \
    VkResult result = expr;                                   \
    HM_ASSERT_MSG(expr == VK_SUCCESS, string_VkResult(result));   \
}

typedef struct VulkanContext {
    VkInstance instance;
    VkAllocationCallbacks *allocation_callbacks;
} VulkanContext;
