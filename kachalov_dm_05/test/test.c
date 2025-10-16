#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <inttypes.h>
#include "../include/mylib.h"

static const double test_eps = 1e-9; // допуск сравнения double

static int almost_equal(double x, double y) {
    double diff;
    diff = fabs(x - y);
    return diff <= test_eps;
}

static int almost_zero(double x) {
    return fabs(x) <= test_eps;
}

static void assert_real_roots(double a, double b, double r1_expect, double r2_expect) {
    wurzeln_t roots;
    roots = finde_wurzeln(a, b);
    assert(!isnan(roots.r1));
    assert(!isnan(roots.r2));
    assert(almost_equal(roots.r1, r1_expect));
    assert(almost_equal(roots.r2, r2_expect));
    assert(roots.equal == (almost_equal(r1_expect, r2_expect)));
}

static void test_wurzeln_real_distinct(void) {
    double a;
    double b;
    a = 3.0;
    b = -2.0;
    assert_real_roots(a, b, 2.0, 1.0);
}

static void test_wurzeln_real_double(void) {
    double a;
    double b;
    wurzeln_t roots;
    a = 2.0;
    b = -1.0;
    roots = finde_wurzeln(a, b);
    assert(!isnan(roots.r1));
    assert(!isnan(roots.r2));
    assert(roots.equal == 1);
    assert(almost_equal(roots.r1, 1.0));
    assert(almost_equal(roots.r2, 1.0));
}

static void test_wurzeln_negative_discriminant_marks_nan(void) {
    double a;
    double b;
    wurzeln_t roots;
    a = 0.0;
    b = -1.0; // D = -4
    roots = finde_wurzeln(a, b);
    assert(isnan(roots.r1));
    assert(isnan(roots.r2));
    assert(roots.equal == 0);
}

static void test_wurzeln_complex(void) {
    double a;
    double b;
    double complex r1c;
    double complex r2c;
    a = 0.0;
    b = -1.0; // D = -4 => корни +-i
    finde_wurzeln_complex(a, b, &r1c, &r2c);
    assert(almost_zero(creal(r1c)));
    assert(almost_zero(creal(r2c)));
    assert(almost_equal(fabs(cimag(r1c)), 1.0));
    assert(almost_equal(fabs(cimag(r2c)), 1.0));
    assert(almost_equal(cabs(r1c), 1.0));
    assert(almost_equal(cabs(r2c), 1.0));
}

static void test_konstanten_distinct(void) {
    double r1;
    double r2;
    double c1_expect;
    double c2_expect;
    konstanten_t k;
    r1 = 2.0;
    r2 = 1.0;
    k = finde_konstanten(r1, r2, 1.0, 3.0);
    c1_expect = 2.0; // A = (d - c*r2)/(r1 - r2) = (3 - 1)/1 = 2
    c2_expect = -1.0; // B = (c*r1 - d)/(r1 - r2) = (2 - 3)/1 = -1
    assert(almost_equal(k.c1, c1_expect));
    assert(almost_equal(k.c2, c2_expect));
}

static void test_konstanten_doppelt(void) {
    double r;
    double x1;
    double x2;
    konstanten_t k;
    r = 1.0;
    x1 = 5.0;
    x2 = 7.0;
    k = finde_konstanten_doppelt(r, x1, x2);
    assert(almost_equal(k.c1, 5.0)); // A = x1
    assert(almost_equal(k.c2, 2.0)); // B = x2/r - x1 = 7 - 5
}

static void test_konstanten_denominator_near_zero_path(void) {
    double r1;
    double r2;
    double x1;
    double x2;
    konstanten_t k;
    r1 = 1.0;
    r2 = 1.0;
    x1 = 5.0;
    x2 = 7.0;
    k = finde_konstanten(r1, r2, x1, x2);
    assert(almost_equal(k.c1, 5.0));
    assert(almost_equal(k.c2, 2.0));
}

static void test_xn_closed_form_base_cases(void) {
    double a;
    double b;
    double x1;
    double x2;
    double x1_res;
    double x2_res;
    a = 3.0;
    b = -2.0;
    x1 = 10.0;
    x2 = 20.0;
    x1_res = xn_closed_form(a, b, x1, x2, 1);
    x2_res = xn_closed_form(a, b, x1, x2, 2);
    assert(almost_equal(x1_res, x1));
    assert(almost_equal(x2_res, x2));
}

static void test_xn_closed_form_distinct_roots(void) {
    double a;
    double b;
    double c;
    double d;
    double x5;
    a = 3.0;
    b = -2.0;
    c = 1.0;
    d = 3.0;
    x5 = xn_closed_form(a, b, c, d, 5);
    assert(almost_equal(x5, 31.0)); // 2^5 - 1
}

static void test_xn_closed_form_double_root(void) {
    double a;
    double b;
    double c;
    double d;
    double x10;
    a = 2.0;
    b = -1.0; // D = 0, r = 1
    c = 5.0;
    d = 7.0; // A=5, B=2, x_n = 5 + 2(n-1)
    x10 = xn_closed_form(a, b, c, d, 10);
    assert(almost_equal(x10, 23.0)); // 5 + 2*9
}

static void test_xn_closed_form_complex_roots(void) {
    double a;
    double b;
    double c;
    double d;
    double x5;
    double x6;
    a = 0.0;
    b = -1.0; // корни ±i, последовательность: 1,0,-1,0,1,0,...
    c = 1.0;
    d = 0.0;
    x5 = xn_closed_form(a, b, c, d, 5);
    x6 = xn_closed_form(a, b, c, d, 6);
    assert(almost_equal(x5, 1.0));
    assert(almost_equal(x6, 0.0));
}

static void test_xn_closed_form_repeated_root_r_zero_guard(void) {
    double a;
    double b;
    double c;
    double d;
    double x3;
    a = 0.0;
    b = 0.0; // D = 0, r = 0
    c = 2.0;
    d = 3.0;
    x3 = xn_closed_form(a, b, c, d, 3);
    assert(almost_equal(x3, 0.0)); // защита внутри реализации
}

int main(void) {
    test_wurzeln_real_distinct();
    test_wurzeln_real_double();
    test_wurzeln_negative_discriminant_marks_nan();
    test_wurzeln_complex();

    test_konstanten_distinct();
    test_konstanten_doppelt();
    test_konstanten_denominator_near_zero_path();

    test_xn_closed_form_base_cases();
    test_xn_closed_form_distinct_roots();
    test_xn_closed_form_double_root();
    test_xn_closed_form_complex_roots();
    test_xn_closed_form_repeated_root_r_zero_guard();

    printf("Все тесты прошли успешно!\n");
    return 0;
}
