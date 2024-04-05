#pragma once

#include "defines.h"

typedef u16 EventCode;

typedef struct event_context {
    // 128 bytes
    union {
        i64 i64[2];
        u64 u64[2];
        f64 f64[2];

        i32 i32[4];
        u32 u32[4];
        f32 f32[4];

        i16 i16[8];
        u16 u16[8];

        i8 i8[16];
        u8 u8[16];

        char c[16];
    } data;
} EventContext;

/**
 * Listeners must implement this method to be able to listen to events. This callback will be called for events.
 */
typedef b8(*PFN_on_event)(EventCode code, void *sender, void *listener_inst, EventContext context);

b8 event_init();
void event_shutdown();

b8 event_subscribe(EventCode code, void *listener, PFN_on_event on_event);
b8 event_unsubscribe(EventCode code, void *listener, PFN_on_event on_event);

/**
 * Fires an event, which will be passed to all listeners.
 *
 * @param code type of event
 * @param sender the sender of the event
 * @param context event context: data passed to listeners from the sender
 * @return TRUE if the event was handled by any listener, FALSE if not. (not an error case)
 */
b8 event_fire(EventCode code, void *sender, EventContext context);

typedef enum system_event_code {
    EVENT_CODE_APP_QUIT = 0x01,
    EVENT_CODE_KEY_PRESSED = 0x02,
    EVENT_CODE_KEY_RELEASED = 0x03,
    EVENT_CODE_BUTTON_PRESSED = 0x04,
    EVENT_CODE_BUTTON_RELEASED = 0x05,
    EVENT_CODE_MOUSE_MOVED = 0x06,
    EVENT_CODE_MOUSE_WHEEL = 0x07,
    EVENT_CODE_RESIZED = 0x08,

    MAX_EVENT_CODE = 0xFF,
} SystemEventCode;