#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/mylib.h"

// Проверка валидации
static void test_clause_valid(void) {
    clause_t c1 = {1, -2, 3};
    clause_t c2 = {0, 1, 2};
    clause_t c3 = {1, 5, 2};
    assert(clause_valid(&c1, 3) == 1);
    assert(clause_valid(&c2, 3) == 0); // 0 не допустим
    assert(clause_valid(&c3, 3) == 0); // 5 > n
}

// Проверка вычисления одной клаузы
static void test_clause_satisfied(void) {
    signed char x[3] = {1, -1, 1};

    clause_t c1 = {1, -2, 3};// истина
    clause_t c2 = {-1,  2, -3}; // ложь
    clause_t c3 = {1, 1, 1}; // истина
    clause_t c4 = {-1, -1, -1}; //ложь

    assert(clause_satisfied(&c1, x, 3) == 1);
    assert(clause_satisfied(&c2, x, 3) == 0);
    assert(clause_satisfied(&c3, x, 3) == 1);
    assert(clause_satisfied(&c4, x, 3) == 0);
}

// Проверка полной формулы — выполнимая и невыполнимая
static void test_is_sat(void) {
    // Пример 1: выполнимая
    unsigned n1 = 3;
    clause_t f1[] = {
        {1, -2, 3},
        {-1, -3, 2}
    };
    assert(is_sat(f1, 2, n1) == 1);

    // Пример 2: невыполнимая
    unsigned n2 = 1;
    clause_t f2[] = {
        {1, 1, 1},
        {-1, -1, -1}
    };
    assert(is_sat(f2, 2, n2) == 0);

    // Пример 3: пустая формула — всегда выполнима
    unsigned n3 = 4;
    clause_t *f3 = NULL;
    assert(is_sat(f3, 0, n3) == 1);
}

// Проверка всех возможных присвоений для маленького n
static void test_small_bruteforce(void) {
    // n=2, формула: (1,2,2) /\ (-1,-2,-2)
    // При x1=-1, x2=1 формула истина
    unsigned n = 2;
    clause_t f[] = {
        {1, 2, 2},
        {-1, -2, -2}
    };
    assert(is_sat(f, 2, n) == 1);
}

int main(void) {
    test_clause_valid();
    test_clause_satisfied();
    test_is_sat();
    test_small_bruteforce();

    printf("Все тесты успешно пройдены!\n");
    return 0;
}