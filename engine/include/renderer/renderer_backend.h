#pragma once

#include "renderer_types.h"

typedef struct platform_state PlatformState;

b8 renderer_backend_create(RendererBackendType type, PlatformState *plat_state, RendererBackend *out_backend);

void renderer_backend_destroy(RendererBackend *backend);