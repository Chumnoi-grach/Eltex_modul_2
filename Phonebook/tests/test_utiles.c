#include <check.h>
#include "../src/Utiles/utiles.h"

START_TEST(test_has_duplicates_no_duplicates) {
    ck_assert_int_eq(has_duplicates(""), 0);
    ck_assert_int_eq(has_duplicates("1"), 0);
    ck_assert_int_eq(has_duplicates("123"), 0);
    ck_assert_int_eq(has_duplicates("123456789"), 0);
    ck_assert_int_eq(has_duplicates("abc"), 0);
    ck_assert_int_eq(has_duplicates(NULL), 0);
}
END_TEST

START_TEST(test_has_duplicates_with_duplicates) {
    ck_assert_int_eq(has_duplicates("11"), 1);
    ck_assert_int_eq(has_duplicates("112"), 1);
    ck_assert_int_eq(has_duplicates("1231"), 1);
    ck_assert_int_eq(has_duplicates("1234567891"), 1);
    ck_assert_int_eq(has_duplicates("aa"), 1);
    ck_assert_int_eq(has_duplicates("aba"), 1);
}
END_TEST

// Создание набора тестов
Suite* utiles_suite(void) {
    Suite *s = suite_create("Utiles");
    TCase *tc_core = tcase_create("Core");
    
    tcase_add_test(tc_core, test_has_duplicates_no_duplicates);
    tcase_add_test(tc_core, test_has_duplicates_with_duplicates);
    
    suite_add_tcase(s, tc_core);
    return s;
}