#pragma once

#include "defines.h"
#include <vulkan/vulkan.h>

typedef struct VulkanContext {
    VkInstance instance;
    VkAllocationCallbacks *allocation_callbacks;
} VulkanContext;
