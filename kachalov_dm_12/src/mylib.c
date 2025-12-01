#include "../include/mylib.h"
#include <string.h>
#include <stdio.h>

// преобразование таблицы истинности в коэффициенты полинома Жегалкина
void zhegalkin_transform(int *a, size_t len) {
    if (!a || len == 0) {
        return;
    }

    size_t tmp = len;
    size_t n = 0;

    while ((tmp & 1u) == 0u) {
        tmp >>= 1;
        n++;
    }
    if (tmp != 1u) {
        return;
    }

    for (size_t bit = 0; bit < n; bit++) {
        size_t mask = 1ULL << bit;
        for (size_t i = 0; i < len; i++) {
            if (i & mask) {
                a[i] ^= a[i ^ mask];
            }
        }
    }
}

// построение строки полинома Жегалкина по коэффициентам
void build_zhegalkin(const int *a, int n, char *buf, size_t buf_size) {
    if (!buf || buf_size == 0) {
        return;
    }

    buf[0] = '\0';

    if (!a || n < 0) {
        return;
    }

    size_t len = 1ULL << n;
    int first = 1;

    for (size_t i = 0; i < len; i++) {
        if (!a[i]) {
            continue;
        }

        if (!first) {
            strncat(buf, " ^ ", buf_size - strlen(buf) - 1);
        }
        first = 0;

        if (i == 0) {
            strncat(buf, "1", buf_size - strlen(buf) - 1);
            continue;
        }

        for (int bit = 0; bit < n; bit++) {
            if (i & (1u << bit)) {
                char x[8];
                int var_index = n - bit;
                snprintf(x, sizeof(x), "x%d", var_index);
                strncat(buf, x, buf_size - strlen(buf) - 1);
            }
        }
    }

    if (first) {
        strncpy(buf, "0", buf_size);
        buf[buf_size - 1] = '\0';
    }
}