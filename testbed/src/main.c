#include <core/logger.h>
#include <core/asserts.h>

int main() {
    HM_TRACE("Test message %d", 123);
    HM_DEBUG("Test message %d", 123);
    HM_INFO("Test message %d", 123);
    HM_WARN("Test message %d", 123);
    HM_ERROR("Test message %d", 123);
    HM_FATAL("Test message %d", 123);

    HM_ASSERT(1 == 0);

    return 0;
}
