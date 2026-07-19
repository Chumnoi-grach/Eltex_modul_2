#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include "contact.h"

#define MAX_PARAMETERS_FOR_EDIT_LEN 9

struct PhoneBook{
    char title[MAX_TITLE];
    Contact contacts[MAX_CONTACTS];
    int count;
    int last_id;
};

typedef struct PhoneBook PhoneBook;


int init_phonebook(PhoneBook *pb);
int add_contact(PhoneBook *pb, Contact *contact);
int edit_contact_by(PhoneBook *pb, int index, Contact *new_contact);
int delete_contact(PhoneBook *pb, int index);
Contact* find_contact(PhoneBook *pb, const char *name);
Contact* find_contact_by_id(PhoneBook *pb, int id);
void sort_contacts(PhoneBook *pb);
void create_contact(PhoneBook *pb, const char *name, const char* surname);
int edit_contact(Contact *c, const char *parameters, ...);

#endif