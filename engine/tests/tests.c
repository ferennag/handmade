#include <check.h>
#include "check_check.h"

int main(void) {
    SRunner *sr = srunner_create(darray_suite());

    // To add more suites
//    srunner_add_suite(sr, suite);
//    srunner_add_suite(sr, suite);
//    srunner_add_suite(sr, suite);

    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return failed;
}