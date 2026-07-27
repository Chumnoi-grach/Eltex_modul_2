#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include "contact.h"

typedef struct PhoneBook PhoneBook;


typedef struct TreeNode {
    Contact *contact;
    struct TreeNode *left;
    struct TreeNode *right;
    int height;
} TreeNode;



struct PhoneBook{
    char title[MAX_TITLE];
    TreeNode *root;
    int count;
    int last_id;
    int rotations;
};




int init_phonebook(PhoneBook *pb);
int add_contact(PhoneBook *pb, Contact *contact);
int edit_contact_by(PhoneBook *pb, int id, Contact *new_contact);
int delete_contact(PhoneBook *pb, int id);
Contact* find_contact(PhoneBook *pb, const char *name);
Contact* find_contact_by_id(PhoneBook *pb, int id);
void sort_contacts(PhoneBook *pb);
void create_contact(PhoneBook *pb, const char *name, const char* surname);
int edit_contact(Contact *c, const char *parameters, ...);
void free_phonebook(PhoneBook *pb);
void display_all_contacts(PhoneBook *pb);
void balance_tree(PhoneBook *pb);

void display_tree_hierarchy(PhoneBook *pb);

#endif