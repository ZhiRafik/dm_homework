#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/mylib.h"

// проверка правильности одной скобочной последовательности
static int is_valid_psp(const char *s) {
    int balance = 0;
    for (size_t i = 0; s[i] != '\0'; i++) {
        if (s[i] == '(') {
            balance++;
        } else if (s[i] == ')') {
            balance--;
        } else {
            return 0; // недопустимый символ
        }
        if (balance < 0) {
            return 0; // закрывающих больше чем открывающих
        }
    }

    return balance == 0; // баланс должен вернуться к нулю
}

// вычисление числа Каталана для n (через длинное целое 64 бит)
static unsigned long long catalan(unsigned n) {
    unsigned long long c = 1;
    for (unsigned k = 0; k < n; k++) {
        c = c * (4ULL * k + 2ULL) / (k + 2ULL);
    }

    return c;
}

static void check_catalan() {
    unsigned long long expected = 42ULL;
    unsigned long long result = catalan(5);
    assert(result == expected); // проверим корректность для 5, например
}

// тест для одного значения n
static void test_iterator_for_n(unsigned n) {
    iterator_t it;
    iterator_init(&it, n);

    unsigned long long count = 0;

    const cbs_t *v = iterator_value(&it);
    assert(v != NULL);
    assert(v->n == n);

    // цикл по всем значениям
    while (1) {
        assert(is_valid_psp(v->data));
        count++;

        if (!iterator_has_next(&it)) {
            break;
        }
        iterator_next(&it);
        v = iterator_value(&it);
        assert(v != NULL && v->data != NULL);
    }

    unsigned long long expected = catalan(n);
    assert(count == expected);

    iterator_destroy(&it);
    printf("n = %u: прошло (%llu последовательностей)\n", n, count);
}

int main(void) {
    check_catalan();
    test_iterator_for_n(1);
    test_iterator_for_n(2);
    test_iterator_for_n(3);
    test_iterator_for_n(4);
    test_iterator_for_n(5);
    test_iterator_for_n(10);
    printf("Все тесты для значений от 1 до 5 и для 10 прошли успешно!\n");
    return 0;
}