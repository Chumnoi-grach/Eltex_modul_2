#include "contact.h"
#include "phonebook.h"
#include "validator.h"
#include <string.h>
#include <stdio.h>


static int next_id = 1;


int init_contact(Contact *c) {
    if (c == NULL) return 1;
    memset(c, 0, sizeof(Contact));
    return 0;
}

int copy_contact(Contact *dest, Contact *src) {
    if (dest == NULL || src == NULL) return 1;
    memcpy(dest, src, sizeof(Contact));
    return 0;
}

int is_contact_empty(Contact *c) {
    if (c == NULL) return 1;
    return c->surname[0] == '\0' || c->name[0] == '\0';
}

int print_contact(Contact *c) {
    printf("ID: %d\n", c->id);
    if (!is_valid(c)) {
        printf("Не соответствие формату\n");
        return 1;
    }

    printf("ФИО: %s %s", c->surname, c->name);
    if (c->patronymic[0]) printf(" %s", c->patronymic);
    printf("\n");
    if (c->workplace[0]) printf("Работа: %s\n", c->workplace);
    if (c->position[0]) printf("Должность: %s\n", c->position);
    if (c->phone[0]) printf("Телефон: %s\n", c->phone);
    if (c->email[0]) printf("Email: %s\n", c->email);
    if (c->social[0]) printf("Соцсети: %s\n", c->social);
    if (c->messenger[0]) printf("Мессенджеры: %s\n", c->messenger);
    return 0;
}

int generate_id(Contact *c) {
    if (c == NULL) return -1;

    c->id = next_id++;
    return c->id;
}

void display_all_contacts(PhoneBook *pb) {
    if (!pb) return;
    if (pb->count == 0) {
        printf("Телефонная книга пуста\n");
    }
    for (int i = 0; i < pb->count; i++) {
        print_contact(&pb->contacts[i]);
        printf("\n======================\n");
    }
}

int is_valid(const Contact *c) {
    if (c == NULL) return 0;
    return validate_name(c->name) && validate_name(c->surname);
}