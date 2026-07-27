#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "permissions.h"
#include "parser.h"
#include "chmod_parser.h"

void clear_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void safe_input(char* buffer, int size) {
    fgets(buffer, size, stdin);
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

void print_menu() {
    printf("   КАЛЬКУЛЯТОР ПРАВ ДОСТУПА\n\n"
        
        "1. Ввести права вручную\n"
        "2. Прочитать права файла\n"
        "3. Изменить текущие права\n"
        "4. Показать текущие права\n"
        "0. Выход\n\n"
    
        "Выберите действие: ");
}

int main() {
    mode_t current_mode = 0;
    int has_current = 0;
    char input[256];
    char filename[256];
    int choice;
    
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) {
            clear_input();
            printf("Ошибка ввода!\n");
            continue;
        }
        clear_input();
        
        switch (choice) {
            case 0:
                printf("Buy buy!\n");
                return 0;
                
            case 1: {
                printf("Введите права (например: 755 или rwxr-xr-x или 110100011): ");
                safe_input(input, sizeof(input));
                
                mode_t mode = parse_permissions(input);
                if (mode == 0 && !is_string_permissions(input) && !is_octal_permissions(input)) {
                    printf("Ошибка: неверный формат прав!\n");
                    break;
                }
                
                current_mode = mode;
                has_current = 1;
                
                printf("\nИтоговые права: ");
                print_permissions(current_mode);
                printf("\n");
                break;
            }
            
            case 2: {
                printf("Введите имя файла: ");
                safe_input(filename, sizeof(filename));
                
                mode_t mode;
                if (get_file_permissions(filename, &mode) != 0) {
                    printf("Ошибка: не удалось прочитать файл '%s'\n", filename);
                    break;
                }
                
                current_mode = mode;
                has_current = 1;
                
                printf("\nПрава файла '%s': ", filename);
                print_permissions(current_mode);
                printf("\n");
                break;
            }
            
            case 3: {
                if (!has_current) {
                    printf("Ошибка: сначала импортируйте права извне!\n");
                    break;
                }
                
                printf("\nТекущие права: ");
                print_permissions(current_mode);
                printf("\n");
                
                printf("\nВведите модификацию прав доступа:\n");
                printf("Примеры: u+x, g-w, o=r, a+x, u=rwx,g=rx,o=r\n");
                printf("Модификация: ");
                safe_input(input, sizeof(input));
                
                mode_t new_mode = apply_chmod_modification(current_mode, input);
                if (new_mode == 0) {
                    printf("Ошибка: неверный формат модификации!\n");
                    break;
                }
                
                printf("\nНовые права:\n");
                print_permissions(new_mode);

                printf("\nСохранить новые права как текущие для дальнейших операций? (y/n): ");
                safe_input(input, sizeof(input));
                if (input[0] == 'y' || input[0] == 'Y') {
                    current_mode = new_mode;
                    printf("Права обновлены.\n");
                } else {
                    printf("Права не изменены.\n");
                }
                break;
            }
            
            case 4: {
                if (!has_current) {
                    printf("Ошибка: сначала импортируйте права извне!\n");
                    break;
                }
                
                printf("\nТекущие права: ");
                print_permissions(current_mode);
                printf("\n");
                break;
            }
            
            default:
                printf("Неверный выбор! Попробуйте снова.\n");
                break;
        }
    }
    
    return 0;
}