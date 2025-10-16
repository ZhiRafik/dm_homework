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
    uint64_t n;

    wurzeln_t real_roots;
    double complex root1_complex;
    double complex root2_complex;
    double result_value;

    printf("Введите коэффициенты рекуррентного уравнения:\n");
    printf("x_n = a * x_{n-1} + b * x_{n-2}\n");

    printf("a = ");
    if (scanf("%lf", &a) != 1) {
        printf("Ошибка ввода a\n");
        return 1;
    }

    printf("b = ");
    if (scanf("%lf", &b) != 1) {
        printf("Ошибка ввода b\n");
        return 1;
    }

    printf("Введите начальные значения:\n");

    printf("x1 = ");
    if (scanf("%lf", &c) != 1) {
        printf("Ошибка ввода x1\n");
        return 1;
    }

    printf("x2 = ");
    if (scanf("%lf", &d) != 1) {
        printf("Ошибка ввода x2\n");
        return 1;
    }

    printf("Введите номер n (>=1): ");
    if (scanf("%" SCNu64, &n) != 1) {
        printf("Ошибка ввода n\n");
        return 1;
    }

    printf("\nВычисляем x%" PRIu64 " для уравнения: x_n = %.3f * x_{n-1} + %.3f * x_{n-2}\n", n, a, b);
    printf("Начальные значения: x1 = %.3f, x2 = %.3f\n\n", c, d);

    real_roots = finde_wurzeln(a, b);

    if (isnan(real_roots.r1)) {
        printf("Дискриминант отрицательный — корни комплексные\n");
        finde_wurzeln_complex(a, b, &root1_complex, &root2_complex);
        printf("r1 = %.3f%+.3fi\n", creal(root1_complex), cimag(root1_complex));
        printf("r2 = %.3f%+.3fi\n\n", creal(root2_complex), cimag(root2_complex));
    } else if (real_roots.equal) {
        printf("Кратный корень: r = %.3f\n\n", real_roots.r1);
    } else {
        printf("Два разных корня:\n");
        printf("r1 = %.3f\n", real_roots.r1);
        printf("r2 = %.3f\n\n", real_roots.r2);
    }

    result_value = xn_closed_form(a, b, c, d, n);
    printf("Результат: x%" PRIu64 " = %.10f\n", n, result_value);

    return 0;
}
