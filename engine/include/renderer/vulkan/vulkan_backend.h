#pragma once

#include "renderer/renderer_backend.h"

b8 vulkan_backend_initialize(RendererBackend *renderer_backend, const char *app_name, PlatformState *plat_state);

void vulkan_backend_shutdown(RendererBackend *renderer_backend);

void vulkan_backend_resized(RendererBackend *renderer_backend, u16 width, u16 height);

b8 vulkan_backend_begin_frame(RendererBackend *renderer_backend, f32 delta_time);

b8 vulkan_backend_end_frame(RendererBackend *renderer_backend, f32 delta_time);
