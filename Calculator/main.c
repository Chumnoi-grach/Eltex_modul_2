#include "math.h"
#include <stdio.h>

int main() {
    int choice;
    double num1, num2, result;
    
    while (1) {
        printf("\nВыберите действие:\n");
        printf("1. Сложение\n");
        printf("2. Вычитание\n");
        printf("3. Умножение\n");
        printf("4. Деление\n");
        printf("5. Возведение в степень\n");
        printf("6. Остаток от деления\n");
        printf("0. Выход\n");
        printf("Ваш выбор: ");
        
        scanf("%d", &choice);
    
        if (choice == 0) {
            printf("Buy buy!\n");
            break;
        }
        if (choice < 1 || choice > 6) {
            printf("Неверный выбор! Попробуйте снова.\n");
            continue;
        }
        
        printf("Введите первое число: ");
        scanf("%lf", &num1);
        printf("Введите второе число: ");
        scanf("%lf", &num2);
        
        switch (choice) {
            case 1:
                result = summ(num1, num2);
                break;
            case 2:
                result = sub(num1, num2);
                break;
            case 3:
                result = mul(num1, num2);
                break;
            case 4:
                result = div(num1, num2);
                break;
            case 5:
                result = power(num1, num2);
                break;
            case 6:
                result = ost(num1, num2);
                break;
        }
        printf("Результат: %f\n", result);
    }
    
    return 0;
}