#include "math.h"
#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>


#define LIB "lib"


int load_operations(const char *dir_path, Commands **commands, int *operation_count) {
    DIR *dir = opendir(dir_path);
    *commands = NULL;
    *operation_count = 0;
    if (dir == NULL) {
        return -1;
    }
    struct dirent *entry; 
    printf("Файлы библиотеки:\n");
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".so") == NULL) continue;
        
        (*operation_count)++;
        Commands *temp = realloc(*commands, *operation_count * (sizeof(Commands)));
        if (temp == NULL) {
            for (int i = 0; i < *operation_count - 1; i++) {
                if ((*commands)[i].handle) {
                    dlclose((*commands)[i].handle);
                }
            }
            free(*commands);
            *commands = NULL;
            closedir(dir);
            return -3;
        }
        *commands = temp;

        char lib_path[512];
        snprintf(lib_path, sizeof(lib_path), "%s/%s", dir_path, entry->d_name);
        
        void *handle = dlopen(lib_path, RTLD_LAZY);
        if (!handle) {
            printf("Ошибка загрузки библиотеки %s\n", entry->d_name);
            (*operation_count)--;
            continue;
        }
        printf("\t%s\n", entry->d_name);

        char func_name[64];
        snprintf(func_name, strlen(entry->d_name) - 5, "%s", entry->d_name + 3);
        printf("\tfunc_name: %s\n", func_name);
        operation_func math_func = dlsym(handle, func_name);
        if (!math_func) {
            printf("Ошибка: функция %s не найдена в библиотеке %s\n", func_name, entry->d_name);
            dlclose(handle);
            (*operation_count)--;
            continue;
        }
        strncpy((*commands)[(*operation_count) - 1].name, func_name, sizeof((*commands)[*(operation_count) - 1].name) - 1); 
        (*commands)[(*operation_count) - 1].func = math_func;
        (*commands)[(*operation_count) - 1].handle = handle;
    }

    closedir(dir);
    if (*operation_count == 0) {
        free(*commands);
        return -2;
    }
    
    return 0;
}


void cleanup_operations(Commands **commands, int operation_count) {
    if (*commands == NULL) return;
    
    for (int i = 0; i < operation_count; i++) {
        if ((*commands)[i].handle) {
            dlclose((*commands)[i].handle);
        }
    }
    free(*commands);
    *commands = NULL;
}

int main() {
    Commands *commands = NULL;//malloc(sizeof(Commands) * 10);
    int operation_count;
    
    while (1) {
        int load_res = load_operations(LIB, &commands, &operation_count);
        if (load_res == -1) {
            printf("Ошибка: не найдена директория с *.c файлами.\n");
        }
        else if (load_res == -2) {
            printf("Ошибка: нет файлов расширения *.so.\n");
        }
        else if (load_res != 0) {
            printf("%d", load_res);
        }
        char choice[20];
        double num1, num2, result;
        printf("\nВыберите действие:\n");
        printf("1. Сложение (summ)\n");
        printf("2. Вычитание (sub)\n");
        printf("3. Умножение (mul)\n");
        printf("4. Деление (divide)\n");
        printf("5. Возведение в степень (power)\n");
        printf("6. Остаток от деления (ost)\n");
        printf("0. Выход\n");
        
        fgets(choice, sizeof(choice), stdin);
        
        choice[strcspn(choice, "\n")] = '\0';
        if (strcmp(choice, "0") == 0) {
            printf("Buy buy!\n");
            break;
        }
        int number_of_command = -1;
        for (int i = 0; i < operation_count; i++) {
            if (strcmp(commands[i].name, choice) == 0) {
                number_of_command = i;
            }
        }
        if (number_of_command == -1) {
            continue;
        }

        printf("Введите первое число: ");
        scanf("%lf", &num1);
        printf("Введите второе число: ");
        scanf("%lf", &num2);
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        result = commands[number_of_command].func(num1, num2);

        printf("Результат: %f\n", result);
    }

    cleanup_operations(&commands, operation_count);
    
    return 0;
}