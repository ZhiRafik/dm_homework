#ifndef MYLIB_H
#define MYLIB_H

// Тройка индексов из {-n, …, -1, 1, …, n}, 0 не допустим
typedef struct Clause {
    int i, j, k;
} clause_t;

// sign(i): +1 если i>0, -1 если i<0
int sign(int i);

// Проверяет, что 1 <= |i|,|j|,|k| <= n и ни один не равен 0
int clause_valid(const clause_t *c, unsigned n);

// Возвращает 1, если на данном наборе x[0..n-1] выполняется условие
// max{x_|i| * sgn(i), x_|j| * sgn(j), x_|k| * sgn(k)} >= 0
int clause_satisfied(const clause_t *c, const signed char *x, unsigned n);

// Проверяет, удовлетворяет ли присвоение x[0..n-1] всем клаузам
int all_clauses_satisfied(const clause_t *clauses, unsigned size,
                          const signed char *x, unsigned n);

// Проверяет, существует ли набор x[0..n-1] принадлежит {-1,1}, который удовлетворяет всем клаузам
// Полный перебор по всем 2^n вариантам
int is_sat(const clause_t *clauses, unsigned size, unsigned n);

#endif 