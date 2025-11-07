#include <stdio.h>
#include <complex.h>
#include <inttypes.h>
#include <math.h>
#include "../include/mylib.h"

int main(void) {
    double a;
    double b;
    double c;
    double d;
    closed_form_data_t data;

    printf("Введите коэффициенты уравнения: x_n = a * x_{n-1} + b * x_{n-2}\n");

    printf("a = ");
    scanf("%lf", &a);
    printf("b = ");
    scanf("%lf", &b);
    printf("x1 = ");
    scanf("%lf", &c);
    printf("x2 = ");
    scanf("%lf", &d);

    data = compute_closed_form(a, b, c, d);

    printf("\n");
    print_closed_form(&data);
    printf("\n");

    return 0;
}

/*
Пример рабоыт программы:

Введите коэффициенты уравнения: x_n = a * x_{n-1} + b * x_{n-2}
a = 3
b = -5
x1 = 4
x2 = 5

Характеристическое уравнение: r^2 - 3.000 r + 5.000 = 0
Дискриминант D = -11.000000
Комплексные корни:
r1 = 1.500000 +1.658312i
r2 = 1.500000 -1.658312i
Коэффициенты:
A = 2.000000 +0.301511i
B = 2.000000 -0.301511i
Общий член:
x_n = (2.000000 +0.301511i) * (1.500000 +1.658312i)^(n-1) + (2.000000 -0.301511i) * (1.500000 -1.658312i)^(n-1) 
*/