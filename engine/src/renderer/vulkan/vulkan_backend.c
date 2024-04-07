#include "renderer/vulkan/vulkan_backend.h"
#include "renderer/vulkan/vulkan_types.h"
#include "core/logger.h"
#include <vulkan/vk_enum_string_helper.h>

static VulkanContext context = {0};

b8 vulkan_backend_initialize(RendererBackend *renderer_backend, const char *app_name, PlatformState *plat_state) {
    // TODO custom allocator
    context.allocation_callbacks = 0;

    VkApplicationInfo app_info = {VK_STRUCTURE_TYPE_APPLICATION_INFO};
    app_info.apiVersion = VK_API_VERSION_1_2;
    app_info.pApplicationName = app_name;
    app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    app_info.pEngineName = "Handmade Engine";
    app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);

    const char *extensions[] = {VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME};

    VkInstanceCreateInfo create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = sizeof(extensions) / sizeof(const char *);
    create_info.ppEnabledExtensionNames = extensions;
    create_info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    create_info.ppEnabledLayerNames = 0;
    create_info.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&create_info, context.allocation_callbacks, &context.instance);
    if (result != VK_SUCCESS) {
        HM_ERROR("VkCreateInstance failed with: %s", string_VkResult(result));
        return FALSE;
    }

    HM_INFO("Vulkan Renderer initialized successfully.");

    return TRUE;
}

void vulkan_backend_shutdown(RendererBackend *renderer_backend) {
}

void vulkan_backend_resized(RendererBackend *renderer_backend, u16 width, u16 height) {
}

b8 vulkan_backend_begin_frame(RendererBackend *renderer_backend, f32 delta_time) {
    return TRUE;
}

b8 vulkan_backend_end_frame(RendererBackend *renderer_backend, f32 delta_time) {
    return TRUE;
}