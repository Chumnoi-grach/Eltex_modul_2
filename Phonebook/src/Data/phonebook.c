#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "contact.h"
#include "phonebook.h"
#include "../Utiles/utiles.h"

int init_phonebook(PhoneBook *pb) {
    if (!pb) return 1;
    
    pb->count = 0;
    pb->last_id = 0;
    for (int i = 0; i < MAX_CONTACTS; i++) {
        init_contact(&pb->contacts[i]);
    }
    return 0;
}

int add_contact(PhoneBook *pb, Contact *contact) {
    if (!pb || !contact) return -3;
    if (pb->count >= MAX_CONTACTS) return -1;
    if (is_contact_empty(contact)) return -2;
    if (!is_valid(contact)) return -4;
    
    copy_contact(&pb->contacts[pb->count], contact);
    pb->count++;
    return pb->count - 1;
}

int delete_contact(PhoneBook *pb, int index) {
    if (pb == NULL || index < 0 || index >= pb->count) {
        return -1;
    }
    
    for (int i = index; i < pb->count - 1; i++) {
        copy_contact(&pb->contacts[i], &pb->contacts[i + 1]);
    }
    
    init_contact(&pb->contacts[pb->count - 1]);
    pb->count--;
    return 0;
}

int edit_contact_by(PhoneBook *pb, int index, Contact *new_contact) {
    if (index < 0 || index >= pb->count) {
        return -1;
    }
    
    if (is_contact_empty(new_contact)) {
        return -2;
    }
    
    copy_contact(&pb->contacts[index], new_contact);
    return 0;
}

Contact* find_contact(PhoneBook *pb, const char *name) {
    if (!pb) return NULL;
    
    for (int i = 0; i < pb->count; i++) {
        if (strstr(pb->contacts[i].name, name) || strstr(pb->contacts[i].surname, name) || strstr(pb->contacts[i].patronymic, name)) {
            return &pb->contacts[i];
        }
    }
    return NULL;
}

Contact* find_contact_by_id(PhoneBook *pb, int id) {
    for (int i = 0; i < pb->count; i++) {
        if (pb->contacts[i].id == id) {
            return &pb->contacts[i];
        }
    }
    return NULL;
}

void sort_contacts(PhoneBook *pb) {
    for (int i = 0; i < pb->count - 1; i++) {
        for (int j = 0; j < pb->count - i - 1; j++) {
            if (strcmp(pb->contacts[j].surname, 
                      pb->contacts[j + 1].surname) > 0) {
                Contact temp;
                copy_contact(&temp, &pb->contacts[j]);
                copy_contact(&pb->contacts[j], &pb->contacts[j + 1]);
                copy_contact(&pb->contacts[j + 1], &temp);
            }
        }
    }
}

void create_contact(PhoneBook *pb, const char *name, const char* surname) {
    Contact human;
    init_contact(&human);
    generate_id(&human);
    strncpy(human.name, name, MAX_NAME_LEN - 1);
    human.name[MAX_NAME_LEN - 1] = '\0';
    strncpy(human.surname, surname, MAX_NAME_LEN - 1);
    human.surname[MAX_NAME_LEN - 1] = '\0';
    add_contact(pb, &human);
}

int edit_contact(Contact *c, const char *parameters, ...) {
    if (c == NULL) {
        fprintf(stderr, "Ошибка: контакт NULL\n");
        return -1;
    }
    
    if (parameters == NULL || strlen(parameters) == 0) {
        fprintf(stderr, "Ошибка: параметры не указаны\n");
        return -2;
    }
    
    for (int i = 0; parameters[i]; i++) {
        if (parameters[i] < '1' || parameters[i] > '9') {
            fprintf(stderr, "Ошибка: неверный параметр '%c'\n", parameters[i]);
            return -3;
        }
    }

    if (has_duplicates(parameters)) {
        fprintf(stderr, "Ошибка: повторяющиеся параметры\n");
    }
    
    va_list args;
    va_start(args, parameters);
    
    Contact backup;
    copy_contact(&backup, c);
    
    int field_count = strlen(parameters);
    int error = 0;
    char *new_value;

       
    for (int i = 0; i < field_count; i++) {
        char field = parameters[i];
        new_value = va_arg(args, char*);
        
        if (new_value == NULL) {
            fprintf(stderr, "Ошибка: NULL значение для поля %c\n", field);
            error = -4;
            break;
        }
        
        size_t len = strlen(new_value);
        
        switch (field) {
            case '1': // surname
                if (len >= MAX_NAME_LEN) {
                    fprintf(stderr, "Ошибка: фамилия слишком длинная\n");
                    error = -5;
                    break;
                }
                strcpy(c->surname, new_value);
                break;
                
            case '2': // name
                if (len >= MAX_NAME_LEN) {
                    fprintf(stderr, "Ошибка: имя слишком длинное\n");
                    error = -5;
                    break;
                }
                strcpy(c->name, new_value);
                break;
                
            case '3': // patronymic
                if (len >= MAX_NAME_LEN) {
                    fprintf(stderr, "Ошибка: отчество слишком длинное\n");
                    error = -5;
                    break;
                }
                strcpy(c->patronymic, new_value);
                break;
                
            case '4': // workplace
                if (len >= MAX_WORK_LEN) {
                    fprintf(stderr, "Ошибка: место работы слишком длинное\n");
                    error = -5;
                    break;
                }
                strcpy(c->workplace, new_value);
                break;
                
            case '5': // position
                if (len >= MAX_WORK_LEN) {
                    fprintf(stderr, "Ошибка: должность слишком длинная\n");
                    error = -5;
                    break;
                }
                strcpy(c->position, new_value);
                break;
                
            case '6': // phone
                if (len >= MAX_PHONE_LEN) {
                    fprintf(stderr, "Ошибка: телефон слишком длинный\n");
                    error = -5;
                    break;
                }
                strcpy(c->phone, new_value);
                break;
                
            case '7': // email
                if (len >= MAX_EMAIL_LEN) {
                    fprintf(stderr, "Ошибка: email слишком длинный\n");
                    error = -5;
                    break;
                }
                strcpy(c->email, new_value);
                break;
                
            case '8': // social
                if (len >= MAX_SOCIAL_LEN) {
                    fprintf(stderr, "Ошибка: социальный статус слишком длинный\n");
                    error = -5;
                    break;
                }
                strcpy(c->social, new_value);
                break;
                
            case '9': // messenger
                if (len >= MAX_MESSENGER_LEN) {
                    fprintf(stderr, "Ошибка: мессенджеры слишком длинные\n");
                    error = -5;
                    break;
                }
                strcpy(c->messenger, new_value);
                break;
                
            default:
                fprintf(stderr, "Ошибка: неизвестное поле %c\n", field);
                error = -6;
                break;
        }
        
        if (error != 0) break;
    }
    
    va_end(args);
    
    if (error != 0) {
        copy_contact(c, &backup);
        return error;
    }
    
    if (is_contact_empty(c)) {
        copy_contact(c, &backup);
        fprintf(stderr, "Ошибка: контакт не может быть пустым\n");
        return -7;
    }
    
    return 0;
}