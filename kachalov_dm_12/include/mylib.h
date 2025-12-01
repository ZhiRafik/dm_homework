#ifndef MYLIB_H
#define MYLIB_H

#include <stddef.h>

// преобразование Мёбиуса по XOR
void zhegalkin_transform(int *a, size_t len);

// построение строки полинома Жегалкина -
// обходит полученные коэффициенты и собирает строку полинома,
// добавляя мономы по битам индекса с нумерацией переменных
void build_zhegalkin(const int *a, int n, char *buf, size_t buf_size);

#endif