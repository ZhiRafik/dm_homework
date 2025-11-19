#ifndef MYLIB_H
#define MYLIB_H

#include <stddef.h>
#include <stdbool.h>

// Структура графа
typedef struct graph {
    size_t vertex_count; // количество вершин
    size_t *adj_sizes; // размеры списков смежности
    size_t *adj_capacity; // вместимость списков смежности
    size_t **adj; // списки смежности
} graph_t;

// Создаёт граф
graph_t *graph_create(size_t vertex_count);

// Освобождает граф
void graph_free(graph_t *graph);

// Возвращает количество вершин
size_t graph_get_vertex_count(const graph_t *graph);

// Добавляет ребро u v
int graph_add_edge(graph_t *graph, size_t u, size_t v);

// Проверяет наличие ребра u v
bool graph_has_edge(const graph_t *graph, size_t u, size_t v);

// Выполняет раскраску в 5 цветов
int graph_color_5(const graph_t *graph, int *colors);

// Проверяет корректность раскраски
bool graph_is_coloring_correct(const graph_t *graph,
                               const int *colors,
                               size_t color_count);

// Вспомогательные функции внутренней реализации
int graph_ensure_capacity(graph_t *graph, size_t v, size_t needed);

bool can_use_color(const graph_t *graph, const int *colors, size_t vertex, int color);

bool color_dfs(const graph_t *graph, int *colors, size_t index);

#endif