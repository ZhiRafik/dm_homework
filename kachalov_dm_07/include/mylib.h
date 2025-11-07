#ifndef MYLIB_H
#define MYLIB_H

#include <stddef.h>
#include <stdint.h>

// граф в виде матрицы смежности (0 — нет ребра, 1 — есть ребро)
typedef struct {
    size_t n; // число вершин
    uint8_t **matrix;  // матрица n x n
} Graph;

// создать граф без рёбер, матрица заполнена нулями
// вернуть NULL при ошибке памяти
Graph *create_graph(size_t n);

// добавить ребро (u, v), если вершины в диапазоне и u != v
// не добавляет петли
int add_edge(Graph *g, size_t u, size_t v);

// BFS от вершины start
// вернуть макс расстояние (в рёбрах) от start до любой достижимой вершины
// если start вне диапазона или g == NULL -> 0
int bfs_farthest(const Graph *g, size_t start);

// диаметр графа = макс кратчайшее расстояние между любыми двумя достижимыми вершинами
// перебираем каждую вершину как старт и берём максимум bfs_farthest()
// если граф пустой или из одной вершины -> 0
int compute_graph_diameter(const Graph *g);

// проверяем граф на связность
// если граф связен, возвращаем 1, иначе -1
int check_graph_connectivity(const Graph *g);

// освободить память графа
void free_graph(Graph *g);

#endif