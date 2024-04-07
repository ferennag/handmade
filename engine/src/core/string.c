#include "core/string.h"
#include <string.h>

b8 strings_equal(const char *str1, const char *str2) {
    return strcmp(str1, str2) == 0;
}