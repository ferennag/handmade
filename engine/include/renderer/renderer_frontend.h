#pragma once

#include "renderer_types.h"

struct static_mesh_data;
typedef struct platform_state PlatformState;

b8 renderer_init(const char *app_name, PlatformState *plat_state);

void renderer_shutdown();

void renderer_on_resized(u16 width, u16 height);

b8 renderer_draw_frame(RenderPacket *packet);
