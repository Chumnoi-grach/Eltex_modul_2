#include <check.h>
#include "../src/Data/validator.h"

START_TEST(test_validate_name_valid) {
    ck_assert_int_eq(validate_name("Иван"), 1);
    ck_assert_int_eq(validate_name("Петр"), 1);
    ck_assert_int_eq(validate_name("Ivan"), 1);
    ck_assert_int_eq(validate_name("John Doe"), 1);
    ck_assert_int_eq(validate_name("Анна-Мария"), 1);
    ck_assert_int_eq(validate_name("Анна_Мария"), 1);
    ck_assert_int_eq(validate_name("Иван Петров"), 1);
    ck_assert_int_eq(validate_name("Петр Иванович"), 1);
}
END_TEST

START_TEST(test_validate_name_invalid) {
    ck_assert_int_eq(validate_name(""), 0);
    ck_assert_int_eq(validate_name("123"), 0);
    ck_assert_int_eq(validate_name("Иван123"), 0);
    ck_assert_int_eq(validate_name("!@#$"), 0);
    ck_assert_int_eq(validate_name("Иван!"), 0);
    ck_assert_int_eq(validate_name("John@Doe"), 0);
    ck_assert_int_eq(validate_name(NULL), 0);
}
END_TEST

START_TEST(test_validate_phone_valid) {
    ck_assert_int_eq(validate_phone(""), 1);
    ck_assert_int_eq(validate_phone("+7-999-123-45-67"), 1);
    ck_assert_int_eq(validate_phone("8-999-123-45-67"), 1);
    ck_assert_int_eq(validate_phone("+7 999 123 45 67"), 1);
    ck_assert_int_eq(validate_phone("1234567890"), 1);
    ck_assert_int_eq(validate_phone(NULL), 1);
}
END_TEST

START_TEST(test_validate_phone_invalid) {
    ck_assert_int_eq(validate_phone("abc"), 0);
    ck_assert_int_eq(validate_phone("+7-999-abc-45-67"), 0);
    ck_assert_int_eq(validate_phone("!@#$"), 0);
}
END_TEST

START_TEST(test_validate_email_valid) {
    ck_assert_int_eq(validate_email(""), 1);
    ck_assert_int_eq(validate_email("test@mail.ru"), 1);
    ck_assert_int_eq(validate_email("user@example.com"), 1);
    ck_assert_int_eq(validate_email("name.surname@domain.org"), 1);
    ck_assert_int_eq(validate_email(NULL), 1);
}
END_TEST

START_TEST(test_validate_email_invalid) {
    ck_assert_int_eq(validate_email("test@"), 0);
    ck_assert_int_eq(validate_email("@mail.ru"), 0);
    ck_assert_int_eq(validate_email("test@mail"), 0);
    ck_assert_int_eq(validate_email("testmail.ru"), 0);
    ck_assert_int_eq(validate_email("test@.ru"), 0);
    ck_assert_int_eq(validate_email("test@mail."), 0);
}
END_TEST

// Создание набора тестов
Suite* validator_suite(void) {
    Suite *s = suite_create("Validator");
    TCase *tc_core = tcase_create("Core");
    
    tcase_add_test(tc_core, test_validate_name_valid);
    tcase_add_test(tc_core, test_validate_name_invalid);
    tcase_add_test(tc_core, test_validate_phone_valid);
    tcase_add_test(tc_core, test_validate_phone_invalid);
    tcase_add_test(tc_core, test_validate_email_valid);
    tcase_add_test(tc_core, test_validate_email_invalid);
    
    suite_add_tcase(s, tc_core);
    return s;
}