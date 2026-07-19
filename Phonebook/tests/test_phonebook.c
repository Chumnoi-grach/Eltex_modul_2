#include <check.h>
#include "../src/Data/phonebook.h"
#include "../src/Data/contact.h"

static PhoneBook pb;
static Contact test_contact;

void setup_phonebook(void) {
    init_phonebook(&pb);
    init_contact(&test_contact);
    strcpy(test_contact.surname, "Иванов");
    strcpy(test_contact.name, "Иван");
}

void teardown_phonebook(void) {
    // Очистка не нужна, так как pb локальная
}

START_TEST(test_init_phonebook) {
    PhoneBook pb2;
    int result = init_phonebook(&pb2);
    
    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(pb2.count, 0);
    ck_assert_int_eq(pb2.last_id, 0);
    
    // Проверяем, что все контакты инициализированы
    for (int i = 0; i < MAX_CONTACTS; i++) {
        ck_assert_str_eq(pb2.contacts[i].surname, "");
        ck_assert_str_eq(pb2.contacts[i].name, "");
    }
}
END_TEST

START_TEST(test_add_contact) {
    int result = add_contact(&pb, &test_contact);
    
    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(pb.count, 1);
    ck_assert_str_eq(pb.contacts[0].surname, "Иванов");
    ck_assert_str_eq(pb.contacts[0].name, "Иван");
}
END_TEST

START_TEST(test_add_contact_full) {
    // Заполняем книгу до предела
    for (int i = 0; i < MAX_CONTACTS; i++) {
        Contact c;
        init_contact(&c);
        strcpy(c.surname, "Test");
        strcpy(c.name, "Test");
        add_contact(&pb, &c);
    }
    
    // Пытаемся добавить еще один
    int result = add_contact(&pb, &test_contact);
    ck_assert_int_eq(result, -1);
    ck_assert_int_eq(pb.count, MAX_CONTACTS);
}
END_TEST

START_TEST(test_add_contact_empty) {
    Contact empty;
    init_contact(&empty);
    
    int result = add_contact(&pb, &empty);
    ck_assert_int_eq(result, -2);
    ck_assert_int_eq(pb.count, 0);
}
END_TEST

START_TEST(test_add_contact_null) {
    int result = add_contact(&pb, NULL);
    ck_assert_int_eq(result, -3);
}
END_TEST

START_TEST(test_delete_contact) {
    add_contact(&pb, &test_contact);
    ck_assert_int_eq(pb.count, 1);
    
    int result = delete_contact(&pb, 0);
    ck_assert_int_eq(result, 0);
    ck_assert_int_eq(pb.count, 0);
}
END_TEST

START_TEST(test_delete_contact_invalid_index) {
    int result = delete_contact(&pb, 0);
    ck_assert_int_eq(result, -1);
    
    add_contact(&pb, &test_contact);
    result = delete_contact(&pb, 5);
    ck_assert_int_eq(result, -1);
}
END_TEST

START_TEST(test_delete_contact_null) {
    int result = delete_contact(NULL, 0);
    ck_assert_int_eq(result, -1);
}
END_TEST

START_TEST(test_find_contact_by_id) {
    generate_id(&test_contact);
    add_contact(&pb, &test_contact);
    
    Contact *found = find_contact_by_id(&pb, 1);
    ck_assert_ptr_nonnull(found);
    ck_assert_str_eq(found->surname, "Иванов");
    ck_assert_str_eq(found->name, "Иван");
    
    // Несуществующий ID
    found = find_contact_by_id(&pb, 999);
    ck_assert_ptr_null(found);
}
END_TEST

START_TEST(test_sort_contacts) {
    // Добавляем контакты в случайном порядке
    Contact c1, c2, c3;
    init_contact(&c1); strcpy(c1.surname, "Смирнов"); strcpy(c1.name, "Алексей");
    init_contact(&c2); strcpy(c2.surname, "Иванов"); strcpy(c2.name, "Иван");
    init_contact(&c3); strcpy(c3.surname, "Петров"); strcpy(c3.name, "Петр");
    
    add_contact(&pb, &c1);
    add_contact(&pb, &c2);
    add_contact(&pb, &c3);
    
    sort_contacts(&pb);
    
    // Проверяем сортировку по фамилии
    ck_assert_str_eq(pb.contacts[0].surname, "Иванов");
    ck_assert_str_eq(pb.contacts[1].surname, "Петров");
    ck_assert_str_eq(pb.contacts[2].surname, "Смирнов");
}
END_TEST

START_TEST(test_edit_contact_by) {
    add_contact(&pb, &test_contact);
    
    Contact new_data;
    init_contact(&new_data);
    strcpy(new_data.surname, "Петров");
    strcpy(new_data.name, "Петр");
    
    int result = edit_contact_by(&pb, 0, &new_data);
    ck_assert_int_eq(result, 0);
    ck_assert_str_eq(pb.contacts[0].surname, "Петров");
    ck_assert_str_eq(pb.contacts[0].name, "Петр");
}
END_TEST

START_TEST(test_edit_contact_by_invalid) {
    add_contact(&pb, &test_contact);
    
    Contact new_data;
    init_contact(&new_data);
    
    // Пустой контакт
    int result = edit_contact_by(&pb, 0, &new_data);
    ck_assert_int_eq(result, -2);
    
    // Неверный индекс
    strcpy(new_data.surname, "Петров");
    strcpy(new_data.name, "Петр");
    result = edit_contact_by(&pb, 5, &new_data);
    ck_assert_int_eq(result, -1);
}
END_TEST

START_TEST(test_create_contact) {
    create_contact(&pb, "Иван", "Иванов");
    
    ck_assert_int_eq(pb.count, 1);
    ck_assert_str_eq(pb.contacts[0].surname, "Иванов");
    ck_assert_str_eq(pb.contacts[0].name, "Иван");
    ck_assert_int_eq(pb.contacts[0].id, 1);
}
END_TEST

START_TEST(test_find_contact_by_name) {
    create_contact(&pb, "Иван", "Иванов");
    create_contact(&pb, "Петр", "Петров");
    
    Contact *found = find_contact(&pb, "Иван");
    ck_assert_ptr_nonnull(found);
    ck_assert_str_eq(found->surname, "Иванов");
    
    found = find_contact(&pb, "Петров");
    ck_assert_ptr_nonnull(found);
    ck_assert_str_eq(found->name, "Петр");
    
    found = find_contact(&pb, "Несуществующий");
    ck_assert_ptr_null(found);
}
END_TEST

// Создание набора тестов
Suite* phonebook_suite(void) {
    Suite *s = suite_create("PhoneBook");
    TCase *tc_core = tcase_create("Core");
    
    tcase_add_checked_fixture(tc_core, setup_phonebook, teardown_phonebook);
    
    tcase_add_test(tc_core, test_init_phonebook);
    tcase_add_test(tc_core, test_add_contact);
    tcase_add_test(tc_core, test_add_contact_full);
    tcase_add_test(tc_core, test_add_contact_empty);
    tcase_add_test(tc_core, test_add_contact_null);
    tcase_add_test(tc_core, test_delete_contact);
    tcase_add_test(tc_core, test_delete_contact_invalid_index);
    tcase_add_test(tc_core, test_delete_contact_null);
    tcase_add_test(tc_core, test_find_contact_by_id);
    tcase_add_test(tc_core, test_sort_contacts);
    tcase_add_test(tc_core, test_edit_contact_by);
    tcase_add_test(tc_core, test_edit_contact_by_invalid);
    tcase_add_test(tc_core, test_create_contact);
    tcase_add_test(tc_core, test_find_contact_by_name);
    
    suite_add_tcase(s, tc_core);
    return s;
}