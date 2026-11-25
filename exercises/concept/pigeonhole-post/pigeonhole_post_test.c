#include "vendor/unity.h"

#include <stdint.h>

extern int32_t letters_in_first(const int32_t *row);
extern int32_t letters_in_third(const int32_t *row);
extern int32_t total_of_four(const int32_t *row);
extern void deliver(int32_t *pigeonhole, int32_t count);
extern void move_letters(int32_t *from, int32_t *to);
extern void swap_pigeonholes(int32_t *a, int32_t *b);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_first_of_a_busy_row(void) {
    const int32_t row[] = {7, 2, 9, 4};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(7, letters_in_first(row), "letters_in_first({7, 2, 9, 4})");
}

void test_first_pigeonhole_is_empty(void) {
    TEST_IGNORE();
    const int32_t row[] = {0, 5, 5, 5};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, letters_in_first(row), "letters_in_first({0, 5, 5, 5})");
}

void test_first_of_a_row_with_one_full_pigeonhole(void) {
    TEST_IGNORE();
    const int32_t row[] = {31, 0, 0, 0};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(31, letters_in_first(row), "letters_in_first({31, 0, 0, 0})");
}

// TASK: 2
void test_third_of_a_busy_row(void) {
    TEST_IGNORE();
    const int32_t row[] = {7, 2, 9, 4};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(9, letters_in_third(row), "letters_in_third({7, 2, 9, 4})");
}

void test_third_pigeonhole_is_empty(void) {
    TEST_IGNORE();
    const int32_t row[] = {3, 8, 0, 6};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, letters_in_third(row), "letters_in_third({3, 8, 0, 6})");
}

void test_only_the_third_has_letters(void) {
    TEST_IGNORE();
    const int32_t row[] = {0, 0, 12, 0};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(12, letters_in_third(row), "letters_in_third({0, 0, 12, 0})");
}

void test_third_of_a_row_of_different_counts(void) {
    TEST_IGNORE();
    const int32_t row[] = {1, 2, 3, 4};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, letters_in_third(row), "letters_in_third({1, 2, 3, 4})");
}

// TASK: 3
void test_total_of_a_busy_row(void) {
    TEST_IGNORE();
    const int32_t row[] = {7, 2, 9, 4};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(22, total_of_four(row), "total_of_four({7, 2, 9, 4})");
}

void test_total_of_an_empty_row(void) {
    TEST_IGNORE();
    const int32_t row[] = {0, 0, 0, 0};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, total_of_four(row), "total_of_four({0, 0, 0, 0})");
}

void test_total_when_only_the_last_has_letters(void) {
    TEST_IGNORE();
    const int32_t row[] = {0, 0, 0, 15};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(15, total_of_four(row), "total_of_four({0, 0, 0, 15})");
}

void test_total_of_a_row_after_the_christmas_rush(void) {
    TEST_IGNORE();
    const int32_t row[] = {120, 75, 210, 98};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(503, total_of_four(row), "total_of_four({120, 75, 210, 98})");
}

// TASK: 4
void test_deliver_to_a_pigeonhole_with_letters(void) {
    TEST_IGNORE();
    int32_t pigeonhole = 5;
    deliver(&pigeonhole, 3);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(8, pigeonhole, "deliver(&pigeonhole, 3) with pigeonhole = 5");
}

void test_deliver_to_an_empty_pigeonhole(void) {
    TEST_IGNORE();
    int32_t pigeonhole = 0;
    deliver(&pigeonhole, 4);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, pigeonhole, "deliver(&pigeonhole, 4) with pigeonhole = 0");
}

void test_deliver_no_letters(void) {
    TEST_IGNORE();
    int32_t pigeonhole = 6;
    deliver(&pigeonhole, 0);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(6, pigeonhole, "deliver(&pigeonhole, 0) with pigeonhole = 6");
}

void test_deliver_to_the_middle_of_a_row(void) {
    TEST_IGNORE();
    int32_t row[] = {7, 2, 9, 4};
    const int32_t expected[] = {7, 12, 9, 4};
    deliver(&row[1], 10);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 4, "deliver(&row[1], 10) with row = {7, 2, 9, 4}");
}

// TASK: 5
void test_move_letters_to_a_pigeonhole_with_letters(void) {
    TEST_IGNORE();
    int32_t row[] = {4, 1};
    const int32_t expected[] = {0, 5};
    move_letters(&row[0], &row[1]);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 2, "move_letters(&row[0], &row[1]) with row = {4, 1}");
}

void test_move_letters_to_an_empty_pigeonhole(void) {
    TEST_IGNORE();
    int32_t row[] = {0, 9};
    const int32_t expected[] = {9, 0};
    move_letters(&row[1], &row[0]);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 2, "move_letters(&row[1], &row[0]) with row = {0, 9}");
}

void test_move_letters_from_an_empty_pigeonhole(void) {
    TEST_IGNORE();
    int32_t row[] = {0, 3};
    const int32_t expected[] = {0, 3};
    move_letters(&row[0], &row[1]);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 2, "move_letters(&row[0], &row[1]) with row = {0, 3}");
}

void test_move_letters_across_a_row(void) {
    TEST_IGNORE();
    int32_t row[] = {6, 2, 8, 11};
    const int32_t expected[] = {6, 0, 8, 13};
    move_letters(&row[1], &row[3]);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 4, "move_letters(&row[1], &row[3]) with row = {6, 2, 8, 11}");
}

// TASK: 6
void test_swap_two_pigeonholes(void) {
    TEST_IGNORE();
    int32_t row[] = {6, 2};
    const int32_t expected[] = {2, 6};
    swap_pigeonholes(&row[0], &row[1]);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 2, "swap_pigeonholes(&row[0], &row[1]) with row = {6, 2}");
}

void test_swap_with_an_empty_pigeonhole(void) {
    TEST_IGNORE();
    int32_t row[] = {0, 14};
    const int32_t expected[] = {14, 0};
    swap_pigeonholes(&row[1], &row[0]);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 2, "swap_pigeonholes(&row[1], &row[0]) with row = {0, 14}");
}

void test_swap_two_equal_pigeonholes(void) {
    TEST_IGNORE();
    int32_t row[] = {5, 5};
    const int32_t expected[] = {5, 5};
    swap_pigeonholes(&row[0], &row[1]);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 2, "swap_pigeonholes(&row[0], &row[1]) with row = {5, 5}");
}

void test_swap_the_ends_of_a_row(void) {
    TEST_IGNORE();
    int32_t row[] = {1, 2, 3, 4};
    const int32_t expected[] = {4, 2, 3, 1};
    swap_pigeonholes(&row[0], &row[3]);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 4, "swap_pigeonholes(&row[0], &row[3]) with row = {1, 2, 3, 4}");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_first_of_a_busy_row);
    RUN_TEST(test_first_pigeonhole_is_empty);
    RUN_TEST(test_first_of_a_row_with_one_full_pigeonhole);
    RUN_TEST(test_third_of_a_busy_row);
    RUN_TEST(test_third_pigeonhole_is_empty);
    RUN_TEST(test_only_the_third_has_letters);
    RUN_TEST(test_third_of_a_row_of_different_counts);
    RUN_TEST(test_total_of_a_busy_row);
    RUN_TEST(test_total_of_an_empty_row);
    RUN_TEST(test_total_when_only_the_last_has_letters);
    RUN_TEST(test_total_of_a_row_after_the_christmas_rush);
    RUN_TEST(test_deliver_to_a_pigeonhole_with_letters);
    RUN_TEST(test_deliver_to_an_empty_pigeonhole);
    RUN_TEST(test_deliver_no_letters);
    RUN_TEST(test_deliver_to_the_middle_of_a_row);
    RUN_TEST(test_move_letters_to_a_pigeonhole_with_letters);
    RUN_TEST(test_move_letters_to_an_empty_pigeonhole);
    RUN_TEST(test_move_letters_from_an_empty_pigeonhole);
    RUN_TEST(test_move_letters_across_a_row);
    RUN_TEST(test_swap_two_pigeonholes);
    RUN_TEST(test_swap_with_an_empty_pigeonhole);
    RUN_TEST(test_swap_two_equal_pigeonholes);
    RUN_TEST(test_swap_the_ends_of_a_row);
    return UNITY_END();
}
