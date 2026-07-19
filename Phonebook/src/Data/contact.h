#ifndef CONTACT_H
#define CONTACT_H

#define MAX_TITLE 100
#define MAX_NAME_LEN 50
#define MAX_WORK_LEN 100
#define MAX_PHONE_LEN 20
#define MAX_EMAIL_LEN 50
#define MAX_SOCIAL_LEN 100
#define MAX_MESSENGER_LEN 1000
#define MAX_CONTACTS 100



typedef struct PhoneBook PhoneBook;

typedef struct {
    int id;
    char surname[MAX_NAME_LEN];
    char name[MAX_NAME_LEN];
    char patronymic[MAX_NAME_LEN];
    char workplace[MAX_WORK_LEN];
    char position[MAX_WORK_LEN];
    char phone[MAX_PHONE_LEN];
    char email[MAX_EMAIL_LEN];
    char social[MAX_SOCIAL_LEN];
    char messenger[MAX_MESSENGER_LEN];
} Contact;



int init_contact(Contact *c);
int copy_contact(Contact *dest, Contact *src);
int is_contact_empty(Contact *c);
int print_contact(Contact *c);
int generate_id(Contact *c);
void display_all_contacts(PhoneBook *pb);
int is_valid(const Contact *c);

#endif