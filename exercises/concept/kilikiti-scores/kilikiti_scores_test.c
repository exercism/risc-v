#include "vendor/unity.h"

#include <stdint.h>

extern int32_t runs_by(const int32_t *scores, int32_t index);
extern int32_t total_runs(const int32_t *scores, int32_t count);
extern int32_t highest_score(const int32_t *scores, int32_t count);
extern int32_t count_ducks(const int32_t *scores, int32_t count);
extern void add_bonus(int32_t *overs, int32_t count, int32_t bonus);
extern int32_t first_century(const int32_t *scores, int32_t count);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_runs_by_the_third_batter(void) {
    const int32_t scores[] = {12, 40, 7, 23};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(7, runs_by(scores, 2), "runs_by({12, 40, 7, 23}, 2)");
}

void test_runs_by_the_opening_batter(void) {
    TEST_IGNORE();
    const int32_t scores[] = {12, 40, 7, 23};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(12, runs_by(scores, 0), "runs_by({12, 40, 7, 23}, 0)");
}

void test_runs_by_the_last_batter(void) {
    TEST_IGNORE();
    const int32_t scores[] = {12, 40, 7, 23};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(23, runs_by(scores, 3), "runs_by({12, 40, 7, 23}, 3)");
}

void test_runs_by_a_batter_out_for_a_duck(void) {
    TEST_IGNORE();
    const int32_t scores[] = {31, 0, 55};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, runs_by(scores, 1), "runs_by({31, 0, 55}, 1)");
}

void test_runs_by_the_tenth_batter(void) {
    TEST_IGNORE();
    const int32_t scores[] = {3, 8, 15, 0, 22, 9, 1, 4, 17, 64, 5};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(64, runs_by(scores, 9), "runs_by({3, 8, 15, 0, 22, 9, 1, 4, 17, 64, 5}, 9)");
}

// TASK: 2
void test_total_of_four_batters(void) {
    TEST_IGNORE();
    const int32_t scores[] = {12, 40, 7, 23};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(82, total_runs(scores, 4), "total_runs({12, 40, 7, 23}, 4)");
}

void test_total_of_one_batter(void) {
    TEST_IGNORE();
    const int32_t scores[] = {57};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(57, total_runs(scores, 1), "total_runs({57}, 1)");
}

void test_total_of_no_batters(void) {
    TEST_IGNORE();
    const int32_t scores[] = {25};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, total_runs(scores, 0), "total_runs({}, 0)");
}

void test_total_when_everyone_is_out_for_a_duck(void) {
    TEST_IGNORE();
    const int32_t scores[] = {0, 0, 0};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, total_runs(scores, 3), "total_runs({0, 0, 0}, 3)");
}

void test_total_of_a_long_innings(void) {
    TEST_IGNORE();
    const int32_t scores[] = {14, 3, 88, 0, 25, 6, 41, 9, 0, 12, 30, 7};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(235, total_runs(scores, 12), "total_runs({14, 3, 88, 0, 25, 6, 41, 9, 0, 12, 30, 7}, 12)");
}

// TASK: 3
void test_highest_of_four_batters(void) {
    TEST_IGNORE();
    const int32_t scores[] = {12, 40, 7, 23};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(40, highest_score(scores, 4), "highest_score({12, 40, 7, 23}, 4)");
}

void test_highest_of_one_batter(void) {
    TEST_IGNORE();
    const int32_t scores[] = {57};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(57, highest_score(scores, 1), "highest_score({57}, 1)");
}

void test_highest_is_the_first(void) {
    TEST_IGNORE();
    const int32_t scores[] = {93, 12, 5, 60};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(93, highest_score(scores, 4), "highest_score({93, 12, 5, 60}, 4)");
}

void test_highest_is_the_last(void) {
    TEST_IGNORE();
    const int32_t scores[] = {4, 18, 2, 26};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(26, highest_score(scores, 4), "highest_score({4, 18, 2, 26}, 4)");
}

void test_highest_when_all_are_ducks(void) {
    TEST_IGNORE();
    const int32_t scores[] = {0, 0, 0, 0};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, highest_score(scores, 4), "highest_score({0, 0, 0, 0}, 4)");
}

// TASK: 4
void test_three_ducks(void) {
    TEST_IGNORE();
    const int32_t scores[] = {0, 15, 0, 3, 0};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, count_ducks(scores, 5), "count_ducks({0, 15, 0, 3, 0}, 5)");
}

void test_no_ducks(void) {
    TEST_IGNORE();
    const int32_t scores[] = {12, 40, 7, 23};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, count_ducks(scores, 4), "count_ducks({12, 40, 7, 23}, 4)");
}

void test_all_ducks(void) {
    TEST_IGNORE();
    const int32_t scores[] = {0, 0, 0, 0};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, count_ducks(scores, 4), "count_ducks({0, 0, 0, 0}, 4)");
}

void test_ducks_in_an_empty_scorebook(void) {
    TEST_IGNORE();
    const int32_t scores[] = {0};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, count_ducks(scores, 0), "count_ducks({}, 0)");
}

void test_one_duck_at_the_end(void) {
    TEST_IGNORE();
    const int32_t scores[] = {5, 1, 9, 0};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, count_ducks(scores, 4), "count_ducks({5, 1, 9, 0}, 4)");
}

// TASK: 5
void test_bonus_of_one_run(void) {
    TEST_IGNORE();
    int32_t overs[] = {6, 2, 9};
    const int32_t expected[] = {7, 3, 10};
    add_bonus(overs, 3, 1);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, overs, 3, "add_bonus(overs, 3, 1) with overs = {6, 2, 9}");
}

void test_bonus_of_five_runs(void) {
    TEST_IGNORE();
    int32_t overs[] = {0, 12, 4, 7, 1, 3};
    const int32_t expected[] = {5, 17, 9, 12, 6, 8};
    add_bonus(overs, 6, 5);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, overs, 6, "add_bonus(overs, 6, 5) with overs = {0, 12, 4, 7, 1, 3}");
}

void test_bonus_of_no_runs(void) {
    TEST_IGNORE();
    int32_t overs[] = {6, 2, 9};
    const int32_t expected[] = {6, 2, 9};
    add_bonus(overs, 3, 0);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, overs, 3, "add_bonus(overs, 3, 0) with overs = {6, 2, 9}");
}

void test_bonus_with_no_overs_bowled(void) {
    TEST_IGNORE();
    int32_t overs[] = {9, 9};
    const int32_t expected[] = {9, 9};
    add_bonus(overs, 0, 4);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, overs, 2, "add_bonus(overs, 0, 4) with overs = {9, 9}");
}

void test_bonus_only_for_the_overs_counted(void) {
    TEST_IGNORE();
    int32_t overs[] = {6, 2, 9, 4};
    const int32_t expected[] = {9, 5, 9, 4};
    add_bonus(overs, 2, 3);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, overs, 4, "add_bonus(overs, 2, 3) with overs = {6, 2, 9, 4}");
}

// TASK: 6
void test_first_century_is_the_second_batter(void) {
    TEST_IGNORE();
    const int32_t scores[] = {45, 101, 18, 120};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, first_century(scores, 4), "first_century({45, 101, 18, 120}, 4)");
}

void test_no_century(void) {
    TEST_IGNORE();
    const int32_t scores[] = {45, 99, 18, 72};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, first_century(scores, 4), "first_century({45, 99, 18, 72}, 4)");
}

void test_century_of_exactly_one_hundred(void) {
    TEST_IGNORE();
    const int32_t scores[] = {100, 3, 8};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, first_century(scores, 3), "first_century({100, 3, 8}, 3)");
}

void test_first_century_is_the_last_batter(void) {
    TEST_IGNORE();
    const int32_t scores[] = {12, 40, 7, 23, 150};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, first_century(scores, 5), "first_century({12, 40, 7, 23, 150}, 5)");
}

void test_no_century_in_an_empty_scorebook(void) {
    TEST_IGNORE();
    const int32_t scores[] = {150};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, first_century(scores, 0), "first_century({}, 0)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_runs_by_the_third_batter);
    RUN_TEST(test_runs_by_the_opening_batter);
    RUN_TEST(test_runs_by_the_last_batter);
    RUN_TEST(test_runs_by_a_batter_out_for_a_duck);
    RUN_TEST(test_runs_by_the_tenth_batter);
    RUN_TEST(test_total_of_four_batters);
    RUN_TEST(test_total_of_one_batter);
    RUN_TEST(test_total_of_no_batters);
    RUN_TEST(test_total_when_everyone_is_out_for_a_duck);
    RUN_TEST(test_total_of_a_long_innings);
    RUN_TEST(test_highest_of_four_batters);
    RUN_TEST(test_highest_of_one_batter);
    RUN_TEST(test_highest_is_the_first);
    RUN_TEST(test_highest_is_the_last);
    RUN_TEST(test_highest_when_all_are_ducks);
    RUN_TEST(test_three_ducks);
    RUN_TEST(test_no_ducks);
    RUN_TEST(test_all_ducks);
    RUN_TEST(test_ducks_in_an_empty_scorebook);
    RUN_TEST(test_one_duck_at_the_end);
    RUN_TEST(test_bonus_of_one_run);
    RUN_TEST(test_bonus_of_five_runs);
    RUN_TEST(test_bonus_of_no_runs);
    RUN_TEST(test_bonus_with_no_overs_bowled);
    RUN_TEST(test_bonus_only_for_the_overs_counted);
    RUN_TEST(test_first_century_is_the_second_batter);
    RUN_TEST(test_no_century);
    RUN_TEST(test_century_of_exactly_one_hundred);
    RUN_TEST(test_first_century_is_the_last_batter);
    RUN_TEST(test_no_century_in_an_empty_scorebook);
    return UNITY_END();
}
