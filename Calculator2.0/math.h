#ifndef MYMATH
#define MYMATH

typedef double (*operation_func)(double, double);
typedef struct Commands Commands;

double summ(double a, double b);
double mul(double a, double b);
double sub(double a, double b);
double divide(double a, double b);
double ost(double a, double b);
double power(double a, double b);

struct Commands{
    char symbol;
    operation_func func;
};

extern Commands comands[];

#endif