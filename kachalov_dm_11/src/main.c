#include "../include/mylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int n;

    printf("Введите число переменных n: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Ошибка: некорректное значение n\n");
        return 1;
    }

    // вычисляем длину вектора значений 2^n
    if (n >= (int)(8 * sizeof(int))) {
        fprintf(stderr, "Ошибка: слишком большое n\n");
        return 1;
    }

    int total = 1 << n;

    // читаем вектор значений функции
    char *vec = (char *)malloc((size_t)total + 1);
    if (!vec) {
        fprintf(stderr, "Ошибка: не удалось выделить память\n");
        return 1;
    }

    printf("Введите вектор значений функции (строка из %d символов 0/1):\n", total);

    if (scanf("%s", vec) != 1) {
        fprintf(stderr, "Ошибка: не удалось прочитать вектор\n");
        free(vec);
        return 1;
    }

    // проверяем длину и корректность символов
    int len = (int)strlen(vec);
    if (len != total) {
        fprintf(stderr, "Ошибка: длина вектора должна быть %d, получено %d\n", total, len);
        free(vec);
        return 1;
    }

    for (int i = 0; i < total; i++) {
        if (vec[i] != '0' && vec[i] != '1') {
            fprintf(stderr, "Ошибка: вектор должен содержать только символы '0' и '1'\n");
            free(vec);
            return 1;
        }
    }

    // строим СДНФ и СКНФ
    String sdnf;
    String sknf;
    str_init(&sdnf);
    str_init(&sknf);

    build_sdnf(n, vec, &sdnf);
    build_sknf(n, vec, &sknf);

    // выводим результат
    printf("СДНФ: %s\n", sdnf.data);
    printf("СКНФ: %s\n", sknf.data);

    // освобождаем память
    str_free(&sdnf);
    str_free(&sknf);
    free(vec);

    return 0;
}