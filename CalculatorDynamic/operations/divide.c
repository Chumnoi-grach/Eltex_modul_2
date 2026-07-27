#include <math.h>
#include <stdio.h>
#include "../math.h"

double divide(double a, double b) {
    if (b == 0) {
        //printf("Ошибка деления на 0\n");
        return NAN;
    }
    return a / b;
}