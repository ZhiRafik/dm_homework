#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>
#include "../include/mylib.h"

static const double test_eps = 1e-9; // допуск сравнения double

static int almost_equal(double x, double y) {
    double diff;
    diff = fabs(x - y);
    return diff <= test_eps;
}

static int complex_almost_equal(double complex z1, double complex z2) {
    double re_equal;
    double im_equal;
    re_equal = almost_equal(creal(z1), creal(z2));
    im_equal = almost_equal(cimag(z1), cimag(z2));
    return re_equal && im_equal;
}

// Тест: разные действительные корни (a=3, b=-2; r1=2, r2=1)
static void test_real_distinct_roots(void) {
    double a;
    double b;
    double c;
    double d;
    closed_form_data_t data;

    a = 3.0;
    b = -2.0;
    c = 1.0;  // x1
    d = 3.0;  // x2

    data = compute_closed_form(a, b, c, d);

    assert(data.complex_roots == 0);
    assert(data.double_root == 0);
    assert(almost_equal(data.discriminant, a * a + 4.0 * b));
    assert(almost_equal(data.r1, 2.0));
    assert(almost_equal(data.r2, 1.0));

    // Константы: A = 2, B = -1
    assert(almost_equal(data.constants.c1, 2.0));
    assert(almost_equal(data.constants.c2, -1.0));

    // Дублирование в комплексные коэффициенты
    assert(almost_equal(creal(data.A_complex), 2.0));
    assert(almost_equal(cimag(data.A_complex), 0.0));
    assert(almost_equal(creal(data.B_complex), -1.0));
    assert(almost_equal(cimag(data.B_complex), 0.0));
}

// Тест: кратный корень (a = 2, b =- 1; r = 1)
static void test_real_double_root(void) {
    double a;
    double b;
    double c;
    double d;
    closed_form_data_t data;

    a = 2.0;
    b = -1.0;
    c = 5.0; // x1
    d = 7.0; // x2

    data = compute_closed_form(a, b, c, d);

    assert(data.complex_roots == 0);
    assert(data.double_root == 1);
    assert(almost_equal(data.discriminant, 0.0));
    assert(almost_equal(data.r1, 1.0));
    assert(almost_equal(data.r2, 1.0));

    // Кратный корень: A = x1, B = x2/r - x1 = 2
    assert(almost_equal(data.constants.c1, 5.0));
    assert(almost_equal(data.constants.c2, 2.0));

    // В A_complex/B_complex лежат те же коэффициенты
    assert(almost_equal(creal(data.A_complex), 5.0));
    assert(almost_equal(cimag(data.A_complex), 0.0));
    assert(almost_equal(creal(data.B_complex), 2.0));
    assert(almost_equal(cimag(data.B_complex), 0.0));
}

// Тест: комплексные корни (a = 1, b =- 1; D < 0)
static void test_complex_roots(void) {
    double a;
    double b;
    double c;
    double d;
    closed_form_data_t data;
    double complex r1_expected;
    double complex r2_expected;
    double complex diff_expected;
    double complex A_expected;
    double complex B_expected;

    a = 1.0;
    b = -1.0;
    c = 1.0; // x1
    d = 0.0; // x2

    data = compute_closed_form(a, b, c, d);

    assert(data.complex_roots == 1);
    assert(data.double_root == 0);
    assert(data.discriminant < 0.0);

    // r1, r2 — сопряжённые: (a +- i*sqrt(|D|)) / 2
    r1_expected = (a + csqrt((double complex)(a * a + 4.0 * b))) / 2.0;
    r2_expected = (a - csqrt((double complex)(a * a + 4.0 * b))) / 2.0;

    assert(complex_almost_equal(data.r1_complex, r1_expected));
    assert(complex_almost_equal(data.r2_complex, r2_expected));

    // Константы A, B по формуле в комплексной арифметике
    diff_expected = r1_expected - r2_expected;

    if (cabs(diff_expected) <= test_eps) {
        double complex r;
        r = r1_expected;
        A_expected = c;
        if (cabs(r) <= test_eps) {
            B_expected = 0.0;
        } else {
            B_expected = (d / r) - c;
        }
    } else {
        A_expected = (d - c * r2_expected) / diff_expected;
        B_expected = (c * r1_expected - d) / diff_expected;
    }

    assert(complex_almost_equal(data.A_complex, A_expected));
    assert(complex_almost_equal(data.B_complex, B_expected));

    // В этом случае constants.c1/c2 помечены как NaN
    assert(isnan(data.constants.c1));
    assert(isnan(data.constants.c2));
}

// Тест: вырожденный случай r=0 (a=0, b=0)
static void test_r_zero(void) {
    double a;
    double b;
    double c;
    double d;
    closed_form_data_t data;

    a = 0.0;
    b = 0.0;
    c = 2.0;
    d = 3.0;

    data = compute_closed_form(a, b, c, d);

    assert(data.complex_roots == 0);
    assert(data.double_root == 1);
    assert(almost_equal(data.discriminant, 0.0));
    assert(almost_equal(data.r1, 0.0));
    assert(almost_equal(data.r2, 0.0));

    assert(almost_equal(data.constants.c1, c));
    assert(almost_equal(data.constants.c2, 0.0));

    assert(almost_equal(creal(data.A_complex), c));
    assert(almost_equal(cimag(data.A_complex), 0.0));
    assert(almost_equal(creal(data.B_complex), 0.0));
    assert(almost_equal(cimag(data.B_complex), 0.0));
}

// Тест прямых функций: finde_wurzeln и finde_konstanten 
static void test_direct_helpers(void) { 
    wurzeln_t roots; //** если что, я забыл, как будет уравнение по-английски, но вспомнил по-немецки - а там понеслось :D
    konstanten_t k;
    double r1;
    double r2;

    roots = finde_wurzeln(3.0, -2.0);
    assert(!isnan(roots.r1));
    assert(!isnan(roots.r2));
    assert(roots.equal == 0);
    assert(almost_equal(roots.r1, 2.0));
    assert(almost_equal(roots.r2, 1.0));

    r1 = 2.0;
    r2 = 1.0;
    k = finde_konstanten(r1, r2, 1.0, 3.0);
    assert(almost_equal(k.c1, 2.0));
    assert(almost_equal(k.c2, -1.0));
}

int main(void) {
    test_real_distinct_roots();
    test_real_double_root();
    test_complex_roots();
    test_degenerate_r_zero();
    test_direct_helpers();

    printf("Все тесты прошли успешно!\n");
    return 0;
}