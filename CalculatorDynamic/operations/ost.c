#include <math.h>
#include <stdio.h>
#include "../math.h"

double ost(double a, double b) {
    return b? fmod(a, b): NAN;
}