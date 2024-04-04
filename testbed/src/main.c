#include <core/logger.h>

int main() {
    HM_TRACE("Test message %d", 123);
    HM_DEBUG("Test message %d", 123);
    HM_INFO("Test message %d", 123);
    HM_WARN("Test message %d", 123);
    HM_ERROR("Test message %d", 123);
    HM_FATAL("Test message %d", 123);
    return 0;
}
