#include "../include/mylib.h"

#include <stdlib.h>

// Вспомогательная функция для изменения вместимости списка вершины
int graph_ensure_capacity(graph_t *graph, size_t v, size_t needed) {
    size_t current_capacity;
    size_t new_capacity;
    size_t *new_data;

    current_capacity = graph->adj_capacity[v];
    if (current_capacity >= needed) {
        return 0;
    }

    if (current_capacity == 0) {
        new_capacity = 4;
    } else {
        new_capacity = current_capacity;
    }

    while (new_capacity < needed) {
        if (new_capacity > (SIZE_MAX / 2)) {
            return 1;
        }
        new_capacity *= 2;
    }

    new_data = realloc(graph->adj[v], new_capacity * sizeof(size_t));
    if (new_data == NULL) {
        return 1;
    }

    graph->adj[v] = new_data;
    graph->adj_capacity[v] = new_capacity;

    return 0;
}

// Проверка возможности использовать цвет для вершины
bool can_use_color(const graph_t *graph, const int *colors,
                    size_t vertex, int color) {
    size_t i;

    for (i = 0; i < graph->adj_sizes[vertex]; i++) {
        size_t to = graph->adj[vertex][i];
        if (colors[to] == color) {
            return false;
        }
    }

    return true;
}

// Рекурсивная раскраска по вершинам
bool color_dfs(const graph_t *graph, int *colors, size_t index) {
    size_t n;
    int color;

    n = graph->vertex_count;
    if (index >= n) {
        return true;
    }

    for (color = 0; color < 5; color++) {
        if (can_use_color(graph, colors, index, color)) {
            colors[index] = color;
            if (color_dfs(graph, colors, index + 1)) {
                return true;
            }
        }
    }

    colors[index] = -1;
    return false;
}

graph_t *graph_create(size_t vertex_count) {
    graph_t *graph;

    graph = malloc(sizeof(graph_t));
    if (graph == NULL) {
        return NULL;
    }

    graph->vertex_count = vertex_count;
    graph->adj_sizes = NULL;
    graph->adj_capacity = NULL;
    graph->adj = NULL;

    if (vertex_count == 0) {
        return graph;
    }

    graph->adj_sizes = calloc(vertex_count, sizeof(size_t));
    if (graph->adj_sizes == NULL) {
        free(graph);
        return NULL;
    }

    graph->adj_capacity = calloc(vertex_count, sizeof(size_t));
    if (graph->adj_capacity == NULL) {
        free(graph->adj_sizes);
        free(graph);
        return NULL;
    }

    graph->adj = calloc(vertex_count, sizeof(size_t *));
    if (graph->adj == NULL) {
        free(graph->adj_capacity);
        free(graph->adj_sizes);
        free(graph);
        return NULL;
    }

    return graph;
}

void graph_free(graph_t *graph) {
    size_t v;

    if (graph == NULL) {
        return;
    }

    if (graph->adj != NULL) {
        for (v = 0; v < graph->vertex_count; v++) {
            free(graph->adj[v]);
        }
    }

    free(graph->adj);
    free(graph->adj_capacity);
    free(graph->adj_sizes);
    free(graph);
}

size_t graph_get_vertex_count(const graph_t *graph) {
    if (graph == NULL) {
        return 0;
    }

    return graph->vertex_count;
}

int graph_add_edge(graph_t *graph, size_t u, size_t v) {
    size_t n;

    if (graph == NULL) {
        return 1;
    }

    n = graph->vertex_count;
    if (u >= n || v >= n) {
        return 1;
    }

    if (u == v) {
        return 1;
    }

    if (graph_has_edge(graph, u, v)) {
        return 1;
    }

    if (graph_ensure_capacity(graph, u, graph->adj_sizes[u] + 1) != 0) {
        return 1;
    }

    if (graph_ensure_capacity(graph, v, graph->adj_sizes[v] + 1) != 0) {
        return 1;
    }

    graph->adj[u][graph->adj_sizes[u]] = v;
    graph->adj_sizes[u] += 1;

    graph->adj[v][graph->adj_sizes[v]] = u;
    graph->adj_sizes[v] += 1;

    return 0;
}

bool graph_has_edge(const graph_t *graph, size_t u, size_t v) {
    size_t n;
    size_t i;

    if (graph == NULL) {
        return false;
    }

    n = graph->vertex_count;
    if (u >= n || v >= n) {
        return false;
    }

    for (i = 0; i < graph->adj_sizes[u]; i++) {
        if (graph->adj[u][i] == v) {
            return true;
        }
    }

    for (i = 0; i < graph->adj_sizes[v]; i++) {
        if (graph->adj[v][i] == u) {
            return true;
        }
    }

    return false;
}

int graph_color_5(const graph_t *graph, int *colors) {
    size_t n;
    size_t i;

    if (graph == NULL || colors == NULL) {
        return 1;
    }

    n = graph->vertex_count;

    for (i = 0; i < n; i++) {
        colors[i] = -1;
    }

    if (!color_dfs(graph, colors, 0)) {
        return 1;
    }

    return 0;
}

bool graph_is_coloring_correct(const graph_t *graph,const int *colors, size_t color_count) {
    size_t n, u, i;

    if (graph == NULL || colors == NULL) {
        return false;
    }

    n = graph->vertex_count;
    if (color_count < n) {
        return false;
    }

    for (u = 0; u < n; u++) {
        if (colors[u] < 0 || colors[u] >= 5) {
            return false;
        }
    }

    for (u = 0; u < n; u++) {
        for (i = 0; i < graph->adj_sizes[u]; i++) {
            size_t v = graph->adj[u][i];
            if (v < n && colors[u] == colors[v]) {
                return false;
            }
        }
    }

    return true;
}