#include "../include/mylib.h"
#include <math.h>
#include <stdio.h>

static const double almost_zero_epsilon = 1e-12; // порог сравнения с нулем

static int is_almost_zero(double value) {
    return fabs(value) <= almost_zero_epsilon; // у нас всё-таки double и могут быть некоторые неточности, что 0 обернётся в 0.000000000000054 (например)
}

// Корни характеристического r^2 - a, r - b = 0 (реальные, если D>=0)
wurzeln_t finde_wurzeln(double a, double b) {
    wurzeln_t roots;
    double discriminant = a * a + 4.0 * b; // а у нас в D константен и равен 1 (ax^2 + bx + c, a = 1)

    if (discriminant < -almost_zero_epsilon) { // на всякий, передаём в complex
        roots.r1 = NAN;
        roots.r2 = NAN;
        roots.equal = false;
        return roots;
    }

    if (is_almost_zero(discriminant)) {
        double repeated_root = a / 2.0;
        roots.r1 = repeated_root;
        roots.r2 = repeated_root;
        roots.equal = true;
        return roots;
    }

    double sqrt_discriminant = sqrt(discriminant);
    roots.r1 = (a + sqrt_discriminant) / 2.0;
    roots.r2 = (a - sqrt_discriminant) / 2.0;
    roots.equal = false;
    return roots;
}

// Всегда находит корни (в т.ч. комплексные)
void finde_wurzeln_complex(double a, double b, double complex* root1, double complex* root2) { // сначала начал думать, как делать вручную - а потом узнал про потрясающую <math.h> :D
    double complex discriminant = (double complex)(a * a + 4.0 * b);
    double complex sqrt_discriminant = csqrt(discriminant);
    *root1 = (a + sqrt_discriminant) / 2.0;
    *root2 = (a - sqrt_discriminant) / 2.0;
}

// Константы для случая разных корней (x1=c, x2=d)
konstanten_t finde_konstanten(double root1, double root2, double x1_value, double x2_value) {
    konstanten_t constants;
    double diff = root1 - root2;

    if (is_almost_zero(diff)) {
        double repeated_root = root1;
        constants.c1 = x1_value;
        constants.c2 = is_almost_zero(repeated_root) ? 0.0 : (x2_value / repeated_root - x1_value);
        return constants;
    }

    constants.c1 = (x2_value - x1_value * root2) / diff;
    constants.c2 = (x1_value * root1 - x2_value) / diff;
    return constants;
}

// Константы для кратного корня r (x1=c, x2=d): x_n = (A + B(n-1)) r^(n-1)
konstanten_t finde_konstanten_doppelt(double repeated_root, double x1_value, double x2_value) {
    konstanten_t constants;
    constants.c1 = x1_value;
    constants.c2 = is_almost_zero(repeated_root) ? 0.0 : (x2_value / repeated_root - x1_value);
    return constants;
}

closed_form_data_t compute_closed_form(double a, double b, double c, double d) {
    closed_form_data_t data;
    wurzeln_t roots;
    double complex diff;
    double complex A_complex;
    double complex B_complex;

    data.a = a;
    data.b = b;
    data.discriminant = a * a + 4.0 * b;
    data.complex_roots = false;
    data.double_root = false;
    data.A_complex = 0.0;
    data.B_complex = 0.0;

    roots = finde_wurzeln(a, b);

    // Комплексные корни: finde_wurzeln помечает r1, r2 как NAN
    if (isnan(roots.r1)) {
        data.complex_roots = true;
        finde_wurzeln_complex(a, b, &data.r1_complex, &data.r2_complex);
        diff = data.r1_complex - data.r2_complex;

        if (cabs(diff) <= almost_zero_epsilon) {
            double complex repeated_root;
            repeated_root = data.r1_complex;
            A_complex = c;
            if (cabs(repeated_root) <= almost_zero_epsilon) {
                B_complex = 0.0;
            } else {
                B_complex = (d / repeated_root) - c;
            }
        } else {
            A_complex = (d - c * data.r2_complex) / diff;
            B_complex = (c * data.r1_complex - d) / diff;
        }

        data.A_complex = A_complex;
        data.B_complex = B_complex;
        data.constants.c1 = NAN;
        data.constants.c2 = NAN;
        return data;
    }

    // Кратный действительный корень
    if (roots.equal) {
        data.double_root = true;
        data.r1 = roots.r1;
        data.r2 = roots.r2;
        data.constants = finde_konstanten_doppelt(data.r1, c, d);
        data.A_complex = data.constants.c1;
        data.B_complex = data.constants.c2;
        return data;
    }

    // Два разных действительных корня
    data.r1 = roots.r1;
    data.r2 = roots.r2;

    // Для печати формулы нужны A и B
    data.constants = finde_konstanten(data.r1, data.r2, c, d);
    data.A_complex = data.constants.c1;
    data.B_complex = data.constants.c2;

    return data;
}

void print_closed_form(const closed_form_data_t* data) {
    if (!data) {
        return;
    }

    double c_term;
    char sign_c;
    c_term = -data->b; // в уравнении r^2 - a r - b = 0 свободный член равен (-b)
    sign_c = (c_term >= 0.0) ? '+' : '-';

    
    printf("Характеристическое уравнение: r^2 - %.3f r %c %.3f = 0\n", data->a, sign_c, fabs(c_term));
    printf("Дискриминант D = %.6f\n", data->discriminant);

    if (data->complex_roots) {
        printf("Комплексные корни:\n");
        printf("r1 = %.6f %+.6fi\n", creal(data->r1_complex), cimag(data->r1_complex));
        printf("r2 = %.6f %+.6fi\n", creal(data->r2_complex), cimag(data->r2_complex));

        printf("Коэффициенты:\n");
        printf("A = %.6f %+.6fi\n", creal(data->A_complex), cimag(data->A_complex));
        printf("B = %.6f %+.6fi\n", creal(data->B_complex), cimag(data->B_complex));

        printf("Общий член:\n");
        printf("x_n = (%.6f %+.6fi) * (%.6f %+.6fi)^(n-1) + "
            "(%.6f %+.6fi) * (%.6f %+.6fi)^(n-1)\n",
            creal(data->A_complex), cimag(data->A_complex),
            creal(data->r1_complex), cimag(data->r1_complex),
            creal(data->B_complex), cimag(data->B_complex),
            creal(data->r2_complex), cimag(data->r2_complex));

        return;
    }

    if (data->double_root) {
        printf("Кратный корень: r = %.6f\n", data->r1);
        printf("Общий член:\n");
        printf("x_n = (%.6f + %.6f*(n - 1)) * (%.6f)^(n - 1)\n",
               data->constants.c1, data->constants.c2, data->r1);


        return;
    }

    printf("Два разных корня:\n");
    printf("r1 = %.6f, r2 = %.6f\n", data->r1, data->r2);
    printf("Общий член:\n");
    printf("x_n = (%.6f) * (%.6f)^(n-1) + (%.6f) * (%.6f)^(n-1)\n",
           data->constants.c1, data->r1, data->constants.c2, data->r2);
}
