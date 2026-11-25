#include "vendor/unity.h"

#include <stdint.h>

extern int32_t every_other(int32_t *dest, const int32_t *src, int32_t count);
extern void reverse_tune(int32_t *dest, const int32_t *src, int32_t count);
extern void zigzag(int32_t *dest, const int32_t *src, int32_t count);
extern int32_t keep_high_notes(int32_t *dest, const int32_t *src, int32_t count, int32_t threshold);
extern void echo_previous(int32_t *dest, const int32_t *src, int32_t count, int32_t first);

// Every array in these tests has one extra element after the last one the function is given.
// It holds SENTINEL, which would change the answer if the function read it,
// and the tests check that the function did not change it.
// Every element of dest starts as SENTINEL too, so the tests also check that
// the function wrote nothing past the notes it was asked to write.
#define SENTINEL 0x5EA5EA

static void fill_sentinel(int32_t *array, int32_t count) {
    for (int32_t i = 0; i < count; i++) {
        array[i] = SENTINEL;
    }
}

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_every_other_of_five_notes(void) {
    const int32_t src[] = {1, 3, 5, 3, 1, SENTINEL};
    int32_t dest[] = {SENTINEL, SENTINEL, SENTINEL, SENTINEL};
    const int32_t expected[] = {1, 5, 1, SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, every_other(dest, src, 5), "every_other(dest, {1, 3, 5, 3, 1}, 5)");
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 4, "every_other(dest, {1, 3, 5, 3, 1}, 5)");
}

void test_every_other_of_one_note(void) {
    TEST_IGNORE();
    const int32_t src[] = {8, SENTINEL};
    int32_t dest[] = {SENTINEL, SENTINEL};
    const int32_t expected[] = {8, SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, every_other(dest, src, 1), "every_other(dest, {8}, 1)");
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 2, "every_other(dest, {8}, 1)");
}

void test_every_other_of_no_notes(void) {
    TEST_IGNORE();
    const int32_t src[] = {SENTINEL};
    int32_t dest[] = {SENTINEL};
    const int32_t expected[] = {SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, every_other(dest, src, 0), "every_other(dest, {}, 0)");
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 1, "every_other(dest, {}, 0)");
}

void test_every_other_of_seventeen_notes(void) {
    TEST_IGNORE();
    int32_t src[18];
    int32_t dest[10];
    int32_t expected[10];
    for (int32_t i = 0; i < 17; i++) {
        src[i] = 1 + i % 13;
    }
    src[17] = SENTINEL;
    fill_sentinel(dest, 10);
    for (int32_t i = 0; i < 9; i++) {
        expected[i] = src[2 * i];
    }
    expected[9] = SENTINEL;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(9, every_other(dest, src, 17), "every_other(dest, src, 17) with src[i] = 1 + i % 13");
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 10, "every_other(dest, src, 17) with src[i] = 1 + i % 13");
}

void test_every_other_of_a_hundred_notes(void) {
    TEST_IGNORE();
    int32_t src[101];
    int32_t dest[51];
    int32_t expected[51];
    for (int32_t i = 0; i < 100; i++) {
        src[i] = 100 + i;
    }
    src[100] = SENTINEL;
    fill_sentinel(dest, 51);
    for (int32_t i = 0; i < 50; i++) {
        expected[i] = 100 + 2 * i;
    }
    expected[50] = SENTINEL;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(50, every_other(dest, src, 100), "every_other(dest, src, 100) with src[i] = 100 + i");
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 51, "every_other(dest, src, 100) with src[i] = 100 + i");
}

// TASK: 2
void test_reverse_five_notes(void) {
    TEST_IGNORE();
    const int32_t src[] = {1, 2, 3, 5, 8, SENTINEL};
    int32_t dest[] = {SENTINEL, SENTINEL, SENTINEL, SENTINEL, SENTINEL, SENTINEL};
    const int32_t expected[] = {8, 5, 3, 2, 1, SENTINEL};
    reverse_tune(dest, src, 5);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 6, "reverse_tune(dest, {1, 2, 3, 5, 8}, 5)");
}

void test_reverse_one_note(void) {
    TEST_IGNORE();
    const int32_t src[] = {12, SENTINEL};
    int32_t dest[] = {SENTINEL, SENTINEL};
    const int32_t expected[] = {12, SENTINEL};
    reverse_tune(dest, src, 1);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 2, "reverse_tune(dest, {12}, 1)");
}

void test_reverse_no_notes(void) {
    TEST_IGNORE();
    const int32_t src[] = {SENTINEL};
    int32_t dest[] = {SENTINEL};
    const int32_t expected[] = {SENTINEL};
    reverse_tune(dest, src, 0);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 1, "reverse_tune(dest, {}, 0)");
}

void test_reverse_seventeen_notes(void) {
    TEST_IGNORE();
    int32_t src[18];
    int32_t dest[18];
    int32_t expected[18];
    for (int32_t i = 0; i < 17; i++) {
        src[i] = i + 1;
        expected[i] = 17 - i;
    }
    src[17] = SENTINEL;
    fill_sentinel(dest, 18);
    expected[17] = SENTINEL;
    reverse_tune(dest, src, 17);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 18, "reverse_tune(dest, src, 17) with src[i] = i + 1");
}

void test_reverse_a_hundred_notes(void) {
    TEST_IGNORE();
    int32_t src[101];
    int32_t dest[101];
    int32_t expected[101];
    for (int32_t i = 0; i < 100; i++) {
        src[i] = (i * 7) % 16;
    }
    for (int32_t i = 0; i < 100; i++) {
        expected[i] = src[99 - i];
    }
    src[100] = SENTINEL;
    fill_sentinel(dest, 101);
    expected[100] = SENTINEL;
    reverse_tune(dest, src, 100);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 101, "reverse_tune(dest, src, 100) with src[i] = (i * 7) % 16");
}

// TASK: 3
void test_zigzag_four_notes(void) {
    TEST_IGNORE();
    const int32_t src[] = {1, 2, 3, 4, SENTINEL};
    int32_t dest[] = {SENTINEL, SENTINEL, SENTINEL, SENTINEL, SENTINEL};
    const int32_t expected[] = {2, 1, 4, 3, SENTINEL};
    zigzag(dest, src, 4);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 5, "zigzag(dest, {1, 2, 3, 4}, 4)");
}

void test_zigzag_two_notes(void) {
    TEST_IGNORE();
    const int32_t src[] = {9, 5, SENTINEL};
    int32_t dest[] = {SENTINEL, SENTINEL, SENTINEL};
    const int32_t expected[] = {5, 9, SENTINEL};
    zigzag(dest, src, 2);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 3, "zigzag(dest, {9, 5}, 2)");
}

void test_zigzag_no_notes(void) {
    TEST_IGNORE();
    const int32_t src[] = {SENTINEL};
    int32_t dest[] = {SENTINEL};
    const int32_t expected[] = {SENTINEL};
    zigzag(dest, src, 0);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 1, "zigzag(dest, {}, 0)");
}

void test_zigzag_eighteen_notes(void) {
    TEST_IGNORE();
    int32_t src[19];
    int32_t dest[19];
    int32_t expected[19];
    for (int32_t i = 0; i < 18; i++) {
        src[i] = 10 + i;
        expected[i] = 10 + (i ^ 1);
    }
    src[18] = SENTINEL;
    fill_sentinel(dest, 19);
    expected[18] = SENTINEL;
    zigzag(dest, src, 18);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 19, "zigzag(dest, src, 18) with src[i] = 10 + i");
}

void test_zigzag_a_hundred_notes(void) {
    TEST_IGNORE();
    int32_t src[101];
    int32_t dest[101];
    int32_t expected[101];
    for (int32_t i = 0; i < 100; i++) {
        src[i] = (i * 5) % 16;
    }
    for (int32_t i = 0; i < 100; i++) {
        expected[i] = src[i ^ 1];
    }
    src[100] = SENTINEL;
    fill_sentinel(dest, 101);
    expected[100] = SENTINEL;
    zigzag(dest, src, 100);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 101, "zigzag(dest, src, 100) with src[i] = (i * 5) % 16");
}

// TASK: 4
void test_keep_high_notes_of_five(void) {
    TEST_IGNORE();
    const int32_t src[] = {3, 11, 7, 14, 9, SENTINEL};
    int32_t dest[] = {SENTINEL, SENTINEL, SENTINEL, SENTINEL, SENTINEL, SENTINEL};
    const int32_t expected[] = {11, 14, 9, SENTINEL, SENTINEL, SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, keep_high_notes(dest, src, 5, 8), "keep_high_notes(dest, {3, 11, 7, 14, 9}, 5, 8)");
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 6, "keep_high_notes(dest, {3, 11, 7, 14, 9}, 5, 8)");
}

void test_keep_no_notes_at_the_threshold(void) {
    TEST_IGNORE();
    const int32_t src[] = {8, 2, 8, SENTINEL};
    int32_t dest[] = {SENTINEL, SENTINEL, SENTINEL, SENTINEL};
    const int32_t expected[] = {SENTINEL, SENTINEL, SENTINEL, SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, keep_high_notes(dest, src, 3, 8), "keep_high_notes(dest, {8, 2, 8}, 3, 8)");
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 4, "keep_high_notes(dest, {8, 2, 8}, 3, 8)");
}

void test_keep_high_notes_of_none(void) {
    TEST_IGNORE();
    const int32_t src[] = {SENTINEL};
    int32_t dest[] = {SENTINEL};
    const int32_t expected[] = {SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, keep_high_notes(dest, src, 0, 0), "keep_high_notes(dest, {}, 0, 0)");
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 1, "keep_high_notes(dest, {}, 0, 0)");
}

void test_keep_high_notes_of_seventeen(void) {
    TEST_IGNORE();
    int32_t src[18];
    int32_t dest[18];
    int32_t expected[18];
    int32_t kept = 0;
    fill_sentinel(dest, 18);
    fill_sentinel(expected, 18);
    for (int32_t i = 0; i < 17; i++) {
        src[i] = (i * 5) % 16;
        if (src[i] > 4) {
            expected[kept] = src[i];
            kept++;
        }
    }
    src[17] = SENTINEL;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(kept, keep_high_notes(dest, src, 17, 4),
                                    "keep_high_notes(dest, src, 17, 4) with src[i] = (i * 5) % 16");
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 18, "keep_high_notes(dest, src, 17, 4) with src[i] = (i * 5) % 16");
}

void test_keep_high_notes_of_a_hundred(void) {
    TEST_IGNORE();
    int32_t src[101];
    int32_t dest[101];
    int32_t expected[101];
    int32_t kept = 0;
    fill_sentinel(dest, 101);
    fill_sentinel(expected, 101);
    for (int32_t i = 0; i < 100; i++) {
        src[i] = (i * 7) % 16;
        if (src[i] > 10) {
            expected[kept] = src[i];
            kept++;
        }
    }
    src[100] = SENTINEL;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(kept, keep_high_notes(dest, src, 100, 10),
                                    "keep_high_notes(dest, src, 100, 10) with src[i] = (i * 7) % 16");
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 101, "keep_high_notes(dest, src, 100, 10) with src[i] = (i * 7) % 16");
}

// TASK: 5
void test_echo_five_notes(void) {
    TEST_IGNORE();
    const int32_t src[] = {3, 5, 7, 9, 11, SENTINEL};
    int32_t dest[] = {SENTINEL, SENTINEL, SENTINEL, SENTINEL, SENTINEL, SENTINEL};
    const int32_t expected[] = {1, 3, 5, 7, 9, SENTINEL};
    echo_previous(dest, src, 5, 1);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 6, "echo_previous(dest, {3, 5, 7, 9, 11}, 5, 1)");
}

void test_echo_one_note(void) {
    TEST_IGNORE();
    const int32_t src[] = {6, SENTINEL};
    int32_t dest[] = {SENTINEL, SENTINEL};
    const int32_t expected[] = {4, SENTINEL};
    echo_previous(dest, src, 1, 4);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 2, "echo_previous(dest, {6}, 1, 4)");
}

void test_echo_no_notes(void) {
    TEST_IGNORE();
    const int32_t src[] = {SENTINEL};
    int32_t dest[] = {SENTINEL};
    const int32_t expected[] = {SENTINEL};
    echo_previous(dest, src, 0, 4);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 1, "echo_previous(dest, {}, 0, 4)");
}

void test_echo_seventeen_notes(void) {
    TEST_IGNORE();
    int32_t src[18];
    int32_t dest[18];
    int32_t expected[18];
    for (int32_t i = 0; i < 17; i++) {
        src[i] = 20 + i;
        expected[i] = 19 + i;
    }
    expected[0] = 2;
    src[17] = SENTINEL;
    fill_sentinel(dest, 18);
    expected[17] = SENTINEL;
    echo_previous(dest, src, 17, 2);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 18, "echo_previous(dest, src, 17, 2) with src[i] = 20 + i");
}

void test_echo_a_hundred_notes(void) {
    TEST_IGNORE();
    int32_t src[101];
    int32_t dest[101];
    int32_t expected[101];
    for (int32_t i = 0; i < 100; i++) {
        src[i] = (i * 3) % 16 + 1;
    }
    expected[0] = -1;
    for (int32_t i = 1; i < 100; i++) {
        expected[i] = src[i - 1];
    }
    src[100] = SENTINEL;
    fill_sentinel(dest, 101);
    expected[100] = SENTINEL;
    echo_previous(dest, src, 100, -1);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, dest, 101, "echo_previous(dest, src, 100, -1) with src[i] = (i * 3) % 16 + 1");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_every_other_of_five_notes);
    RUN_TEST(test_every_other_of_one_note);
    RUN_TEST(test_every_other_of_no_notes);
    RUN_TEST(test_every_other_of_seventeen_notes);
    RUN_TEST(test_every_other_of_a_hundred_notes);
    RUN_TEST(test_reverse_five_notes);
    RUN_TEST(test_reverse_one_note);
    RUN_TEST(test_reverse_no_notes);
    RUN_TEST(test_reverse_seventeen_notes);
    RUN_TEST(test_reverse_a_hundred_notes);
    RUN_TEST(test_zigzag_four_notes);
    RUN_TEST(test_zigzag_two_notes);
    RUN_TEST(test_zigzag_no_notes);
    RUN_TEST(test_zigzag_eighteen_notes);
    RUN_TEST(test_zigzag_a_hundred_notes);
    RUN_TEST(test_keep_high_notes_of_five);
    RUN_TEST(test_keep_no_notes_at_the_threshold);
    RUN_TEST(test_keep_high_notes_of_none);
    RUN_TEST(test_keep_high_notes_of_seventeen);
    RUN_TEST(test_keep_high_notes_of_a_hundred);
    RUN_TEST(test_echo_five_notes);
    RUN_TEST(test_echo_one_note);
    RUN_TEST(test_echo_no_notes);
    RUN_TEST(test_echo_seventeen_notes);
    RUN_TEST(test_echo_a_hundred_notes);
    return UNITY_END();
}
