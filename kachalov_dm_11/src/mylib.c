#include "../include/mylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// инициализация динамической строки
void str_init(String *s) {
    s->cap = 128;
    s->size = 0;
    s->data = (char *)malloc(s->cap);
    if (!s->data) {
        fprintf(stderr, "memory allocation error\n");
        exit(1);
    }
    s->data[0] = '\0';
}

// освобождает память строки
void str_free(String *s) {
    if (s->data) {
        free(s->data);
        s->data = NULL;
    }
    s->size = 0;
    s->cap = 0;
}

// увеличивает буфер при необходимости (добавление extra символов)
void str_reserve(String *s, size_t extra) {
    if (s->size + extra + 1 > s->cap) {
        while (s->size + extra + 1 > s->cap) {
            s->cap *= 2;
        }
        char *p = (char *)realloc(s->data, s->cap);
        if (!p) {
            fprintf(stderr, "memory allocation error\n");
            exit(1);
        }
        s->data = p;
    }
}

// добавляет один символ
void str_append_char(String *s, char c) {
    str_reserve(s, 1);
    s->data[s->size++] = c;
    s->data[s->size] = '\0';
}

// добавляет строку t
void str_append_str(String *s, const char *t) {
    size_t len = strlen(t);
    if (len == 0) {
        return;
    }
    str_reserve(s, len);
    memcpy(s->data + s->size, t, len);
    s->size += len;
    s->data[s->size] = '\0';
}

// добавляет целое число как текст
void str_append_int(String *s, int x) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%d", x);
    str_append_str(s, buf);
}

// строит совершенную ДНФ по вектору значений
// n  — число переменных
// vec — строка длины 2^n (символы '0'/'1')
// out — результат
void build_sdnf(int n, const char *vec, String *out) {
    int count_terms = 0;
    int total = 1 << n;

    for (int i = 0; i < total; i++) {
        if (vec[i] != '1') {
            continue;
        }

        if (count_terms > 0) {
            str_append_str(out, " v ");
        }

        str_append_char(out, '(');
        for (int j = 0; j < n; j++) {
            if (j > 0) {
                str_append_str(out, " & ");
            }

            // x1 — старший бит, xn — младший
            int bit_index = n - 1 - j;
            int bit = (i >> bit_index) & 1;

            if (!bit) {
                str_append_char(out, '!');
            }
            str_append_char(out, 'x');
            str_append_int(out, j + 1);
        }
        str_append_char(out, ')');

        count_terms++;
    }

    // функция тождественно 0
    if (count_terms == 0) {
        str_append_char(out, '0');
    }
}

// строит совершенную КНФ по вектору значений
// n  — число переменных
// vec — строка длины 2^n (символы '0'/'1')
// out — результат
void build_sknf(int n, const char *vec, String *out) {
    int count_clauses = 0;
    int total = 1 << n;

    for (int i = 0; i < total; i++) {
        if (vec[i] != '0') {
            continue;
        }

        if (count_clauses > 0) {
            str_append_str(out, " & ");
        }

        str_append_char(out, '(');
        for (int j = 0; j < n; j++) {
            if (j > 0) {
                str_append_str(out, " v ");
            }

            // x1 — старший бит, xn — младший
            int bit_index = n - 1 - j;
            int bit = (i >> bit_index) & 1;

            // для КНФ: 1 -> !xi, 0 -> xi
            if (bit == 1) {
                str_append_char(out, '!');
            }
            str_append_char(out, 'x');
            str_append_int(out, j + 1);
        }
        str_append_char(out, ')');

        count_clauses++;
    }

    // функция тождественно 1
    if (count_clauses == 0) {
        str_append_char(out, '1');
    }
}