#include "renderer/renderer_frontend.h"
#include "renderer/renderer_backend.h"
#include "core/logger.h"
#include "core/memory.h"

static RendererBackend *backend = 0;

b8 renderer_init(const char *app_name, PlatformState *plat_state) {
    backend = hm_alloc(sizeof(RendererBackend), MEMORY_TAG_RENDERER);
    backend->frame_number = 0;

    // TODO make the backend type configurable
    if (!renderer_backend_create(RENDERER_BACKEND_TYPE_VULKAN, plat_state, backend)) {
        HM_FATAL("Unable to create the renderer backend!");
        return FALSE;
    }

    if (!backend->initialize(backend, app_name, plat_state)) {
        HM_FATAL("Unable to initialize the renderer backend!");
        return FALSE;
    }

    return TRUE;
}

void renderer_shutdown() {
    if (backend == 0) {
        return;
    }

    backend->shutdown(backend);
    hm_free(backend, sizeof(RendererBackend), MEMORY_TAG_RENDERER);
}

b8 renderer_start_frame(f32 delta_time) {
    return backend->begin_frame(backend, delta_time);
}

b8 renderer_end_frame(f32 delta_time) {
    b8 result = backend->end_frame(backend, delta_time);
    backend->frame_number++;
    return result;
}

b8 renderer_draw_frame(RenderPacket *packet) {
    if (renderer_start_frame(packet->delta_time)) {
        if (!renderer_end_frame(packet->delta_time)) {
            HM_ERROR("Unable to render the frame!");
            return FALSE;
        }
    }

    return TRUE;
}

void renderer_on_resized(u16 width, u16 height) {

}
