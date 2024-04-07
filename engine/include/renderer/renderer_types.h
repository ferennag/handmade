#pragma once

#include "defines.h"

typedef struct platform_state PlatformState;

typedef enum RendererBackendType {
    RENDERER_BACKEND_TYPE_VULKAN,
    RENDERER_BACKEND_TYPE_OPENGL,
    RENDERER_BACKEND_TYPE_DIRECTX,
} RendererBackendType;

typedef struct RendererBackend {
    PlatformState *platform_state;
    u64 frame_number;

    b8 (*initialize)(struct RendererBackend *renderer_backend, const char *app_name, PlatformState *plat_state);

    void (*shutdown)(struct RendererBackend *renderer_backend);

    void (*resized)(struct RendererBackend *renderer_backend, u16 width, u16 height);

    b8 (*begin_frame)(struct RendererBackend *renderer_backend, f32 delta_time);

    b8 (*end_frame)(struct RendererBackend *renderer_backend, f32 delta_time);
} RendererBackend;

typedef struct RenderPacket {
    f32 delta_time;
} RenderPacket;