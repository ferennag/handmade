#pragma once

#include "defines.h"

typedef struct platform_state PlatformState;
typedef struct VulkanContext VulkanContext;

void platform_get_required_extension_names(const char ***out_extensions);

b8 platform_create_vulkan_surface(PlatformState *platform_state, VulkanContext *vulkan_context);