#include <assert.h>
#include "../include/mylib.h"

// проверка базовых свойств дерева
static void check_tree_basic(Tree *t, int n_expected) {
    assert(t != NULL);
    assert(t->n == n_expected);

    int sum_deg = 0;

    for (int v = 1; v <= t->n; v++) {
        assert(t->adj[v] != NULL);
        int deg = t->adj[v][0];
        assert(deg >= 0);
        sum_deg += deg;
    }

    if (n_expected == 1) {
        assert(sum_deg == 0);
    } else {
        assert(sum_deg == 2 * (n_expected - 1));
    }
}

// проверка симметрии: если v соединён с u, то u соединён с v
static void check_tree_symmetry(Tree *t) {
    for (int v = 1; v <= t->n; v++) {
        int deg_v = t->adj[v][0];
        for (int i = 1; i <= deg_v; i++) {
            int u = t->adj[v][i];
            assert(u >= 1 && u <= t->n);
            assert(u != v); // нет петель

            int found = 0;
            int deg_u = t->adj[u][0];
            for (int j = 1; j <= deg_u; j++) {
                if (t->adj[u][j] == v) {
                    found = 1;
                    break;
                }
            }
            assert(found);
        }
    }
}

// проверка связности через DFS/стек
static void check_tree_connected(Tree *t) {
    if (t->n == 0) {
        return;
    }

    int *stack = (int *)malloc((t->n + 1) * sizeof(int));
    int *used = (int *)calloc(t->n + 1, sizeof(int));
    assert(stack != NULL);
    assert(used != NULL);

    int top = 0;
    stack[top++] = 1;
    used[1] = 1;
    int visited = 0;

    while (top > 0) {
        int v = stack[--top];
        visited++;

        int deg = t->adj[v][0];
        for (int i = 1; i <= deg; i++) {
            int u = t->adj[v][i];
            if (!used[u]) {
                used[u] = 1;
                stack[top++] = u;
            }
        }
    }

    assert(visited == t->n);

    free(stack);
    free(used);
}

// тест: пустой код (дерево из 2 вершин)
static void test_empty_code(void) {
    int *code = NULL;
    int m = 0;
    Tree *t = decode_prufer(code, m);
    assert(t != NULL);
    assert(t->n == 2);
    // степени обеих вершин = 1, соседи разные
    assert(t->adj[1][0] == 1);
    assert(t->adj[2][0] == 1);
    assert(t->adj[1][1] == 2);
    assert(t->adj[2][1] == 1);
    free_tree(t);
}

// тест: минимальный непустой код (n = 3, m = 1)
static void test_minimal_code(void) {
    int code[] = {1};
    Tree *t = decode_prufer(code, 1);
    assert(t != NULL);
    assert(t->n == 3);
    // одно возможное дерево: 1 соединён с 2 и 3
    int deg1 = t->adj[1][0];
    int deg2 = t->adj[2][0];
    int deg3 = t->adj[3][0];
    assert(deg1 == 2);
    assert(deg2 == 1);
    assert(deg3 == 1);
    free_tree(t);
}

// тест: нормальный случай (n = 6, код 1 2 1 2) - пример с семинара
static void test_normal_case_6(void) {
    int code[] = {1, 2, 1, 2};
    Tree *t = decode_prufer(code, 4);
    assert(t != NULL);
    assert(t->n == 6);

    // проверка степеней и ключевых связей
    assert(t->adj[1][0] == 3);  // 1: 3 5 2
    assert(t->adj[2][0] == 3);  // 2: 4 1 6
    assert(t->adj[3][0] == 1);
    assert(t->adj[4][0] == 1);
    assert(t->adj[5][0] == 1);
    assert(t->adj[6][0] == 1);

    // взаимные связи (1-2 и 2-1)
    int found = 0;
    for (int i = 1; i <= t->adj[1][0]; i++) {
        if (t->adj[1][i] == 2) {
            found = 1;
        }
    }
    assert(found);

    found = 0;
    for (int i = 1; i <= t->adj[2][0]; i++) {
        if (t->adj[2][i] == 1) {
            found = 1;
        }
    }
    assert(found);

    free_tree(t);
}

// тест: нормальный случай (n = 10, код 1 5 2 6 6 2 1 3)
static void test_normal_case_10(void) {
    int code[] = {1, 5, 2, 6, 6, 2, 1, 3};
    Tree *t = decode_prufer(code, 8);
    assert(t != NULL);
    assert(t->n == 10);

    // выборочно проверемм связи
    // 1: 4 2 3
    int deg1 = t->adj[1][0];
    assert(deg1 == 3);

    // 2: 5 6 1
    int deg2 = t->adj[2][0];
    assert(deg2 == 3);

    // 6: 8 9 2
    int deg6 = t->adj[6][0];
    assert(deg6 == 3);

    // дерево должно содержать 9 рёбер => сумма степеней = 18
    int sum_deg = 0;
    for (int i = 1; i <= 10; i++) {
        sum_deg += t->adj[i][0];
    }
    assert(sum_deg == 18);

    free_tree(t);
}

// несколько генераций для n = 10
static void test_random_n10(void) {
    for (int k = 0; k < 10; k++) { // случайные деревья для n от 0 до 10 действительно являются деревьями
        Tree *t = generate_random_tree(10);
        assert(t != NULL);
        check_tree_basic(t, 10);
        check_tree_symmetry(t);
        check_tree_connected(t);
        free_tree(t);
    }
}

int main(void) {
    test_empty_code();
    test_minimal_code();
    test_normal_case_6();
    test_normal_case_10();

    test_random_n10();

    printf("Все тесты прошли успешно!\n");
    return 0;
}