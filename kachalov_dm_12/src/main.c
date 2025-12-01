#include "../include/mylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

// знак XOR обозначим за ' ^ '
// (можно взять и ⊕ из pdf'ки, но не везде будет хорошо отображаться, наверное)
int main() {
    int n;

    printf("Введите число переменных n: ");
    if (scanf("%d", &n) != 1 || n < 0) {
        printf("Некорректный ввод\n");
        return 1;
    }

    size_t len = 1ULL << n;

    int *a = malloc(len * sizeof(int));
    if (!a) {
        printf("Ошибка: недостаточно памяти\n");
        return 1;
    }

    printf("Введите %zu значений (0 или 1):\n", len);
    for (size_t i = 0; i < len; i++) {
        if (scanf(" %1d", &a[i]) != 1 || (a[i] != 0 && a[i] != 1)) {
            printf("Некорректный ввод\n");
            free(a);
            return 1;
        }
    }

    zhegalkin_transform(a, len);

    char buf[4096];
    build_zhegalkin(a, n, buf, sizeof(buf));

    printf("Полином Жегалкина:\n%s\n", buf);

    free(a);
    return 0;
}