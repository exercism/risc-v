#include "vendor/unity.h"

#include <stdint.h>

extern int32_t hatched(uint32_t nest);
extern int32_t first_hatched(uint32_t nest);
extern int32_t last_hatched(uint32_t nest);
extern int32_t still_waiting(uint32_t eggs, uint32_t nest);
extern int32_t clamp_reading(int32_t reading, int32_t low, int32_t high);
extern uint32_t rotate_watch(uint32_t rota, int32_t nights);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_three_eggs_have_hatched(void) {
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, hatched(0x00000019), "hatched(0x00000019)");
}

void test_an_empty_nest(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, hatched(0x00000000), "hatched(0x00000000)");
}

void test_every_egg_has_hatched(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(32, hatched(0xFFFFFFFF), "hatched(0xFFFFFFFF)");
}

void test_only_the_top_egg_has_hatched(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, hatched(0x80000000), "hatched(0x80000000)");
}

void test_half_the_eggs_have_hatched(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(16, hatched(0x0F0F0F0F), "hatched(0x0F0F0F0F)");
}

// TASK: 2
void test_the_first_hatchling_is_in_place_6(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(6, first_hatched(0x00000140), "first_hatched(0x00000140)");
}

void test_the_first_hatchling_is_in_place_0(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, first_hatched(0x00000001), "first_hatched(0x00000001)");
}

void test_the_first_hatchling_is_in_place_31(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(31, first_hatched(0x80000000), "first_hatched(0x80000000)");
}

void test_no_first_hatchling_yet(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, first_hatched(0x00000000), "first_hatched(0x00000000)");
}

void test_the_first_of_a_fully_hatched_nest(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, first_hatched(0xFFFFFFFF), "first_hatched(0xFFFFFFFF)");
}

// TASK: 3
void test_the_last_hatchling_is_in_place_8(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(8, last_hatched(0x00000140), "last_hatched(0x00000140)");
}

void test_only_the_first_egg_has_hatched(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, last_hatched(0x00000001), "last_hatched(0x00000001)");
}

void test_the_top_egg_has_hatched(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(31, last_hatched(0x80000000), "last_hatched(0x80000000)");
}

void test_no_last_hatchling_yet(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, last_hatched(0x00000000), "last_hatched(0x00000000)");
}

void test_the_last_of_a_fully_hatched_nest(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(31, last_hatched(0xFFFFFFFF), "last_hatched(0xFFFFFFFF)");
}

// TASK: 4
void test_four_eggs_are_still_waiting(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, still_waiting(0x000000FF, 0x0000000F), "still_waiting(0x000000FF, 0x0000000F)");
}

void test_a_full_nest_with_no_hatchlings(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(32, still_waiting(0xFFFFFFFF, 0x00000000), "still_waiting(0xFFFFFFFF, 0x00000000)");
}

void test_a_full_nest_has_all_hatched(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, still_waiting(0xFFFFFFFF, 0xFFFFFFFF), "still_waiting(0xFFFFFFFF, 0xFFFFFFFF)");
}

void test_crab_holes_are_ignored(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(15, still_waiting(0x0000FFFF, 0x80000001), "still_waiting(0x0000FFFF, 0x80000001)");
}

void test_a_nest_with_no_eggs(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, still_waiting(0x00000000, 0xFFFFFFFF), "still_waiting(0x00000000, 0xFFFFFFFF)");
}

// TASK: 5
void test_a_reading_within_the_range(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(12, clamp_reading(12, -50, 50), "clamp_reading(12, -50, 50)");
}

void test_a_reading_below_the_range(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-50, clamp_reading(-73, -50, 50), "clamp_reading(-73, -50, 50)");
}

void test_a_reading_above_the_range(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(50, clamp_reading(80, -50, 50), "clamp_reading(80, -50, 50)");
}

void test_a_reading_at_the_bottom_of_the_range(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-50, clamp_reading(-50, -50, 50), "clamp_reading(-50, -50, 50)");
}

void test_the_most_negative_reading(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-50, clamp_reading(INT32_MIN, -50, 50), "clamp_reading(INT32_MIN, -50, 50)");
}

// TASK: 6
void test_two_volunteers_move_up_one(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x00000006, rotate_watch(0x00000003, 1), "rotate_watch(0x00000003, 1)");
}

void test_volunteer_31_passes_back_to_volunteer_0(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x00000003, rotate_watch(0x80000001, 1), "rotate_watch(0x80000001, 1)");
}

void test_thirty_nights_later(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0xC0000003, rotate_watch(0x0000000F, 30), "rotate_watch(0x0000000F, 30)");
}

void test_thirty_two_nights_bring_the_rota_back(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x12345678, rotate_watch(0x12345678, 32), "rotate_watch(0x12345678, 32)");
}

void test_thirty_three_nights_is_one_round_and_one_more(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x00000002, rotate_watch(0x00000001, 33), "rotate_watch(0x00000001, 33)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_three_eggs_have_hatched);
    RUN_TEST(test_an_empty_nest);
    RUN_TEST(test_every_egg_has_hatched);
    RUN_TEST(test_only_the_top_egg_has_hatched);
    RUN_TEST(test_half_the_eggs_have_hatched);
    RUN_TEST(test_the_first_hatchling_is_in_place_6);
    RUN_TEST(test_the_first_hatchling_is_in_place_0);
    RUN_TEST(test_the_first_hatchling_is_in_place_31);
    RUN_TEST(test_no_first_hatchling_yet);
    RUN_TEST(test_the_first_of_a_fully_hatched_nest);
    RUN_TEST(test_the_last_hatchling_is_in_place_8);
    RUN_TEST(test_only_the_first_egg_has_hatched);
    RUN_TEST(test_the_top_egg_has_hatched);
    RUN_TEST(test_no_last_hatchling_yet);
    RUN_TEST(test_the_last_of_a_fully_hatched_nest);
    RUN_TEST(test_four_eggs_are_still_waiting);
    RUN_TEST(test_a_full_nest_with_no_hatchlings);
    RUN_TEST(test_a_full_nest_has_all_hatched);
    RUN_TEST(test_crab_holes_are_ignored);
    RUN_TEST(test_a_nest_with_no_eggs);
    RUN_TEST(test_a_reading_within_the_range);
    RUN_TEST(test_a_reading_below_the_range);
    RUN_TEST(test_a_reading_above_the_range);
    RUN_TEST(test_a_reading_at_the_bottom_of_the_range);
    RUN_TEST(test_the_most_negative_reading);
    RUN_TEST(test_two_volunteers_move_up_one);
    RUN_TEST(test_volunteer_31_passes_back_to_volunteer_0);
    RUN_TEST(test_thirty_nights_later);
    RUN_TEST(test_thirty_two_nights_bring_the_rota_back);
    RUN_TEST(test_thirty_three_nights_is_one_round_and_one_more);
    return UNITY_END();
}
