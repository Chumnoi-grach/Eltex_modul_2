#include <check.h>
#include "../src/Data/contact.h"

START_TEST(test_init_contact) {
    Contact c;
    int result = init_contact(&c);
    
    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(c.id, 0);
    ck_assert_str_eq(c.surname, "");
    ck_assert_str_eq(c.name, "");
    ck_assert_str_eq(c.patronymic, "");
    ck_assert_str_eq(c.workplace, "");
    ck_assert_str_eq(c.position, "");
    ck_assert_str_eq(c.phone, "");
    ck_assert_str_eq(c.email, "");
    ck_assert_str_eq(c.social, "");
    ck_assert_str_eq(c.messenger, "");
}
END_TEST

START_TEST(test_init_contact_null) {
    int result = init_contact(NULL);
    ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(test_copy_contact) {
    Contact src, dest;
    init_contact(&src);
    init_contact(&dest);
    
    strcpy(src.surname, "Иванов");
    strcpy(src.name, "Иван");
    src.id = 123;
    
    int result = copy_contact(&dest, &src);
    
    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(dest.id, 123);
    ck_assert_str_eq(dest.surname, "Иванов");
    ck_assert_str_eq(dest.name, "Иван");
}
END_TEST

START_TEST(test_is_contact_empty) {
    Contact c;
    init_contact(&c);
    
    // Пустой контакт
    ck_assert_int_eq(is_contact_empty(&c), 1);
    
    // Только фамилия
    strcpy(c.surname, "Иванов");
    ck_assert_int_eq(is_contact_empty(&c), 1);
    
    // Имя и фамилия
    strcpy(c.name, "Иван");
    ck_assert_int_eq(is_contact_empty(&c), 0);
}
END_TEST

START_TEST(test_generate_id) {
    Contact c1, c2;
    init_contact(&c1);
    init_contact(&c2);
    
    int id1 = generate_id(&c1);
    int id2 = generate_id(&c2);
    
    ck_assert_int_eq(id1, 1);
    ck_assert_int_eq(id2, 2);
    ck_assert_int_eq(c1.id, 1);
    ck_assert_int_eq(c2.id, 2);
}
END_TEST

START_TEST(test_generate_id_null) {
    int result = generate_id(NULL);
    ck_assert_int_eq(result, -1);
}
END_TEST

START_TEST(test_print_contact) {
    Contact c;
    init_contact(&c);
    strcpy(c.surname, "Иванов");
    strcpy(c.name, "Иван");
    c.id = 1;
    
    // Просто проверяем, что не падает
    int result = print_contact(&c);
    ck_assert_int_eq(result, 0);
}
END_TEST

// Создание набора тестов
Suite* contact_suite(void) {
    Suite *s = suite_create("Contact");
    TCase *tc_core = tcase_create("Core");
    
    tcase_add_test(tc_core, test_init_contact);
    tcase_add_test(tc_core, test_init_contact_null);
    tcase_add_test(tc_core, test_copy_contact);
    tcase_add_test(tc_core, test_is_contact_empty);
    tcase_add_test(tc_core, test_generate_id);
    tcase_add_test(tc_core, test_generate_id_null);
    tcase_add_test(tc_core, test_print_contact);
    
    suite_add_tcase(s, tc_core);
    return s;
}