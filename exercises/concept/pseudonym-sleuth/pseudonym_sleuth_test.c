#include "vendor/unity.h"

#include <stdint.h>

extern void split_clue(int32_t first, int32_t second, int32_t *sum, int32_t *difference);
extern int32_t desk_neighbours(int32_t desk, int32_t *left);
extern void case_summary(const int32_t *clues, int32_t *total, int32_t *latest);
extern int32_t update_tally(int32_t *tally, int32_t found);
extern void unscramble(int32_t *first, int32_t *second);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_split_a_clue_with_the_larger_number_first(void) {
    int32_t sum = -1;
    int32_t difference = -1;
    split_clue(12, 5, &sum, &difference);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(17, sum, "sum after split_clue(12, 5, &sum, &difference)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(7, difference, "difference after split_clue(12, 5, &sum, &difference)");
}

void test_split_a_clue_with_the_smaller_number_first(void) {
    TEST_IGNORE();
    int32_t sum = -1;
    int32_t difference = -1;
    split_clue(5, 12, &sum, &difference);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(17, sum, "sum after split_clue(5, 12, &sum, &difference)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-7, difference, "difference after split_clue(5, 12, &sum, &difference)");
}

void test_split_a_clue_of_two_equal_numbers(void) {
    TEST_IGNORE();
    int32_t sum = -1;
    int32_t difference = -1;
    split_clue(9, 9, &sum, &difference);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(18, sum, "sum after split_clue(9, 9, &sum, &difference)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, difference, "difference after split_clue(9, 9, &sum, &difference)");
}

void test_split_a_clue_of_zeros(void) {
    TEST_IGNORE();
    int32_t sum = -1;
    int32_t difference = -1;
    split_clue(0, 0, &sum, &difference);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, sum, "sum after split_clue(0, 0, &sum, &difference)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, difference, "difference after split_clue(0, 0, &sum, &difference)");
}

void test_split_a_clue_with_a_negative_number(void) {
    TEST_IGNORE();
    int32_t sum = -1;
    int32_t difference = -1;
    split_clue(-4, 6, &sum, &difference);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, sum, "sum after split_clue(-4, 6, &sum, &difference)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-10, difference, "difference after split_clue(-4, 6, &sum, &difference)");
}

// TASK: 2
void test_neighbours_of_a_desk_in_the_middle(void) {
    TEST_IGNORE();
    int32_t left = -1;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(15, desk_neighbours(14, &left), "desk_neighbours(14, &left)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(13, left, "left after desk_neighbours(14, &left)");
}

void test_neighbours_of_the_first_desk(void) {
    TEST_IGNORE();
    int32_t left = -1;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, desk_neighbours(1, &left), "desk_neighbours(1, &left)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, left, "left after desk_neighbours(1, &left)");
}

void test_neighbours_of_a_desk_near_the_end(void) {
    TEST_IGNORE();
    int32_t left = -1;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(40, desk_neighbours(39, &left), "desk_neighbours(39, &left)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(38, left, "left after desk_neighbours(39, &left)");
}

void test_neighbours_of_desk_100(void) {
    TEST_IGNORE();
    int32_t left = -1;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(101, desk_neighbours(100, &left), "desk_neighbours(100, &left)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(99, left, "left after desk_neighbours(100, &left)");
}

// TASK: 3
void test_summary_of_three_different_clues(void) {
    TEST_IGNORE();
    const int32_t clues[] = {3, 8, 6};
    int32_t total = -1;
    int32_t latest = -1;
    case_summary(clues, &total, &latest);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(17, total, "total after case_summary(clues, &total, &latest) with clues = {3, 8, 6}");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(6, latest, "latest after case_summary(clues, &total, &latest) with clues = {3, 8, 6}");
}

void test_summary_of_three_zero_clues(void) {
    TEST_IGNORE();
    const int32_t clues[] = {0, 0, 0};
    int32_t total = -1;
    int32_t latest = -1;
    case_summary(clues, &total, &latest);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, total, "total after case_summary(clues, &total, &latest) with clues = {0, 0, 0}");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, latest, "latest after case_summary(clues, &total, &latest) with clues = {0, 0, 0}");
}

void test_summary_when_the_latest_clue_is_zero(void) {
    TEST_IGNORE();
    const int32_t clues[] = {10, 0, 0};
    int32_t total = -1;
    int32_t latest = -1;
    case_summary(clues, &total, &latest);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(10, total, "total after case_summary(clues, &total, &latest) with clues = {10, 0, 0}");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, latest, "latest after case_summary(clues, &total, &latest) with clues = {10, 0, 0}");
}

void test_summary_of_three_equal_clues(void) {
    TEST_IGNORE();
    const int32_t clues[] = {7, 7, 7};
    int32_t total = -1;
    int32_t latest = -1;
    case_summary(clues, &total, &latest);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(21, total, "total after case_summary(clues, &total, &latest) with clues = {7, 7, 7}");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(7, latest, "latest after case_summary(clues, &total, &latest) with clues = {7, 7, 7}");
}

void test_summary_with_a_negative_clue(void) {
    TEST_IGNORE();
    const int32_t clues[] = {20, -5, 2};
    int32_t total = -1;
    int32_t latest = -1;
    case_summary(clues, &total, &latest);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(17, total, "total after case_summary(clues, &total, &latest) with clues = {20, -5, 2}");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, latest, "latest after case_summary(clues, &total, &latest) with clues = {20, -5, 2}");
}

// TASK: 4
void test_tally_grows(void) {
    TEST_IGNORE();
    int32_t tally = 9;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(9, update_tally(&tally, 4), "update_tally(&tally, 4) with tally = 9");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(13, tally, "tally after update_tally(&tally, 4) with tally = 9");
}

void test_first_clues_found(void) {
    TEST_IGNORE();
    int32_t tally = 0;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, update_tally(&tally, 3), "update_tally(&tally, 3) with tally = 0");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, tally, "tally after update_tally(&tally, 3) with tally = 0");
}

void test_no_new_clues(void) {
    TEST_IGNORE();
    int32_t tally = 12;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(12, update_tally(&tally, 0), "update_tally(&tally, 0) with tally = 12");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(12, tally, "tally after update_tally(&tally, 0) with tally = 12");
}

void test_one_more_clue(void) {
    TEST_IGNORE();
    int32_t tally = 25;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(25, update_tally(&tally, 1), "update_tally(&tally, 1) with tally = 25");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(26, tally, "tally after update_tally(&tally, 1) with tally = 25");
}

// TASK: 5
void test_unscramble_a_message(void) {
    TEST_IGNORE();
    int32_t first = 7;
    int32_t second = 3;
    unscramble(&first, &second);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(10, first, "first after unscramble(&first, &second) with first = 7, second = 3");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, second, "second after unscramble(&first, &second) with first = 7, second = 3");
}

void test_unscramble_a_message_with_the_smaller_number_first(void) {
    TEST_IGNORE();
    int32_t first = 3;
    int32_t second = 7;
    unscramble(&first, &second);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(10, first, "first after unscramble(&first, &second) with first = 3, second = 7");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-4, second, "second after unscramble(&first, &second) with first = 3, second = 7");
}

void test_unscramble_when_the_second_number_is_zero(void) {
    TEST_IGNORE();
    int32_t first = 15;
    int32_t second = 0;
    unscramble(&first, &second);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(15, first, "first after unscramble(&first, &second) with first = 15, second = 0");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(15, second, "second after unscramble(&first, &second) with first = 15, second = 0");
}

void test_unscramble_two_equal_numbers(void) {
    TEST_IGNORE();
    int32_t first = 8;
    int32_t second = 8;
    unscramble(&first, &second);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(16, first, "first after unscramble(&first, &second) with first = 8, second = 8");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, second, "second after unscramble(&first, &second) with first = 8, second = 8");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_split_a_clue_with_the_larger_number_first);
    RUN_TEST(test_split_a_clue_with_the_smaller_number_first);
    RUN_TEST(test_split_a_clue_of_two_equal_numbers);
    RUN_TEST(test_split_a_clue_of_zeros);
    RUN_TEST(test_split_a_clue_with_a_negative_number);
    RUN_TEST(test_neighbours_of_a_desk_in_the_middle);
    RUN_TEST(test_neighbours_of_the_first_desk);
    RUN_TEST(test_neighbours_of_a_desk_near_the_end);
    RUN_TEST(test_neighbours_of_desk_100);
    RUN_TEST(test_summary_of_three_different_clues);
    RUN_TEST(test_summary_of_three_zero_clues);
    RUN_TEST(test_summary_when_the_latest_clue_is_zero);
    RUN_TEST(test_summary_of_three_equal_clues);
    RUN_TEST(test_summary_with_a_negative_clue);
    RUN_TEST(test_tally_grows);
    RUN_TEST(test_first_clues_found);
    RUN_TEST(test_no_new_clues);
    RUN_TEST(test_one_more_clue);
    RUN_TEST(test_unscramble_a_message);
    RUN_TEST(test_unscramble_a_message_with_the_smaller_number_first);
    RUN_TEST(test_unscramble_when_the_second_number_is_zero);
    RUN_TEST(test_unscramble_two_equal_numbers);
    return UNITY_END();
}
