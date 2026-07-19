#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "Data/phonebook.h"
#include "UI/menu.h"


void fill_seed_data(PhoneBook *pb) {
    create_contact(pb, "Иван", "Иванов");
    create_contact(pb, "Петр", "Петров");
    create_contact(pb, "Анна", "Сидорова");
    create_contact(pb, "Мария", "Кузнецова");
    create_contact(pb, "Алексей", "Смирнов");
    create_contact(pb, "Елена", "Попова");
    create_contact(pb, "Дмитрий", "Волков");
    create_contact(pb, "Ольга", "Морозова");
    create_contact(pb, "Сергей", "Новиков");
    create_contact(pb, "Наталья", "Федорова");
    
    Contact *c;
    
    c = find_contact_by_id(pb, 1);
    if (c) {
        edit_contact(c, "67", "+7-999-111-22-33", "ivan@mail.ru");
    }
    
    c = find_contact_by_id(pb, 2);
    if (c) {
        edit_contact(c, "67", "+7-999-444-55-66", "petr@mail.ru");
    }
    
    c = find_contact_by_id(pb, 3);
    if (c) {
        edit_contact(c, "67", "+7-999-777-88-99", "anna@mail.ru");
    }
    
    c = find_contact_by_id(pb, 4);
    if (c) {
        edit_contact(c, "46", "ООО Ромашка", "+7-999-222-33-44");
    }
    
    c = find_contact_by_id(pb, 5);
    if (c) {
        edit_contact(c, "46", "ООО Лютик", "+7-999-555-66-77");
    }
    
    printf("База данных инициализирована начальными данными (%d контактов)\n", pb->count);
}


int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    PhoneBook ph;
    init_phonebook(&ph);
    #ifdef WITH_SEED_DATA
        fill_seed_data(&ph);
        printf("Режим: с исходными данными\n\n");
    #else
        printf("Режим: пустая база данных\n\n");
    #endif
    
    run_phonebook(&ph);

    return 0;
}