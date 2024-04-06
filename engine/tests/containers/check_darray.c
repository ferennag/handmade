#include <containers/darray.h>
#include <check.h>

START_TEST(test_darray_create) {
    u64 *array = darray_create(u64);
    ck_assert(array != 0);
    ck_assert_int_eq(0, darray_length(array));
    ck_assert_int_eq(1, darray_capacity(array));
    darray_destroy(array);
}
END_TEST

START_TEST(test_darray_reserve) {
    u64 *array = darray_reserve(u64, 10);
    ck_assert(array != 0);
    ck_assert_int_eq(0, darray_length(array));
    ck_assert_int_eq(10, darray_capacity(array));
    darray_destroy(array);
}
END_TEST

START_TEST(test_darray_push) {
    u64 *array = darray_create(u64);
    darray_push(array, (u64)42);
    darray_push(array, (u64)32);

    ck_assert_int_eq(2, darray_length(array));
    ck_assert_int_eq(2, darray_capacity(array));
    ck_assert_int_eq(42, array[0]);
    ck_assert_int_eq(32, array[1]);
    darray_destroy(array);
}
END_TEST

START_TEST(test_darray_pop) {
    u64 *array = darray_create(u64);
    darray_push(array, (u64)42);
    darray_push(array, (u64)32);
    darray_push(array, (u64)22);

    ck_assert_int_eq(3, darray_length(array));
    ck_assert_int_eq(4, darray_capacity(array));

    u64 popped;
    darray_pop(array, &popped);

    ck_assert_int_eq(2, darray_length(array));
    ck_assert_int_eq(4, darray_capacity(array));
    ck_assert_int_eq(22, popped);

    darray_destroy(array);
}
END_TEST

START_TEST(test_darray_insert_at_first) {
    u64 *array = darray_create(u64);
    darray_push(array, (u64)42);
    darray_push(array, (u64)32);
    darray_insert_at(array, 0, (u64)22);

    ck_assert_int_eq(3, darray_length(array));
    ck_assert_int_eq(4, darray_capacity(array));

    ck_assert_int_eq(22, array[0]);
    ck_assert_int_eq(42, array[1]);
    ck_assert_int_eq(32, array[2]);

    darray_destroy(array);
}
END_TEST

START_TEST(test_darray_insert_at_middle) {
    u64 *array = darray_create(u64);
    darray_push(array, (u64)42);
    darray_push(array, (u64)32);
    darray_insert_at(array, 1, (u64)22);

    ck_assert_int_eq(3, darray_length(array));
    ck_assert_int_eq(4, darray_capacity(array));

    ck_assert_int_eq(42, array[0]);
    ck_assert_int_eq(22, array[1]);
    ck_assert_int_eq(32, array[2]);

    darray_destroy(array);
}
END_TEST

START_TEST(test_darray_insert_at_end) {
    u64 *array = darray_create(u64);
    darray_push(array, (u64)42);
    darray_push(array, (u64)32);
    darray_insert_at(array, 2, (u64)22);

    ck_assert_int_eq(3, darray_length(array));
    ck_assert_int_eq(4, darray_capacity(array));

    ck_assert_int_eq(42, array[0]);
    ck_assert_int_eq(32, array[1]);
    ck_assert_int_eq(22, array[2]);

    darray_destroy(array);
}
END_TEST

START_TEST(test_darray_insert_wrong_index) {
    u64 *array = darray_create(u64);
    darray_push(array, (u64)42);
    darray_push(array, (u64)32);
    darray_insert_at(array, 4, (u64)22);

    ck_assert_int_eq(2, darray_length(array));
    ck_assert_int_eq(2, darray_capacity(array));

    ck_assert_int_eq(42, array[0]);
    ck_assert_int_eq(32, array[1]);

    darray_destroy(array);
}
END_TEST

START_TEST(test_darray_pop_at) {
    u64 *array = darray_create(u64);
    darray_push(array, (u64)42);
    darray_push(array, (u64)32);
    darray_push(array, (u64)22);

    u64 value;
    darray_pop_at(array, 1, &value);

    ck_assert_int_eq(42, array[0]);
    ck_assert_int_eq(22, array[1]);
    ck_assert_int_eq(2, darray_length(array));
    ck_assert_int_eq(value, 32);

    darray_pop_at(array, 0, &value);
    ck_assert_int_eq(22, array[0]);
    ck_assert_int_eq(1, darray_length(array));
    ck_assert_int_eq(value, 42);

    darray_pop_at(array, 0, &value);
    ck_assert_int_eq(0, darray_length(array));
    ck_assert_int_eq(value, 22);

    darray_destroy(array);
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
    tcase_add_test(tc_core, test_darray_reserve);
    tcase_add_test(tc_core, test_darray_push);
    tcase_add_test(tc_core, test_darray_pop);
    tcase_add_test(tc_core, test_darray_insert_at_first);
    tcase_add_test(tc_core, test_darray_insert_at_middle);
    tcase_add_test(tc_core, test_darray_insert_at_end);
    tcase_add_test(tc_core, test_darray_insert_wrong_index);
    tcase_add_test(tc_core, test_darray_pop_at);

    suite_add_tcase(s, tc_core);
    return s;
}