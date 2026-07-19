#include "menu.h"
#include "input.h"
#include "../Data/contact.h"
#include "../Data/phonebook.h"
#include "../Utiles/utiles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void display_menu(void);
static void handle_add_contact(PhoneBook *pb);
static void handle_edit_contact(PhoneBook *pb);
static void handle_delete_contact(PhoneBook *pb);
static void handle_search_contact_by_id(PhoneBook *pb);
static void handle_search_contact_by_name(PhoneBook *pb);
//static void handle_sort_contact(PhoneBook *pb);


void run_phonebook(PhoneBook *pb) {
    if (!pb) {
        printf("Ошибка: телефонная книга не инициализирована\n");
        return;
    }
    
    int choice;
    while (1) {
        display_menu();
        choice = get_int("Выберите действие: ");
        printf("\n===========================\n");
        switch (choice) {
            case 1: handle_add_contact(pb); break;
            case 2: handle_edit_contact(pb); break;
            case 3: handle_delete_contact(pb); break;
            case 4: display_all_contacts(pb); break;
            case 5: handle_search_contact_by_id(pb); break;
            case 6: handle_search_contact_by_name(pb); break;
            case 7: sort_contacts(pb); 
                    printf("Контакты отсортированы\n"); 
                    break;
            case 0: 
                    printf("Buy buy!\n");
                    return;
            default: 
                    printf("Неверный выбор\n");
        }
    }
}

static void display_menu(void) {
    printf("    ТЕЛЕФОННАЯ КНИГА\n");
    printf("1. Добавить контакт\n");
    printf("2. Редактировать контакт\n");
    printf("3. Удалить контакт\n");
    printf("4. Показать все\n");
    printf("5. Найти контакт по ID\n");
    printf("6. Найти контакт по подстроке в ФИО\n");
    printf("7. Сортировать\n");
    printf("0. Выход\n");
}

static void handle_add_contact(PhoneBook *pb) {
    Contact c;
    init_contact(&c);
    
    printf("\n Введите данные:\n");
    get_string("Фамилия (обяз.): ", c.surname, MAX_NAME_LEN);
    get_string("Имя (обяз.): ", c.name, MAX_NAME_LEN);
    get_string("Отчество (опц.): ", c.patronymic, MAX_NAME_LEN);
    get_string("Работа (опц.): ", c.workplace, MAX_WORK_LEN);
    get_string("Должность (опц.): ", c.position, MAX_WORK_LEN);
    get_string("Телефон (опц.): ", c.phone, MAX_PHONE_LEN);
    get_string("Email (опц.): ", c.email, MAX_EMAIL_LEN);
    get_string("Соцсети (опц.): ", c.social, MAX_SOCIAL_LEN);
    get_string("Мессенджеры (опц.): ", c.messenger, MAX_MESSENGER_LEN);
    
    if (!is_valid(&c)) {
        printf("Ошибка: имя и фамилия обязательны\n");
        return;
    }
    
    generate_id(&c);
    
    int result = add_contact(pb, &c);
    if (result >= 0) {
        printf("Контакт добавлен (ID: %d)\n", c.id);
    } else {
        printf("Ошибка добавления (код: %d)\n", result);
    }
}


static void handle_edit_contact(PhoneBook *pb) {
    int id = get_int("Введите ID контакта для редактирования: ");
    Contact *c = find_contact_by_id(pb, id);
    
    if (!c) {
        printf("Контакт с ID %d не найден\n", id);
        return;
    }
    
    printf("\n Текущие данные:\n");
    print_contact(c);
    
    printf("\n Введите новые данные\n"
        "Маска:\n"
        "surname - 1\n"
        "name - 2\n"
        "patronymic - 3\n"
        "workplace - 4\n"
        "position - 5\n"
        "phone - 6\n"
        "email - 7\n"
        "social - 8\n"
        "messenger - 9\n\n"
        "Формат (по выбору): 123456789\n"
        "(Enter - оставить без изменений):\n");
    
    char parameters[MAX_PARAMETERS_FOR_EDIT_LEN] = "";
    while (1) {
        int error = 0;
        get_string("Введите маску:", parameters, MAX_PARAMETERS_FOR_EDIT_LEN);
        for (int i = 0; parameters[i]; i++) {
            if (parameters[i] < '1' || parameters[i] > '9') {
                printf("Ошибка: неверный параметр '%c'\n", parameters[i]);
                error = 1;
            }
        }
        if (has_duplicates(parameters)) {
            printf("Ошибка: повторяющиеся параметры\n");
            error = 1;
        }
        if (error) continue;
        break;
    }

    int param_len = strlen(parameters);
    if (param_len == 0) {
        printf("Изменений не внесено\n");
        return;
    }

    int error = 0;

    char surname[MAX_NAME_LEN] = "";
    char name[MAX_NAME_LEN] = "";
    char patronymic[MAX_NAME_LEN] = "";
    char workplace[MAX_WORK_LEN] = "";
    char position[MAX_WORK_LEN] = "";
    char phone[MAX_PHONE_LEN] = "";
    char email[MAX_EMAIL_LEN] = "";
    char social[MAX_SOCIAL_LEN] = "";
    char messenger[MAX_MESSENGER_LEN] = "";

    

    for (int i = 0; i < param_len; i++) {
        char field = parameters[i];

        switch (field) {
            case '1': // surname
                get_string("Фамилия: ", surname, MAX_NAME_LEN);
                edit_contact(c, "1", surname);
                break;
                
            case '2': // name
                get_string("Имя: ", name, MAX_NAME_LEN);
                edit_contact(c, "2", name);
                break;
                
            case '3': // patronymic
                get_string("Отчество: ", patronymic, MAX_NAME_LEN);
                edit_contact(c, "3", patronymic);
                break;
                
            case '4': // workplace
                get_string("Место работы: ", workplace, MAX_WORK_LEN);
                edit_contact(c, "4", workplace);
                break;
                
            case '5': // position
                get_string("Должность: ", position, MAX_WORK_LEN);
                edit_contact(c, "5", position);
                break;
                
            case '6': // phone
                get_string("Номер телефона: ", phone, MAX_PHONE_LEN);
                edit_contact(c, "6", phone);
                break;
                
            case '7': // email
                get_string("Email: ", email, MAX_EMAIL_LEN);
                edit_contact(c, "7", email);
                break;
                
            case '8': // social
                get_string("Социальный статус: ", social, MAX_SOCIAL_LEN);
                edit_contact(c, "8", social);
                break;
                
            case '9': // messenger
                get_string("Соцсети: ", messenger, MAX_MESSENGER_LEN);
                edit_contact(c, "9", messenger);
                break;
                
            default:
                fprintf(stderr, "Ошибка: неизвестное поле %c\n", field);
                error = -6;
                break;
        }
        
        
        if (error != 0) break;
    }
    if (strlen(parameters) == 0) {
        printf("Изменений не внесено\n");
        return;
    }
    
}

static void handle_delete_contact(PhoneBook *pb) {
    int id = get_int("Введите ID контакта для удаления: ");
    
    int index = -1;
    for (int i = 0; i < pb->count; i++) {
        if (pb->contacts[i].id == id) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf("Контакт не найден\n");
        return;
    }
    
    printf("Удалить контакт ID %d? (y/n): ", id);
    char confirm = get_char();
    if (confirm == 'y' || confirm == 'Y') {
        if (delete_contact(pb, index) == 0) {
            printf("Контакт удален\n");
        } else {
            printf("Ошибка удаления\n");
        }
    } else {
        printf("ℹОперация отменена\n");
    }
}

static void handle_search_contact_by_id(PhoneBook *pb) {
    int id = get_int("Введите ID: ");
    
    Contact *found = find_contact_by_id(pb, id);
    if (found) {
        printf("\nНайден контакт:\n");
        print_contact(found);
    } else {
        printf("Контакт не найден\n");
    }
}

static void handle_search_contact_by_name(PhoneBook *pb) {
    char query[MAX_NAME_LEN];
    get_string("Введите имя или фамилию: ", query, MAX_NAME_LEN);
    
    Contact *found = find_contact(pb, query);
    if (found) {
        printf("\nНайден контакт:\n");
        print_contact(found);
    } else {
        printf("Контакт не найден\n");
    }
}