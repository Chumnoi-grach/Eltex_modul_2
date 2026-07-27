#include <math.h>
#include <stdio.h>
#include "math.h"

double summ(double a, double b) {
    return a + b;
}
double mul(double a, double b){
    return a * b;
}
double sub(double a, double b) {
    return a - b;
}
double div(double a, double b) {
    if (b == 0) {
        printf("Ошибка деления на 0\n");
        return NAN;
    }
    return a / b;
}
double ost(double a, double b) {
    return b? fmod(a, b): NAN;
}
double power(double a, double b) {
    return pow(a,b);
}