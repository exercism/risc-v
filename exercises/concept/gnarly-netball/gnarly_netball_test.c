#include "vendor/unity.h"

#include <stdint.h>

extern int32_t ladder_points(int32_t wins, int32_t draws);
extern int32_t goals_per_quarter(int32_t goals);
extern int32_t average_margin(int32_t total_margin, int32_t games);
extern int32_t oranges_left(int32_t oranges, int32_t players);
extern int32_t buses_needed(int32_t people, int32_t seats);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_three_wins_and_a_draw(void) {
    TEST_ASSERT_EQUAL_INT32_MESSAGE(14, ladder_points(3, 1), "ladder_points(3, 1)");
}

void test_only_wins(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(20, ladder_points(5, 0), "ladder_points(5, 0)");
}

void test_only_draws(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(6, ladder_points(0, 3), "ladder_points(0, 3)");
}

void test_no_wins_or_draws(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, ladder_points(0, 0), "ladder_points(0, 0)");
}

void test_a_whole_season(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(50, ladder_points(11, 3), "ladder_points(11, 3)");
}

// TASK: 2
void test_goals_that_do_not_share_evenly(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(9, goals_per_quarter(38), "goals_per_quarter(38)");
}

void test_goals_that_share_evenly(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(12, goals_per_quarter(48), "goals_per_quarter(48)");
}

void test_fewer_goals_than_quarters(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, goals_per_quarter(3), "goals_per_quarter(3)");
}

void test_no_goals(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, goals_per_quarter(0), "goals_per_quarter(0)");
}

// TASK: 3
void test_a_negative_margin_moves_towards_zero(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-3, average_margin(-7, 2), "average_margin(-7, 2)");
}

void test_a_positive_margin_moves_towards_zero(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, average_margin(7, 2), "average_margin(7, 2)");
}

void test_a_negative_margin_that_shares_evenly(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-4, average_margin(-12, 3), "average_margin(-12, 3)");
}

void test_a_small_negative_margin_becomes_zero(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, average_margin(-3, 5), "average_margin(-3, 5)");
}

void test_a_single_game(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(9, average_margin(9, 1), "average_margin(9, 1)");
}

// TASK: 4
void test_some_oranges_left_over(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, oranges_left(30, 7), "oranges_left(30, 7)");
}

void test_no_oranges_left_over(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, oranges_left(28, 7), "oranges_left(28, 7)");
}

void test_fewer_oranges_than_players(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(5, oranges_left(5, 7), "oranges_left(5, 7)");
}

void test_two_teams_share_the_box(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(6, oranges_left(34, 14), "oranges_left(34, 14)");
}

void test_no_players_to_share_with(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(12, oranges_left(12, 0), "oranges_left(12, 0)");
}

// TASK: 5
void test_one_person_more_than_fills_two_buses(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, buses_needed(41, 20), "buses_needed(41, 20)");
}

void test_exactly_two_full_buses(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, buses_needed(40, 20), "buses_needed(40, 20)");
}

void test_a_single_traveller(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, buses_needed(1, 20), "buses_needed(1, 20)");
}

void test_one_full_bus(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, buses_needed(7, 7), "buses_needed(7, 7)");
}

void test_nobody_travelling(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, buses_needed(0, 20), "buses_needed(0, 20)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_three_wins_and_a_draw);
    RUN_TEST(test_only_wins);
    RUN_TEST(test_only_draws);
    RUN_TEST(test_no_wins_or_draws);
    RUN_TEST(test_a_whole_season);
    RUN_TEST(test_goals_that_do_not_share_evenly);
    RUN_TEST(test_goals_that_share_evenly);
    RUN_TEST(test_fewer_goals_than_quarters);
    RUN_TEST(test_no_goals);
    RUN_TEST(test_a_negative_margin_moves_towards_zero);
    RUN_TEST(test_a_positive_margin_moves_towards_zero);
    RUN_TEST(test_a_negative_margin_that_shares_evenly);
    RUN_TEST(test_a_small_negative_margin_becomes_zero);
    RUN_TEST(test_a_single_game);
    RUN_TEST(test_some_oranges_left_over);
    RUN_TEST(test_no_oranges_left_over);
    RUN_TEST(test_fewer_oranges_than_players);
    RUN_TEST(test_two_teams_share_the_box);
    RUN_TEST(test_no_players_to_share_with);
    RUN_TEST(test_one_person_more_than_fills_two_buses);
    RUN_TEST(test_exactly_two_full_buses);
    RUN_TEST(test_a_single_traveller);
    RUN_TEST(test_one_full_bus);
    RUN_TEST(test_nobody_travelling);
    return UNITY_END();
}
