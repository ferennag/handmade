#include "renderer/vulkan/vulkan_backend.h"
#include "renderer/vulkan/vulkan_types.h"
#include "core/logger.h"
#include "core/string.h"
#include "containers/darray.h"
#include "renderer/vulkan/vulkan_platform.h"

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

    const char **extensions = darray_create(const char *);
    darray_push(extensions, &VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME); // Required for mac
    darray_push(extensions, &VK_KHR_SURFACE_EXTENSION_NAME); // Generic surface extension
    platform_get_required_extension_names(&extensions);

#if DEBUG == TRUE
    darray_push(extensions, &VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

    HM_DEBUG("Required Vulkan Extensions: ");
    for(u32 i = 0; i < darray_length(extensions); ++i) {
        HM_DEBUG("  - %s", extensions[i]);
    }
#endif

    const char **layers = darray_create(const char *);

#if DEBUG == TRUE
    HM_INFO("Vulkan validation layers enabled. Enumerating...");

    darray_push(layers, &"VK_LAYER_KHRONOS_validation");

    u32 available_layer_count = 0;
    VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, 0));
    VkLayerProperties *available_layers = darray_reserve(VkLayerProperties, available_layer_count);
    VK_CHECK(vkEnumerateInstanceLayerProperties(&available_layer_count, available_layers));

    for (u32 i = 0; i < darray_length(layers); ++i) {
        HM_INFO("Searching for layer: %s", layers[i]);
        b8 found = FALSE;
        for (u32 j = 0; j < available_layer_count; ++j) {
            if(strings_equal(layers[i], available_layers[j].layerName)) {
                found = TRUE;
                HM_INFO("Found.");
                break;
            }
        }

        if (!found) {
            HM_FATAL("Required Vulkan validation layer is missing: %s", layers[i]);
            return FALSE;
        }
    }

    HM_INFO("All required Vulkan validation layers are present.");
#endif

    VkInstanceCreateInfo create_info = {VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    create_info.pApplicationInfo = &app_info;
    create_info.enabledExtensionCount = darray_length(extensions);
    create_info.ppEnabledExtensionNames = extensions;
    create_info.enabledLayerCount = darray_length(layers);
    create_info.ppEnabledLayerNames = layers;
    create_info.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    VK_CHECK(vkCreateInstance(&create_info, context.allocation_callbacks, &context.instance));
    darray_destroy(extensions);
    darray_destroy(layers);

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