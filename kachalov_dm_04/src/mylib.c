#include "../include/mylib.h"
#include <stdio.h>
#include <stdlib.h>

int sign(int i) {
    if (i > 0) {
        return 1;
    }
    if (i < 0) {
        return -1;
    }
    return 0; // 0 не допустим по условию, но на всякий случай
}

// Берём индекс правильно (отрицательный индекс иначе может быть)
static int idx_from_literal(int lit, unsigned n) {
    int a = abs(lit);
    if (a < 1 || (unsigned) a > n) {
        return -1;
    }

    return a - 1;
}

// Валидация на допустимые значения
int clause_valid(const clause_t *c, unsigned n) {
    if (!c) {
        return 0;
    }

    int ai = abs(c->i);
    int aj = abs(c->j); 
    int ak = abs(c->k);

    if (c->i == 0 || c->j == 0 || c->k == 0) {
        return 0;
    }
    if (ai < 1 || (unsigned)ai > n) {
        return 0;
    }
    if (aj < 1 || (unsigned)aj > n) {
        return 0;
    }
    if (ak < 1 || (unsigned)ak > n) {
        return 0;
    }
    
    return 1;
}

int clause_satisfied(const clause_t *c, const signed char *x, unsigned n) {
    // Предполагается, что клауза валидна
    int si = sign(c->i);
    int sj = sign(c->j);
    int sk = sign(c->k);

    int ii = idx_from_literal(c->i, n);
    int jj = idx_from_literal(c->j, n);
    int kk = idx_from_literal(c->k, n);

    if (ii < 0 || jj < 0 || kk < 0) {
        return 0; // защитная проверка
    }

    int a = x[ii] * si; // -> {-1, +1}
    int b = x[jj] * sj;
    int cval = x[kk] * sk; // c уже есть

    // max(a,b,c) >= 0 эквивалентно "хотя бы одно из них == +1"
    if (a >= 0) {
        return 1;
    }
    if (b >= 0) {
        return 1;
    }
    if (cval >= 0) {
        return 1;
    }

    return 0;
}

int all_clauses_satisfied(const clause_t *clauses, unsigned size,
                          const signed char *x, unsigned n) {
    for (unsigned t = 0; t < size; ++t) {
        if (!clause_satisfied(&clauses[t], x, n)) {
            return 0;
        }
    }

    return 1; // если всё подошло, то ура
}

// Массив x[0..n-1] хранит значения из {-1, +1}.
// Начинаем с всех -1, затем "инкремент" как двоичный счётчик:
// -1 -> +1; если было +1, сбрасываем в -1 и переносим к следующему разряду.
// Возвращает 1 если удалось перейти к следующему присвоению, 0 — если перебор завершён.
static void assignment_all_minus1(signed char *x, unsigned n) {
    for (unsigned i = 0; i < n; ++i) {
        x[i] = (signed char) - 1;
    }
}

static int assignment_increment(signed char *x, unsigned n) {
    unsigned pos = 0;
    while (pos < n && x[pos] == (signed char)1) {
        x[pos] = (signed char)-1;
        pos++;
    }
    
    if (pos == n) {
        return 0; // переполнение — вариантов больше нет
    }
    x[pos] = (signed char)1;  // перевели -1 -> +1 на первом не-единичном разряде
    
    return 1;
}

int is_sat(const clause_t *clauses, unsigned size, unsigned n) {
    // Быстрая предвалидация входа
    for (unsigned t = 0; t < size; ++t) {
        if (!clause_valid(&clauses[t], n)) {
            return 0;
        }
    }

    // Пустая формула истинна
    if (size == 0) {
        return 1;
    }
    // Если переменных нет, а клаузы есть — невыполнимо
    if (n == 0) {
        return 0;
    }

    signed char *x = (signed char*)malloc(n * (unsigned)sizeof(signed char));
    if (!x) {
        return 0;
    }

    assignment_all_minus1(x, n);
    for (;;) { // прикольный бесконечный цикл :D
        if (all_clauses_satisfied(clauses, size, x, n)) {
            free(x);
            return 1; // нашли хотя бы один удовлетворяющий для всех
        }
        if (!assignment_increment(x, n)) {
            break; // выходим вручную
        }
    }

    free(x);
    return 0;
}