#ifndef MYLIB_H
#define MYLIB_H

#include <stddef.h>

// динамическая строка для построения выражений
typedef struct {
    char *data;   // буфер
    size_t size;  // текущая длина
    size_t cap;   // вместимость
} String;

// инициализация динамической строки
void str_init(String *s);

// освобождает память строки
void str_free(String *s);

// увеличивает буфер при необходимости (добавление extra символов)
void str_reserve(String *s, size_t extra);

// добавляет один символ
void str_append_char(String *s, char c);

// добавляет строку t
void str_append_str(String *s, const char *t);

// добавляет целое число как текст
void str_append_int(String *s, int x);

// строит совершенную ДНФ по вектору значений
// n  — число переменных
// vec — строка длины 2^n (символы '0'/'1')
// out — результат
void build_sdnf(int n, const char *vec, String *out);

// строит совершенную КНФ по вектору значений
// n  — число переменных
// vec — строка длины 2^n (символы '0'/'1')
// out — результат
void build_sknf(int n, const char *vec, String *out);

#endif