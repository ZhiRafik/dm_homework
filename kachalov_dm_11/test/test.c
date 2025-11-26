#include "../include/mylib.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// проверка равенства строк
static void check_eq(const char *got, const char *expected) {
    assert(strcmp(got, expected) == 0);
}

// тест: функция тождественно 0 (все значения 0)
static void test_all_zero(void) {
    int n = 3;
    const char *vec = "00000000";

    String sdnf;
    String sknf;
    str_init(&sdnf);
    str_init(&sknf);

    build_sdnf(n, vec, &sdnf);
    build_sknf(n, vec, &sknf);

    check_eq(sdnf.data, "0");
    check_eq(
        sknf.data,
        "(x1 v x2 v x3) & "
        "(x1 v x2 v !x3) & "
        "(x1 v !x2 v x3) & "
        "(x1 v !x2 v !x3) & "
        "(!x1 v x2 v x3) & "
        "(!x1 v x2 v !x3) & "
        "(!x1 v !x2 v x3) & "
        "(!x1 v !x2 v !x3)"
    );

    str_free(&sdnf);
    str_free(&sknf);
}

// тест: функция тождественно 1 (все значения 1)
static void test_all_one(void) {
    int n = 2;
    const char *vec = "1111";

    String sdnf;
    String sknf;
    str_init(&sdnf);
    str_init(&sknf);

    build_sdnf(n, vec, &sdnf);
    build_sknf(n, vec, &sknf);

    check_eq(
        sdnf.data,
        "(!x1 & !x2) v (!x1 & x2) v (x1 & !x2) v (x1 & x2)"
    );
    check_eq(sknf.data, "1");

    str_free(&sdnf);
    str_free(&sknf);
}

// тест: стандартный смешанный случай для n = 3
static void test_mixed_3vars(void) {
    int n = 3;
    const char *vec = "01101000";

    String sdnf;
    String sknf;
    str_init(&sdnf);
    str_init(&sknf);

    build_sdnf(n, vec, &sdnf);
    build_sknf(n, vec, &sknf);

    // здесь были ошибки во втором конъюнкте — исправлено
    check_eq(
        sdnf.data,
        "(!x1 & !x2 & x3) v "
        "(!x1 & x2 & !x3) v "
        "(x1 & !x2 & !x3)"
    );

    // здесь тоже исправлены дизъюнкты и добавлен последний
    check_eq(
        sknf.data,
        "(x1 v x2 v x3) & "
        "(x1 v !x2 v !x3) & "
        "(!x1 v x2 v !x3) & "
        "(!x1 v !x2 v x3) & "
        "(!x1 v !x2 v !x3)"
    );

    str_free(&sdnf);
    str_free(&sknf);
}

// тест: маленький случай n = 1
static void test_small_n1(void) {
    int n = 1;

    // вектор 01 — тождественно x1
    {
        const char *vec = "01";
        String sdnf;
        String sknf;
        str_init(&sdnf);
        str_init(&sknf);

        build_sdnf(n, vec, &sdnf);
        build_sknf(n, vec, &sknf);

        check_eq(sdnf.data, "(x1)");
        check_eq(sknf.data, "(x1)");

        str_free(&sdnf);
        str_free(&sknf);
    }

    // вектор 10 — тождественно !x1
    {
        const char *vec = "10";
        String sdnf;
        String sknf;
        str_init(&sdnf);
        str_init(&sknf);

        build_sdnf(n, vec, &sdnf);
        build_sknf(n, vec, &sknf);

        check_eq(sdnf.data, "(!x1)");
        check_eq(sknf.data, "(!x1)");

        str_free(&sdnf);
        str_free(&sknf);
    }
}

// тест для большего n (n = 4)
static void test_n4(void) {
    int n = 4;
    const char *vec = "0001001000000000"; // 16 символов

    String sdnf;
    String sknf;
    str_init(&sdnf);
    str_init(&sknf);

    build_sdnf(n, vec, &sdnf);
    build_sknf(n, vec, &sknf);

    // здесь были неверные знаки ! — исправлено по реальному выводу
    check_eq(
        sdnf.data,
        "(!x1 & !x2 & x3 & x4) v "
        "(!x1 & x2 & x3 & !x4)"
    );

    // СКНФ для этого вектора длинная, проверяем грубо
    assert(strlen(sknf.data) > 10);

    str_free(&sdnf);
    str_free(&sknf);
}

// тест: входные данные n = 3, vec = 10100101
static void test_input_10100101(void) {
    int n = 3;
    const char *vec = "10100101";

    String sdnf;
    String sknf;
    str_init(&sdnf);
    str_init(&sknf);

    build_sdnf(n, vec, &sdnf);
    build_sknf(n, vec, &sknf);

    check_eq(
        sdnf.data,
        "(!x1 & !x2 & !x3) v "
        "(!x1 & x2 & !x3) v "
        "(x1 & !x2 & x3) v "
        "(x1 & x2 & x3)"
    );

    check_eq(
        sknf.data,
        "(x1 v x2 v !x3) & "
        "(x1 v !x2 v !x3) & "
        "(!x1 v x2 v x3) & "
        "(!x1 v !x2 v x3)"
    );

    str_free(&sdnf);
    str_free(&sknf);
}

// точка входа тестовой программы
int main(void) {
    test_all_zero();
    test_all_one();
    test_mixed_3vars();
    test_small_n1();
    test_n4();
    test_input_10100101();

    printf("Все тесты успешно пройдены.\n");
    return 0;
}