#include "core/event.h"
#include "core/logger.h"
#include "containers/darray.h"

// TODO need a multi-threaded implementation
// TODO event priorities (high – processed immediately, low – handled async)

/**
 * This describes a specific listener with it's callback.
 */
typedef struct registered_event {
    void *listener;
    PFN_on_event callback;
} RegisteredEvent;

/**
 * Stores the array of listeners for an event type.
 */
typedef struct event_code_entry {
    RegisteredEvent *events;
} EventCodeEntry;

#define MAX_EVENT_CODES 16384

/**
 * The state stores a Darray for each event type, and those arrays contain the listeners to that event.
 * The listeners darray can be NULL, meaning there are no listeners.
 */
typedef struct event_system_state {
    EventCodeEntry registered[MAX_EVENT_CODES];
} EventSystemState;

static b8 initialized = FALSE;
static EventSystemState state = {0};

b8 event_init() {
    if (initialized) {
        return FALSE;
    }

    initialized = TRUE;
    return TRUE;
}

void event_shutdown() {
    for (u16 i = 0; i < MAX_EVENT_CODES; i++) {
        if (state.registered[i].events != 0) {
            darray_destroy(state.registered[i].events);
            state.registered[i].events = 0;
        }
    }
}

b8 event_subscribe(EventCode code, void *listener, PFN_on_event on_event) {
    if (!initialized) {
        return FALSE;
    }

    if (state.registered[code].events == 0) {
        state.registered[code].events = darray_create(RegisteredEvent);
    }

    u64 registered_count = darray_length(state.registered[code].events);

    // Check for duplicates
    for (u64 i = 0; i < registered_count; ++i) {
        if (state.registered[code].events[i].listener == listener) {
            HM_WARN("Duplicate event registrations not allowed!");
            return FALSE;
        }
    }

    RegisteredEvent event = {.listener = listener, .callback = on_event};
    darray_push(state.registered[code].events, event);

    return TRUE;
}

b8 event_unsubscribe(EventCode code, void *listener, PFN_on_event on_event) {
    if (!initialized) {
        return FALSE;
    }

    if (state.registered[code].events == 0) {
        return FALSE;
    }

    u64 registered_count = darray_length(state.registered[code].events);
    for (u64 i = 0; i < registered_count; ++i) {
        RegisteredEvent event = state.registered[code].events[i];
        if (event.listener == listener && event.callback == on_event) {
            RegisteredEvent popped_event;
            darray_pop_at(state.registered[code].events, i, &popped_event);
            return TRUE;
        }
    }

    // Not found
    return FALSE;
}

b8 event_fire(EventCode code, void *sender, EventContext context) {
    if (!initialized) {
        return FALSE;
    }

    if (state.registered[code].events == 0) {
        return FALSE;
    }

    u64 registered_count = darray_length(state.registered[code].events);
    for (u64 i = 0; i < registered_count; ++i) {
        RegisteredEvent event = state.registered[code].events[i];
        if (event.callback(code, sender, event.listener, context)) {
            return TRUE;
        }
    }

    // Not found
    return FALSE;
}