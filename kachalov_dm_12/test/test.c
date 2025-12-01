#include "../include/mylib.h"
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

// заполнение массива a по строке из '0'/'1'
static void fill_from_bits(const char *bits, int *a, size_t len) {
    for (size_t i = 0; i < len; i++) {
        a[i] = (bits[i] == '1') ? 1 : 0;
    }
}

// вычисление n, если len = 2^n 
static int calc_n(size_t len) {
    int n = 0;
    size_t tmp = len;
    while ((tmp & 1u) == 0u) {
        tmp >>= 1;
        n++;
    }
    assert(tmp == 1u);
    return n;
}

// кейс 0: "1" -> "1"
static void test_case_0(void) {
    const char *bits = "1";
    int a[1];
    char buf[64];

    fill_from_bits(bits, a, 1);
    int n = calc_n(1);

    zhegalkin_transform(a, 1);
    build_zhegalkin(a, n, buf, sizeof(buf));

    assert(strcmp(buf, "1") == 0);
}

// кейс 1: "01101001" -> "x1 ^ x2 ^ x3" 
static void test_case_1(void) {
    const char *bits = "01101001";
    int a[8];
    char buf[64];

    fill_from_bits(bits, a, 8);
    int n = calc_n(8);

    zhegalkin_transform(a, 8);
    build_zhegalkin(a, n, buf, sizeof(buf));

    assert(strcmp(buf, "x3 ^ x2 ^ x1") == 0);
}

// кейс 2: "01100101" -> "x3 ^ x2 ^ x1x2" 
static void test_case_2(void) {
    const char *bits = "01100101";
    int a[8];
    char buf[64];

    fill_from_bits(bits, a, 8);
    int n = calc_n(8);

    zhegalkin_transform(a, 8);
    build_zhegalkin(a, n, buf, sizeof(buf));

    assert(strcmp(buf, "x3 ^ x2 ^ x2x1") == 0);
}

// кейс 3: "1001" -> "1 ^ x2 ^ x1"
static void test_case_3(void) {
    const char *bits = "1001";
    int a[4];
    char buf[64];

    fill_from_bits(bits, a, 4);
    int n = calc_n(4);

    zhegalkin_transform(a, 4);
    build_zhegalkin(a, n, buf, sizeof(buf));

    assert(strcmp(buf, "1 ^ x2 ^ x1") == 0);
}

// кейс 4: "10" -> "1 ^ x1"
static void test_case_4(void) {
    const char *bits = "10";
    int a[2];
    char buf[64];

    fill_from_bits(bits, a, 2);
    int n = calc_n(2);

    zhegalkin_transform(a, 2);
    build_zhegalkin(a, n, buf, sizeof(buf));

    assert(strcmp(buf, "1 ^ x1") == 0);
}

// кейс 5: "0110100101001011" -> "x1x3 ^ x2 ^ x3 ^ x4 ^ x1x3x4"
static void test_case_5(void) {
    const char *bits = "0110100101001011";
    int a[16];
    char buf[128];

    fill_from_bits(bits, a, 16);
    int n = calc_n(16);

    zhegalkin_transform(a, 16);
    build_zhegalkin(a, n, buf, sizeof(buf));

    assert(strcmp(buf, "x4 ^ x3 ^ x2 ^ x3x1 ^ x4x3x1") == 0);
}

// кейс 6: "0111100001100011" -> "x4 ^ x3 ^ x3x4 ^ x2 ^ x1x3x4 ^ x1x2 ^ x1x2x4"
static void test_case_6(void) {
    const char *bits = "0111100001100011";
    int a[16];
    char buf[256];

    fill_from_bits(bits, a, 16);
    int n = calc_n(16);

    zhegalkin_transform(a, 16);
    build_zhegalkin(a, n, buf, sizeof(buf));

    assert(strcmp(buf, "x4 ^ x3 ^ x4x3 ^ x2 ^ x4x3x1 ^ x2x1 ^ x4x2x1") == 0);
}

// кейс 7: "00111100101001011111000001101001" -> "x4 ^ x3 ^ x2 ^ x2x5 ^ x2x4^ x1 ^ x1x4" 
static void test_case_7(void) {
    const char *bits = "00111100101001011111000001101001";
    int a[32];
    char buf[256];

    fill_from_bits(bits, a, 32);
    int n = calc_n(32);

    zhegalkin_transform(a, 32);
    build_zhegalkin(a, n, buf, sizeof(buf));

    assert(strcmp(buf, "x4 ^ x3 ^ x2 ^ x5x2 ^ x4x2 ^ x1 ^ x4x1") == 0);
}

int main() {
    test_case_0();
    test_case_1();
    test_case_2();
    test_case_3();
    test_case_4();
    test_case_5();
    test_case_6();
    test_case_7();

    printf("Все тесты прошли успешно!\n");

    return 0;
}