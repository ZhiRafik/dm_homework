#ifndef MYLIB_H
#define MYLIB_H

#include <stdbool.h>
#include <complex.h>
#include <stdint.h>

typedef struct {
    double r1; // корень 1
    double r2; // корень 2
    bool equal; // true, если r1 == r2 (кратный корень)
} wurzeln_t;

typedef struct {
    double c1; // коэффициент при r1^(n-1)
    double c2; // коэффициент при r2^(n-1)
} konstanten_t;

typedef struct {
    bool complex_roots;
    bool double_root;
    double a;
    double b;
    double discriminant;
    double r1;
    double r2;
    double complex r1_complex;
    double complex r2_complex;
    double complex A_complex; // коэффициент A для общего члена, если корни комплексные
    double complex B_complex;
    konstanten_t constants;
} closed_form_data_t;

// Корни характеристического r^2 - a r - b = 0 (реальные, если D>=0) 
wurzeln_t finde_wurzeln(double a, double b);

// Всегда находит корни (в т.ч. комплексные) 
void finde_wurzeln_complex(double a, double b, double complex* r1, double complex* r2);

// Константы для случая разных корней (x1=c, x2=d) 
konstanten_t finde_konstanten(double r1, double r2, double c, double d);

/* Константы для кратного корня r (x1=c, x2=d):
   x_n = (A + B (n-1)) r^(n-1), возвращает A=c1, B=c2 */
konstanten_t finde_konstanten_doppelt(double r, double c, double d);

// Вычисляет формулу общего члена последовательности x_n
closed_form_data_t compute_closed_form(double a, double b, double c, double d);

// Печатает формулу общего члена последовательности x_n
void print_closed_form(const closed_form_data_t* data);

#endif
