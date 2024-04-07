#include "renderer/renderer_backend.h"
#include "core/memory.h"
#include "renderer/vulkan/vulkan_backend.h"

b8 renderer_backend_create(RendererBackendType type, PlatformState *plat_state, RendererBackend *out_backend) {
    out_backend->platform_state = plat_state;

    if (type == RENDERER_BACKEND_TYPE_VULKAN) {
        out_backend->initialize = vulkan_backend_initialize;
        out_backend->shutdown = vulkan_backend_shutdown;
        out_backend->begin_frame = vulkan_backend_begin_frame;
        out_backend->end_frame = vulkan_backend_end_frame;
        out_backend->resized = vulkan_backend_resized;
        return TRUE;
    }

    return FALSE;
}

void renderer_backend_destroy(RendererBackend *backend) {
    hm_zero_memory(backend, sizeof(RendererBackend));
}