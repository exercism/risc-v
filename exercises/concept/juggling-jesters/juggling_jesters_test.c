#include "vendor/unity.h"

#include <stdint.h>

extern int32_t catch_count(int32_t balls, int32_t rounds);
extern int32_t act_score(int32_t balls, int32_t rounds);
extern int32_t encore(int32_t balls, int32_t rounds);
extern int32_t show_total(int32_t acts, int32_t rounds);
extern int32_t crowd_rating(int32_t first, int32_t second, int32_t third);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_three_balls_for_five_rounds(void) {
    TEST_ASSERT_EQUAL_INT32_MESSAGE(15, catch_count(3, 5), "catch_count(3, 5)");
}

void test_no_balls_means_no_catches(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, catch_count(0, 10), "catch_count(0, 10)");
}

void test_one_round_catches_each_ball_once(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(7, catch_count(7, 1), "catch_count(7, 1)");
}

void test_four_balls_for_a_dozen_rounds(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(48, catch_count(4, 12), "catch_count(4, 12)");
}

// TASK: 2
void test_an_act_with_three_balls(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(40, act_score(3, 5), "act_score(3, 5)");
}

void test_a_bow_alone_scores_10(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(10, act_score(0, 8), "act_score(0, 8)");
}

void test_a_single_catch(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(12, act_score(1, 1), "act_score(1, 1)");
}

void test_a_long_act_with_five_balls(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(70, act_score(5, 6), "act_score(5, 6)");
}

// TASK: 3
void test_an_encore_with_one_more_ball(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(35, encore(3, 5), "encore(3, 5)");
}

void test_an_encore_after_juggling_nothing(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, encore(0, 4), "encore(0, 4)");
}

void test_no_rounds_means_no_catches(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, encore(2, 0), "encore(2, 0)");
}

void test_a_big_encore(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(130, encore(6, 10), "encore(6, 10)");
}

// TASK: 4
void test_a_show_of_three_acts(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(24, show_total(3, 4), "show_total(3, 4)");
}

void test_a_show_with_no_acts(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, show_total(0, 5), "show_total(0, 5)");
}

void test_a_show_of_one_act(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(7, show_total(1, 7), "show_total(1, 7)");
}

void test_a_show_of_five_short_acts(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(30, show_total(5, 2), "show_total(5, 2)");
}

void test_a_show_of_four_long_acts(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(100, show_total(4, 10), "show_total(4, 10)");
}

// TASK: 5
void test_one_card_too_high_and_one_too_low(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(17, crowd_rating(7, 12, -3), "crowd_rating(7, 12, -3)");
}

void test_three_fair_cards(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(19, crowd_rating(5, 6, 8), "crowd_rating(5, 6, 8)");
}

void test_three_perfect_tens(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(30, crowd_rating(10, 10, 10), "crowd_rating(10, 10, 10)");
}

void test_every_card_too_high(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(30, crowd_rating(15, 20, 99), "crowd_rating(15, 20, 99)");
}

void test_every_card_zero_or_below(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, crowd_rating(-1, -5, 0), "crowd_rating(-1, -5, 0)");
}

void test_only_the_last_card_counts(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, crowd_rating(0, 0, 4), "crowd_rating(0, 0, 4)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_three_balls_for_five_rounds);
    RUN_TEST(test_no_balls_means_no_catches);
    RUN_TEST(test_one_round_catches_each_ball_once);
    RUN_TEST(test_four_balls_for_a_dozen_rounds);
    RUN_TEST(test_an_act_with_three_balls);
    RUN_TEST(test_a_bow_alone_scores_10);
    RUN_TEST(test_a_single_catch);
    RUN_TEST(test_a_long_act_with_five_balls);
    RUN_TEST(test_an_encore_with_one_more_ball);
    RUN_TEST(test_an_encore_after_juggling_nothing);
    RUN_TEST(test_no_rounds_means_no_catches);
    RUN_TEST(test_a_big_encore);
    RUN_TEST(test_a_show_of_three_acts);
    RUN_TEST(test_a_show_with_no_acts);
    RUN_TEST(test_a_show_of_one_act);
    RUN_TEST(test_a_show_of_five_short_acts);
    RUN_TEST(test_a_show_of_four_long_acts);
    RUN_TEST(test_one_card_too_high_and_one_too_low);
    RUN_TEST(test_three_fair_cards);
    RUN_TEST(test_three_perfect_tens);
    RUN_TEST(test_every_card_too_high);
    RUN_TEST(test_every_card_zero_or_below);
    RUN_TEST(test_only_the_last_card_counts);
    return UNITY_END();
}
