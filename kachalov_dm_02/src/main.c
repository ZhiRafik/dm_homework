#include <stdio.h>
#include <stdlib.h>
#include "../include/mylib.h"

int main() {
    struct Relation r, s;
    printf("Введите количетсво пар в первом отношении R: ");
    initRelation(&r);
    printf("Введите количетсво пар в втором отношении S: ");
    initRelation(&s);
    struct Relation *binary_joined = join_binary(&r, &s);
    printf("Результат композиции (R ∘ S):\n");
    printRelation(binary_joined);
    
    free(r.pairs);
    free(s.pairs);
    free(binary_joined->pairs);
    free(binary_joined);

    return 0;
}