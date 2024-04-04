#pragma once

#include "defines.h"

#define HM_ASSERTIONS_ENABLED TRUE

#if HM_ASSERTIONS_ENABLED == TRUE
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

void report_assertion_failure(const char *expression, const char *message, const char *file, i32 line);

#define HM_ASSERT(expr)                                         \
{                                                               \
    if (expr) {                                                 \
    } else {                                                    \
        report_assertion_failure(#expr, "", __FILE__, __LINE__); \
        debugBreak();                                            \
    }                                                           \
};

#define HM_ASSERT_MSG(expr, message)                                 \
{                                                                    \
    if (expr) {                                                      \
    } else {                                                         \
        report_assertion_failure(#expr, message, __FILE__, __LINE__); \
        debugBreak();                                                 \
    }                                                                \
};

#ifdef _DEBUG
#define HM_ASSERT_DEBUG(expr)                                   \
{                                                               \
    if (expr) {                                                 \
    } else {                                                    \
        report_assertion_failure(#expr, "", __FILE__, __LINE__); \
        debugBreak();                                            \
    }                                                           \
};
#else
#define HM_ASSERT_DEBUG(expr)
#endif

#else
// Enabled == false

#define HM_ASSERT(expr)
#define HM_ASSERT_MSG(expr, message)
#define HM_ASSERT_DEBUG(expr)

#endif