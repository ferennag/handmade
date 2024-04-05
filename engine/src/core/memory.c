#include <stdio.h>
#include <string.h>

#include "core/memory.h"
#include "core/logger.h"
#include "platform/platform.h"

struct memory_stats {
    u64 total_allocated;
    u64 tagged_allocations[MEMORY_TAG_MAX_TAGS];
};

static struct memory_stats stats = {0};

static const char *memory_tag_strings[] = {
        "UNKNOWN",
        "ARRAY",
        "DARRAY",
        "DICT",
        "RING_QUEUE",
        "BST",
        "STRING",
        "APPLICATION",
        "JOB",
        "TEXTURE",
        "MATERIAL_INSTANCE",
        "RENDERER",
        "GAME",
        "TRANSFORM",
        "ENTITY",
        "ENTITY_NODE",
        "SCENE",
};

b8 init_memory() {
    HM_INFO("Initializing memory subsystem...");
    return TRUE;
}

void shutdown_memory() {
    HM_INFO("Shutting down memory subsystem...");
}

void *hm_alloc(u64 size, MemoryTag tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        HM_WARN("hm_alloc called with MEMORY_TAG_UNKNOWN. Make sure to classify all allocations!");
    }

    stats.total_allocated += size;
    stats.tagged_allocations[tag] += size;

    // TODO memory alignment
    return platform_allocate(size, FALSE);
}

void hm_free(void *block, u64 size, MemoryTag tag) {
    if (tag == MEMORY_TAG_UNKNOWN) {
        HM_WARN("hm_alloc called with MEMORY_TAG_UNKNOWN. Make sure to classify all allocations!");
    }

    stats.total_allocated -= size;
    stats.tagged_allocations[tag] -= size;

    // TODO memory alignment
    return platform_free(block, FALSE);
}

void *hm_zero_memory(void *block, u64 size) {
    return platform_zero_memory(block, size);
}

void *hm_copy_memory(void *dest, const void *src, u64 size) {
    return platform_copy_memory(dest, src, size);
}

void *hm_set_memory(void *dest, i32 value, u64 size) {
    return platform_set_memory(dest, value, size);
}

char *get_memory_usage_str() {
    const u64 gib = 1024 * 1024 * 1024;
    const u64 mib = 1024 * 1024;
    const u64 kib = 1024;

    // TODO dynamic string allocation needed
    char buffer[8000] = "System memory use (tagged):\n";
    u64 offset = strlen(buffer);

    for (u32 i = 0; i < MEMORY_TAG_MAX_TAGS; ++i) {
        u64 allocated = stats.tagged_allocations[i];

        char unit[4] = "XiB";
        float amount = 1.0f;

        if (allocated >= gib) {
            unit[0] = 'G';
            amount = allocated / (float) gib;
        } else if (allocated >= mib) {
            unit[0] = 'M';
            amount = allocated / (float) mib;
        } else if (allocated >= kib) {
            unit[0] = 'K';
            amount = allocated / (float) kib;
        } else {
            unit[0] = 'B';
            unit[1] = 0;
            amount = (float) allocated;
        }

        offset += snprintf(buffer + offset, 8000, "  %s: %.2f%s\n", memory_tag_strings[i], amount, unit);
    }

    return strdup(buffer);
}