#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "contact.h"
#include "phonebook.h"
#include "../Utiles/utiles.h"



//Как работает балансировка

//Каждый раз, когда вы добавляете или удаляете элемент, 
//структура дерева может нарушиться (разница высот превысит единицу). 
//Чтобы вернуть баланс, алгоритм выполняет «повороты» узлов — простые локальные перестановки ветвей, 
//которые восстанавливают правило АВЛ, 
//сохраняя при этом правильный порядок сортировки данных.


// Балансировка вспомогательные функции


static int height(TreeNode *node) {
    return node ? node->height : 0;
}
//Разница глубины между ветками
static int balance_factor(TreeNode *node) {
    return node ? height(node->left) - height(node->right) : 0;
}
static void update_height(TreeNode *node) {
    if (node) {
        node->height = 1 + (height(node->left) > height(node->right) ? 
                            height(node->left) : height(node->right));
    }
}
// Создание нового узла
static TreeNode* create_tree_node(Contact *contact) {
    TreeNode *node = (TreeNode*)malloc(sizeof(TreeNode));
    if (!node) return NULL;
    
    node->contact = (Contact*)malloc(sizeof(Contact));
    if (!node->contact) {
        free(node);
        return NULL;
    }
    
    copy_contact(node->contact, contact);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    
    return node;
}
// Освобождение узла (рекурсивно)
static void free_tree_node(TreeNode *node) {
    if (!node) return;
    
    free_tree_node(node->left);
    free_tree_node(node->right);
    free(node->contact);
    free(node);
}
// Далее функции для сравнения 
// Сравнение контактов для упорядочивания
// static int compare_contacts(const Contact *a, const Contact *b) {
//     int cmp_surname = strcmp(a->surname, b->surname);
//     if (cmp_surname != 0) return cmp_surname;
    
//     return strcmp(a->name, b->name);
// }
// Сравнение по ID
static int compare_by_id(const Contact *a, int id) {
    return a->id - id;
}


static TreeNode* rotate_right(TreeNode *y) {
    TreeNode *x = y->left;
    TreeNode *T2 = x->right;
    
    // Выполняем поворот
    x->right = y;
    y->left = T2;
    
    // Обновляем высоты
    update_height(y);
    update_height(x);
    
    return x;
}

// Левый поворот
static TreeNode* rotate_left(TreeNode *x) {
    TreeNode *y = x->right;
    TreeNode *T2 = y->left;
    
    // Выполняем поворот
    y->left = x;
    x->right = T2;
    
    // Обновляем высоты
    update_height(x);
    update_height(y);
    
    return y;
}

// Балансировка узла
static TreeNode* balance_node(TreeNode *node, int *rotations) {
    if (!node) return NULL;
    
    update_height(node);
    
    // Получаем баланс-фактор
    int bf = balance_factor(node);
    
    // Левое поддерево тяжелее
    if (bf > 1) {
        // Левый-правый случай
        if (balance_factor(node->left) < 0) {
            node->left = rotate_left(node->left);
            if (rotations) (*rotations)++;
        }
        // Левый-левый случай
        node = rotate_right(node);
        if (rotations) (*rotations)++;
    }
    // Правое поддерево тяжелее
    else if (bf < -1) {
        // Правый-левый случай
        if (balance_factor(node->right) > 0) {
            node->right = rotate_right(node->right);
            if (rotations) (*rotations)++;
        }
        // Правый-правый случай
        node = rotate_left(node);
        if (rotations) (*rotations)++;
    }
    
    return node;
}
//Дабавление контакта как узел в дерево в нужное по ФИ
static TreeNode* insert_node(TreeNode *node, Contact *contact, int *rotations, int *inserted) {
    if (!node) {
        TreeNode *new_node = create_tree_node(contact);
        if (new_node) *inserted = 1;
        return new_node;
    }
    int cmp = compare_by_id(contact, node->contact->id);
    //int cmp = compare_contacts(contact, node->contact);
    
    if (cmp < 0) {
        node->left = insert_node(node->left, contact, rotations, inserted);
    } else if (cmp > 0) {
        node->right = insert_node(node->right, contact, rotations, inserted);
    } else {
        // Контакт уже существует
        return node;
    }
    
    // Балансируем текущий узел
    return balance_node(node, rotations);
}



static TreeNode* find_min_node(TreeNode *node) {
    TreeNode *current = node;
    while (current && current->left) {
        current = current->left;
    }
    return current;
}

static TreeNode* delete_node(TreeNode *node, int id, int *deleted, int *rotations) {
    if (!node) return NULL;
    
    int cmp = compare_by_id(node->contact, id);
    
    if (cmp > 0) {
        node->left = delete_node(node->left, id, deleted, rotations);
    } else if (cmp < 0) {
        node->right = delete_node(node->right, id, deleted, rotations);
    } else {
        // Нашли узел для удаления
        *deleted = 1;
        
        if (!node->left || !node->right) {
            // Узел с одним или нулем детей
            TreeNode *temp = node->left ? node->left : node->right;
            free(node->contact);
            free(node);
            return temp;
        } else {
            // Узел с двумя детьми
            TreeNode *min_node = find_min_node(node->right);
            copy_contact(node->contact, min_node->contact);
            node->right = delete_node(node->right, min_node->contact->id, deleted, rotations);
        }
    }
    
    return balance_node(node, rotations);
}

// Поиск в дереву
static Contact* find_node_by_id(TreeNode *node, int id) {
    if (!node) return NULL;
    
    int cmp = compare_by_id(node->contact, id);
    
    if (cmp > 0) return find_node_by_id(node->left, id);
    if (cmp < 0) return find_node_by_id(node->right, id);
    return node->contact;
}
static Contact* find_node_by_name(TreeNode *node, const char *query) {
    if (!node) return NULL;
    
    Contact *found = find_node_by_name(node->left, query);
    if (found) return found;
    
    if (strstr(node->contact->name, query) || 
        strstr(node->contact->surname, query) || 
        strstr(node->contact->patronymic, query)) {
        return node->contact;
    }
    
    return find_node_by_name(node->right, query);
}


static void inorder_traversal(TreeNode *node, int *index) {
    if (!node) return;
    
    inorder_traversal(node->left, index);
    printf("[%d] ", (*index)++);
    print_contact(node->contact);
    printf("\n======================\n");
    inorder_traversal(node->right, index);
}
// static void collect_nodes(TreeNode *node, Contact **array, int *index) {
//     if (!node) return;
    
//     collect_nodes(node->left, array, index);
//     array[(*index)++] = node->contact;
//     collect_nodes(node->right, array, index);
// }




// static TreeNode* build_balanced_tree(Contact **array, int start, int end) {
//     if (start > end) return NULL;
    
//     int mid = (start + end) / 2;
//     TreeNode *node = create_tree_node(array[mid]);
    
//     node->left = build_balanced_tree(array, start, mid - 1);
//     node->right = build_balanced_tree(array, mid + 1, end);
    
//     update_height(node);
//     return node;
// }
static void collect_contacts(TreeNode *node, Contact **array, int *index) {
    if (!node) return;
    
    collect_contacts(node->left, array, index);
    
    // Создаем копию контакта
    Contact *copy = (Contact*)malloc(sizeof(Contact));
    if (copy) {
        copy_contact(copy, node->contact);
        array[(*index)++] = copy;
    }
    
    collect_contacts(node->right, array, index);
}

// Построение сбалансированного дерева из массива
static TreeNode* build_balanced_tree_from_array(Contact **array, int start, int end) {
    if (start > end) return NULL;
    
    int mid = (start + end) / 2;
    
    // Создаем узел из копии контакта
    TreeNode *node = create_tree_node(array[mid]);
    
    node->left = build_balanced_tree_from_array(array, start, mid - 1);
    node->right = build_balanced_tree_from_array(array, mid + 1, end);
    
    update_height(node);
    return node;
}

// ПРИНУДИТЕЛЬНАЯ БАЛАНСИРОВКА


void balance_tree(PhoneBook *pb) {
    if (!pb || pb->count <= 1) return;
    
    // Массив для хранения копий контактов
    Contact **array = (Contact**)malloc(pb->count * sizeof(Contact*));
    if (!array) return;
    
    // Собираем копии контактов
    int index = 0;
    collect_contacts(pb->root, array, &index);
    
    // Сортируем массив по фамилии
    for (int i = 0; i < pb->count - 1; i++) {
        for (int j = 0; j < pb->count - i - 1; j++) {
            if (compare_by_id(array[j], array[j + 1]->id) > 0) {
                Contact *temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }
    
    // Освобождаем старое дерево
    free_tree_node(pb->root);
    pb->root = NULL;
    
    // Строим новое сбалансированное дерево
    pb->root = build_balanced_tree_from_array(array, 0, pb->count - 1);
    
    // Освобождаем массив
    for (int i = 0; i < pb->count; i++) {
        free(array[i]);
    }
    free(array);
    
    // Сбрасываем счетчик ротаций
    pb->rotations = 0;
}







//Основной функционал



//Переписал init под двухсвязный список
int init_phonebook(PhoneBook *pb) {
    if (!pb) return 1;
    
    pb->root = NULL;
    pb->count = 0;
    pb->last_id = 0;
    pb->rotations = 0;

    return 0;
}


// // Создание нового узла
// static Contact* create_node(Contact *contact) {
//     Contact *node = (Contact*)malloc(sizeof(Contact));
//     if (!node) return NULL;
    
//     copy_contact(node, contact);
//     node->prev = NULL;
//     node->next = NULL;
    
//     return node;
// }

// // Освобождение узла
// static void free_node(Contact *node) {
//     if (node) free(node);
// }


//Добавляем сортируя по фамилии
int add_contact(PhoneBook *pb, Contact *contact) {
    if (!pb || !contact) return -3;
    if (is_contact_empty(contact)) return -2;
    if (!is_valid(contact)) return -4;
    
    int inserted = 0;
    int rotations = 0;
    
    pb->root = insert_node(pb->root, contact, &rotations, &inserted);
    
    if (!inserted) return -5;
    
    pb->count++;
    pb->rotations += rotations;
    
    // Если дерево сильно разбалансировано, делаем полную балансировку
    if (pb->rotations > pb->count * 2) {
        balance_tree(pb);
        pb->rotations = 0;
    }
    
    return 0;
}

int delete_contact(PhoneBook *pb, int id) {
    if (!pb) return -1;
    
    int deleted = 0;
    int rotations = 0;
    
    pb->root = delete_node(pb->root, id, &deleted, &rotations);
    
    if (!deleted) return -2;
    
    pb->count--;
    pb->rotations += rotations;
    
    return 0;
}

//Изменяем все кроме ID и указателя
int edit_contact_by(PhoneBook *pb, int id, Contact *new_contact) {
    if (!pb || !new_contact) return -3;
    if (is_contact_empty(new_contact)) return -2;
    
    Contact *found = find_contact_by_id(pb, id);
    if (!found) return -1;
    
    // Сохраняем старый ID
    int old_id = found->id;
    copy_contact(found, new_contact);
    found->id = old_id;
    
    return 0;
}

Contact* find_contact_by_id(PhoneBook *pb, int id) {
    if (!pb) return NULL;
    return find_node_by_id(pb->root, id);
}

Contact* find_contact(PhoneBook *pb, const char *query) {
    if (!pb || !query) return NULL;
    return find_node_by_name(pb->root, query);
}

// void sort_contacts(PhoneBook *pb) {
//     if (!pb || pb->count <= 1) return;
    
//     Contact **array = (Contact**)malloc(pb->count * sizeof(Contact*));
//     if (!array) return;
    
//     int idx = 0;
//     Contact *current = pb->head;
//     while (current != NULL) {
//         array[idx++] = current;
//         current = current->next;
//     }
    
    
//     for (int i = 0; i < pb->count - 1; i++) {
//         for (int j = 0; j < pb->count - i - 1; j++) {
//             if (strcmp(array[j]->surname, array[j + 1]->surname) > 0) {
//                 Contact *temp = array[j];
//                 array[j] = array[j + 1];
//                 array[j + 1] = temp;
//             }
//         }
//     }
    
//     pb->head = array[0];
//     pb->head->prev = NULL;
    
//     for (int i = 0; i < pb->count - 1; i++) {
//         array[i]->next = array[i + 1];
//         array[i + 1]->prev = array[i];
//     }
    
//     pb->tail = array[pb->count - 1];
//     pb->tail->next = NULL;
    
//     free(array);
// }

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
    printf("Статистика: %d ротаций\n", pb->rotations);
    
    int index = 0;
    inorder_traversal(pb->root, &index);
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
    free_tree_node(pb->root);
    pb->root = NULL;
    pb->count = 0;
    pb->last_id = 0;
}

static void display_tree_recursive(TreeNode *node, int level, const char *prefix, int is_last) {
    if (!node) return;
    
    for (int i = 0; i < level - 1; i++) {
        printf("    ");
    }
    
    if (level > 0) {
        printf("%s", is_last ? "└── " : "├── ");
    }
    
    // Выводим ID и ФИО
    printf("ID: %d  %s %s", 
           node->contact->id,
           node->contact->surname,
           node->contact->name);
    if (node->contact->patronymic[0]) {
        printf(" %s", node->contact->patronymic);
    }
    printf("\n");
    
    // Рекурсивно выводим левое и правое поддеревья
    if (node->left || node->right) {
        if (node->left) {
            display_tree_recursive(node->left, level + 1, prefix, !node->right);
        }
        if (node->right) {
            display_tree_recursive(node->right, level + 1, prefix, 1);
        }
    }
}

// Отображение иерархии дерева в консоли
void display_tree_hierarchy(PhoneBook *pb) {
    if (!pb) {
        printf("Ошибка: телефонная книга не инициализирована\n");
        return;
    }
    
    if (pb->count == 0) {
        printf("Дерево пусто\n");
        return;
    }
    
    printf("\nИЕРАРХИЯ ДЕРЕВА (AVL)\n");
    printf("═══════════════════════════════════════════════════════\n");
    printf("Всего узлов: %d\n", pb->count);
    printf("Ротаций: %d\n", pb->rotations);
    printf("───────────────────────────────────────────────────────\n\n");
    
    // Выводим дерево начиная с корня
    display_tree_recursive(pb->root, 0, "", 1);
    
    printf("\n═══════════════════════════════════════════════════════\n");
}
