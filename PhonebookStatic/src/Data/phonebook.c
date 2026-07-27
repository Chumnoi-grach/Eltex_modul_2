#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "contact.h"
#include "phonebook.h"
#include "../Utiles/utiles.h"

//Переписал init под двухсвязный список
int init_phonebook(PhoneBook *pb) {
    if (!pb) return 1;
    
    pb->head = NULL;
    pb->tail = NULL;
    pb->count = 0;
    pb->last_id = 0;

    return 0;
}


// Создание нового узла
static Contact* create_node(Contact *contact) {
    Contact *node = (Contact*)malloc(sizeof(Contact));
    if (!node) return NULL;
    
    copy_contact(node, contact);
    node->prev = NULL;
    node->next = NULL;
    
    return node;
}

// Освобождение узла
static void free_node(Contact *node) {
    if (node) free(node);
}


//Добавляем сортируя по фамилии
int add_contact(PhoneBook *pb, Contact *contact) {
    if (!pb || !contact) return -3;
    if (is_contact_empty(contact)) return -2;
    if (!is_valid(contact)) return -4;
    
    Contact *new_node = create_node(contact);
    if (!new_node) return -4;
    
    if (pb->head == NULL) {
        pb->head = new_node;
        pb->tail = new_node;
        pb->count++;
        return 0;
    }
    
    Contact *current = pb->head;
    Contact *prev = NULL;
    
    while (current != NULL && (strcmp(current->surname, new_node->surname) < 0 || (strcmp(current->surname, new_node->surname) == 0 && strcmp(current->name, new_node->name) < 0))) {
        prev = current;
        current = current->next;
    }
    
    // В начало
    if (prev == NULL) {
        new_node->next = pb->head;
        pb->head->prev = new_node;
        pb->head = new_node;
    }
    // В конец
    else if (current == NULL) {
        prev->next = new_node;
        new_node->prev = prev;
        pb->tail = new_node;
    }
    // В середину
    else {
        prev->next = new_node;
        new_node->prev = prev;
        new_node->next = current;
        current->prev = new_node;
    }
    
    pb->count++;
    return 0;
}

int delete_contact(PhoneBook *pb, int id) {
    if (!pb) return -1;
    
    Contact *current = pb->head;
    
    // Ищем контакт по ID
    while (current != NULL && current->id != id) {
        current = current->next;
    }

    if (current == NULL) return -2;
    

    if (current->prev != NULL) {
        current->prev->next = current->next;
    } else {
        pb->head = current->next;
    }
    
    if (current->next != NULL) {
        current->next->prev = current->prev;
    } else {
        pb->tail = current->prev;
    }
    
    free_node(current);
    pb->count--;
    
    return 0;
}

//Изменяем все кроме ID и указателя
int edit_contact_by(PhoneBook *pb, int id, Contact *new_contact) {
    if (!pb || !new_contact) return -3;
    if (is_contact_empty(new_contact)) return -2;
    
    Contact *found = find_contact_by_id(pb, id);
    if (!found) return -1;
    
    int old_id = found->id;
    Contact *prev = found->prev;
    Contact *next = found->next;
    
    // Копируем новые данные
    copy_contact(found, new_contact);
    found->id = old_id;
    found->prev = prev;
    found->next = next;
    
    return 0;
}


//Допилить поиск по нескольким словам, а не по одному
Contact* find_contact(PhoneBook *pb, const char *name) {
    if (!pb || !name) return NULL;
    
    Contact *current = pb->head;
    
    while (current != NULL) {
        if (strstr(current->name, name) || 
            strstr(current->surname, name) || 
            strstr(current->patronymic, name)) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

Contact* find_contact_by_id(PhoneBook *pb, int id) {
    if (!pb) return NULL;
    
    Contact *current = pb->head;
    
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

void sort_contacts(PhoneBook *pb) {
    if (!pb || pb->count <= 1) return;
    
    Contact **array = (Contact**)malloc(pb->count * sizeof(Contact*));
    if (!array) return;
    
    int idx = 0;
    Contact *current = pb->head;
    while (current != NULL) {
        array[idx++] = current;
        current = current->next;
    }
    
    
    for (int i = 0; i < pb->count - 1; i++) {
        for (int j = 0; j < pb->count - i - 1; j++) {
            if (strcmp(array[j]->surname, array[j + 1]->surname) > 0) {
                Contact *temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    
    pb->head = array[0];
    pb->head->prev = NULL;
    
    for (int i = 0; i < pb->count - 1; i++) {
        array[i]->next = array[i + 1];
        array[i + 1]->prev = array[i];
    }
    
    pb->tail = array[pb->count - 1];
    pb->tail->next = NULL;
    
    free(array);
}

void create_contact(PhoneBook *pb, const char *name, const char* surname) {
    if (!pb || !name || !surname) return;

    Contact human;
    init_contact(&human);
    generate_id(&human);
    strncpy(human.name, name, MAX_NAME_LEN - 1);
    human.name[MAX_NAME_LEN - 1] = '\0';
    strncpy(human.surname, surname, MAX_NAME_LEN - 1);
    human.surname[MAX_NAME_LEN - 1] = '\0';
    add_contact(pb, &human);
}


void display_all_contacts(PhoneBook *pb) {
    if (!pb) return;
    
    if (pb->count == 0) {
        printf("Телефонная книга пуста\n");
        return;
    }
    
    printf("Все контакты (%d):\n", pb->count);
    
    Contact *current = pb->head;
    int index = 0;
    
    while (current != NULL) {
        printf("[%d] ", index++);
        print_contact(current);
        printf("\n======================\n");
        current = current->next;
    }
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

void free_phonebook(PhoneBook *pb) {
    if (!pb) return;
    
    Contact *current = pb->head;
    Contact *next;
    
    while (current != NULL) {
        next = current->next;
        free_node(current);
        current = next;
    }
    
    pb->head = NULL;
    pb->tail = NULL;
    pb->count = 0;
    pb->last_id = 0;
}