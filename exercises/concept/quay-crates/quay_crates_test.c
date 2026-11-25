#include "vendor/unity.h"

#include <stdint.h>

extern int32_t hold_capacity(void);
extern int32_t crates_with_spares(int32_t ordered);
extern int32_t space_left(int32_t loaded);
extern int32_t crates_on_board(int32_t arriving, int32_t unloaded, int32_t loaded);
extern int32_t checked_count(int32_t clerk, int32_t crane);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_the_hold_carries_120_crates(void) {
    TEST_ASSERT_EQUAL_INT32(120, hold_capacity());
}

// TASK: 2
void test_a_small_order_gets_3_spares(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(13, crates_with_spares(10), "crates_with_spares(10)");
}

void test_a_large_order_gets_3_spares(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(100, crates_with_spares(97), "crates_with_spares(97)");
}

void test_an_empty_order_still_gets_3_spares(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, crates_with_spares(0), "crates_with_spares(0)");
}

// TASK: 3
void test_space_left_in_a_part_loaded_hold(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(75, space_left(45), "space_left(45)");
}

void test_space_left_in_an_empty_hold(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(120, space_left(0), "space_left(0)");
}

void test_no_space_left_in_a_full_hold(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, space_left(120), "space_left(120)");
}

void test_space_left_with_one_crate_to_go(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, space_left(119), "space_left(119)");
}

// TASK: 4
void test_more_unloaded_than_loaded(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(75, crates_on_board(80, 30, 25), "crates_on_board(80, 30, 25)");
}

void test_more_loaded_than_unloaded(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(64, crates_on_board(50, 6, 20), "crates_on_board(50, 6, 20)");
}

void test_arriving_empty(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(42, crates_on_board(0, 0, 42), "crates_on_board(0, 0, 42)");
}

void test_unloading_everything(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, crates_on_board(37, 37, 0), "crates_on_board(37, 37, 0)");
}

// TASK: 5
void test_the_crane_counted_one_more(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(42, checked_count(41, 42), "checked_count(41, 42)");
}

void test_the_crane_counted_fewer(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(17, checked_count(71, 17), "checked_count(71, 17)");
}

void test_the_counts_agree(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(58, checked_count(58, 58), "checked_count(58, 58)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_the_hold_carries_120_crates);
    RUN_TEST(test_a_small_order_gets_3_spares);
    RUN_TEST(test_a_large_order_gets_3_spares);
    RUN_TEST(test_an_empty_order_still_gets_3_spares);
    RUN_TEST(test_space_left_in_a_part_loaded_hold);
    RUN_TEST(test_space_left_in_an_empty_hold);
    RUN_TEST(test_no_space_left_in_a_full_hold);
    RUN_TEST(test_space_left_with_one_crate_to_go);
    RUN_TEST(test_more_unloaded_than_loaded);
    RUN_TEST(test_more_loaded_than_unloaded);
    RUN_TEST(test_arriving_empty);
    RUN_TEST(test_unloading_everything);
    RUN_TEST(test_the_crane_counted_one_more);
    RUN_TEST(test_the_crane_counted_fewer);
    RUN_TEST(test_the_counts_agree);
    return UNITY_END();
}
