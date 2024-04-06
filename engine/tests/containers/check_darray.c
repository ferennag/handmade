#include <containers/darray.h>
#include <check.h>

START_TEST(test_darray_create) {
    u64 *array = darray_create(u64);
    ck_assert(array != 0);
    ck_assert_int_eq(0, darray_length(array));
    ck_assert_int_eq(1, darray_capacity(array));
}
END_TEST

START_TEST(test_darray_push) {
    u64 *array = darray_create(u64);
    u64 value = 42;
    darray_push(array, &value);

    ck_assert_int_eq(1, darray_length(array));
    ck_assert_int_eq(1, darray_capacity(array));
//    ck_assert_int_eq(42, array[0]);
}
END_TEST

Suite *darray_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Darray");

    /* Core test case */
    tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_darray_create);
    tcase_add_test(tc_core, test_darray_push);

    suite_add_tcase(s, tc_core);
    return s;
}