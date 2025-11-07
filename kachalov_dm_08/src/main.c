#include "../include/mylib.h"

int main(void) {
    int mode;
    // обе функциональности включены в одну программу, меняется по выбору режима
    printf("1 — восстановить дерево по коду Прюфера\n");
    printf("2 — сгенерировать случайное дерево\n");
    printf("Выберите режим: ");
    if (scanf("%d", &mode) != 1) {
        printf("Ошибка ввода\n");
        return 1;
    }

    if (mode == 1) {
        int n;

        printf("Введите количество вершин n: ");
        if (scanf("%d", &n) != 1 || n < 2) {
            printf("Ошибка ввода: n должно быть >= 2\n");
            return 1;
        }

        int m = n - 2;

        if (m == 0) {
            // дерево из двух вершин, код Прюфера пустой
            Tree *t = decode_prufer(NULL, 0);
            if (!t) {
                printf("Ошибка при восстановлении дерева\n");
                return 1;
            }
            printf("\nСписки смежности дерева:\n");
            print_tree(t);
            free_tree(t);
            return 0;
        }

        int *code = (int *)malloc(m * sizeof(int));
        if (!code) {
            printf("Ошибка выделения памяти\n");
            return 1;
        }

        printf("Введите код Прюфера: %d чисел от 1 до %d, через пробел или перевод строки\n", m, n);

        for (int i = 0; i < m; i++) {
            if (scanf("%d", &code[i]) != 1) {
                printf("Ошибка ввода элемента кода на позиции %d\n", i + 1);
                free(code);
                return 1;
            }
            if (code[i] < 1 || code[i] > n) {
                printf("Ошибка: элемент кода %d вне диапазона [1, %d]\n", code[i], n);
                free(code);
                return 1;
            }
        }

        Tree *t = decode_prufer(code, m);
        if (!t) {
            printf("Ошибка при восстановлении дерева\n");
            free(code);
            return 1;
        }

        printf("\nСписки смежности дерева:\n");
        print_tree(t);

        free_tree(t);
        free(code);
    }

    else if (mode == 2) {
        int n;

        printf("Введите порядок дерева n: ");
        if (scanf("%d", &n) != 1 || n <= 0) {
            printf("Ошибка ввода: n должно быть положительным\n");
            return 1;
        }

        Tree *t = generate_random_tree(n);
        if (!t) {
            printf("Ошибка генерации дерева\n");
            return 1;
        }

        printf("\nСлучайное дерево порядка %d:\n", n);
        print_tree(t);

        free_tree(t);
    }

    else {
        printf("Неверный режим. Используйте 1 или 2.\n");
        return 1;
    }

    return 0;
}