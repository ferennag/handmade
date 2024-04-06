#include <core/memory.h>
#include <check.h>

void checked_setup() {
    init_memory();
}

void checked_teardown() {
    shutdown_memory();
}

START_TEST(test_memory_init) {
    ck_assert_uint_eq(TRUE, init_memory());
}
END_TEST

START_TEST(test_memory_alloc) {
    u64 *block = (u64 *) hm_alloc(sizeof(u64), MEMORY_TAG_GAME);
    ck_assert_ptr_ne(0, block);
    hm_free(block, sizeof(u64), MEMORY_TAG_GAME);
}
END_TEST

START_TEST(test_memory_zero) {
    u64 *block = (u64 *) hm_alloc(sizeof(u64), MEMORY_TAG_GAME);

    *block = 10;
    ck_assert_int_eq(10, *block);
    hm_zero_memory(block, sizeof(u64));
    ck_assert_int_eq(0, *block);

    hm_free(block, sizeof(u64), MEMORY_TAG_GAME);
}
END_TEST

START_TEST(test_memory_copy) {
    u64 *block = (u64 *) hm_alloc(sizeof(u64), MEMORY_TAG_GAME);
    u64 *block2 = (u64 *) hm_alloc(sizeof(u64), MEMORY_TAG_GAME);
    hm_zero_memory(block2, sizeof(u64));

    *block = 10;
    hm_copy_memory(block2, block, sizeof(u64));
    ck_assert_int_eq(10, *block);
    ck_assert_int_eq(10, *block2);

    hm_free(block, sizeof(u64), MEMORY_TAG_GAME);
    hm_free(block2, sizeof(u64), MEMORY_TAG_GAME);
}
END_TEST

START_TEST(test_memory_usage_str) {
    ck_assert_int_ne(0, strlen(get_memory_usage_str()));
}
END_TEST

Suite *memory_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Memory");

    /* Core test case */
    tc_core = tcase_create("Core");
    tcase_add_checked_fixture(tc_core, checked_setup, checked_teardown);
    tcase_add_test(tc_core, test_memory_init);
    tcase_add_test(tc_core, test_memory_alloc);
    tcase_add_test(tc_core, test_memory_zero);
    tcase_add_test(tc_core, test_memory_copy);
    tcase_add_test(tc_core, test_memory_usage_str);

    suite_add_tcase(s, tc_core);
    return s;
}