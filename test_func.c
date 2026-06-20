#include <math.h>

double test_f1(double x);
double test_f2(double x);
double test_f3(double x);

// f1(x) = x
double test_f1(double x) {
    return x;
}

// f2(x) = x^2 - 2
double test_f2(double x) {
    return x * x - 2.0;
}

// f3(x) = e^x - 1
double test_f3(double x) {
    return exp(x) - 1.0;
}