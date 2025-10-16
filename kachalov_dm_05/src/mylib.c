#include "../include/mylib.h"
#include <math.h>

static const double almost_zero_epsilon = 1e-12; // порог сравнения с нулем

static int is_almost_zero(double value) {
    return fabs(value) <= almost_zero_epsilon; // у нас всё-таки double и могут быть некоторые неточности, что 0 обернётся в 0.000000000000054 (например)
}

// Корни характеристического r^2 - a, r - b = 0 (реальные, если D>=0)
wurzeln_t finde_wurzeln(double a, double b) {
    wurzeln_t roots;
    double discriminant = a * a + 4.0 * b; // а у нас в D константен и равен 1 (ax^2 + bx + c, a = 1)

    if (discriminant < -almost_zero_epsilon) { // на всякий
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

// Явная формула (double). Работает для любого дискриминанта. n>=1
double xn_closed_form(double a, double b, double c, double d, uint64_t n) {
    if (n == 1) {
        return c;
    }
    if (n == 2) {
        return d;
    }

    double discriminant = a * a + 4.0 * b;

    if (discriminant > almost_zero_epsilon) {
        double sqrt_discriminant = sqrt(discriminant);
        double root1 = (a + sqrt_discriminant) / 2.0;
        double root2 = (a - sqrt_discriminant) / 2.0;
        konstanten_t constants = finde_konstanten(root1, root2, c, d);
        double power1 = pow(root1, (double)(n - 1));
        double power2 = pow(root2, (double)(n - 1));
        return constants.c1 * power1 + constants.c2 * power2;
    }

    if (is_almost_zero(discriminant)) {
        double repeated_root = a / 2.0;
        if (is_almost_zero(repeated_root)) {
            return 0.0;
        }
        konstanten_t constants = finde_konstanten_doppelt(repeated_root, c, d);
        double power = pow(repeated_root, (double)(n - 1));
        double linear_factor = constants.c1 + constants.c2 * (double)(n - 1);
        return linear_factor * power;
    }

    double complex root1_complex;
    double complex root2_complex;
    finde_wurzeln_complex(a, b, &root1_complex, &root2_complex);

    double complex denominator = root1_complex - root2_complex;
    double complex A_constant;
    double complex B_constant;

    if (cabs(denominator) <= almost_zero_epsilon) {
        double complex repeated_root_complex = root1_complex;
        A_constant = c;
        B_constant = (cabs(repeated_root_complex) <= almost_zero_epsilon) ? 0.0 : (d / repeated_root_complex - c);
    } else {
        A_constant = (d - c * root2_complex) / denominator;
        B_constant = (c * root1_complex - d) / denominator;
    }

    double complex power1_complex = cpow(root1_complex, (double)(n - 1));
    double complex power2_complex = cpow(root2_complex, (double)(n - 1));
    double complex result_complex = A_constant * power1_complex + B_constant * power2_complex;

    return creal(result_complex);
}
