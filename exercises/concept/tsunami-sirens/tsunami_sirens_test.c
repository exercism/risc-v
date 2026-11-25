#include "vendor/unity.h"

#include <stdint.h>

extern int32_t larger(int32_t a, int32_t b);
extern int32_t tide_change(int32_t earlier, int32_t later);
extern int32_t alert_level(int32_t height_cm);
extern int32_t should_evacuate(int32_t level, int32_t strong_quake);
extern int32_t siren_minutes(int32_t level);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_the_second_gauge_is_higher(void) {
    TEST_ASSERT_EQUAL_INT32_MESSAGE(30, larger(12, 30), "larger(12, 30)");
}

void test_the_first_gauge_is_higher(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(45, larger(45, 20), "larger(45, 20)");
}

void test_both_gauges_agree(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(17, larger(17, 17), "larger(17, 17)");
}

void test_both_readings_are_negative(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-2, larger(-5, -2), "larger(-5, -2)");
}

void test_one_reading_is_negative(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, larger(-40, 3), "larger(-40, 3)");
}

// TASK: 2
void test_the_sea_rises(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(30, tide_change(120, 150), "tide_change(120, 150)");
}

void test_the_sea_falls(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(30, tide_change(150, 120), "tide_change(150, 120)");
}

void test_the_sea_stays_level(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, tide_change(80, 80), "tide_change(80, 80)");
}

void test_the_sea_rises_from_below_normal(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(55, tide_change(-20, 35), "tide_change(-20, 35)");
}

void test_the_sea_falls_to_below_normal(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(55, tide_change(35, -20), "tide_change(35, -20)");
}

// TASK: 3
void test_just_below_level_1_is_no_threat(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, alert_level(29), "alert_level(29)");
}

void test_exactly_30_cm_is_level_1(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, alert_level(30), "alert_level(30)");
}

void test_exactly_100_cm_is_level_2(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, alert_level(100), "alert_level(100)");
}

void test_just_below_level_3_is_level_2(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, alert_level(299), "alert_level(299)");
}

void test_exactly_300_cm_is_level_3(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, alert_level(300), "alert_level(300)");
}

// TASK: 4
void test_no_alert_and_no_quake_means_stay(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, should_evacuate(0, 0), "should_evacuate(0, 0)");
}

void test_level_1_without_a_quake_means_stay(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, should_evacuate(1, 0), "should_evacuate(1, 0)");
}

void test_level_2_means_evacuate(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, should_evacuate(2, 0), "should_evacuate(2, 0)");
}

void test_level_3_means_evacuate(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, should_evacuate(3, 0), "should_evacuate(3, 0)");
}

void test_a_strong_quake_means_evacuate_without_waiting(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, should_evacuate(0, 1), "should_evacuate(0, 1)");
}

// TASK: 5
void test_level_0_is_silent(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, siren_minutes(0), "siren_minutes(0)");
}

void test_level_1_sounds_for_2_minutes(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, siren_minutes(1), "siren_minutes(1)");
}

void test_level_2_sounds_for_5_minutes(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(5, siren_minutes(2), "siren_minutes(2)");
}

void test_level_3_sounds_for_10_minutes(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(10, siren_minutes(3), "siren_minutes(3)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_the_second_gauge_is_higher);
    RUN_TEST(test_the_first_gauge_is_higher);
    RUN_TEST(test_both_gauges_agree);
    RUN_TEST(test_both_readings_are_negative);
    RUN_TEST(test_one_reading_is_negative);
    RUN_TEST(test_the_sea_rises);
    RUN_TEST(test_the_sea_falls);
    RUN_TEST(test_the_sea_stays_level);
    RUN_TEST(test_the_sea_rises_from_below_normal);
    RUN_TEST(test_the_sea_falls_to_below_normal);
    RUN_TEST(test_just_below_level_1_is_no_threat);
    RUN_TEST(test_exactly_30_cm_is_level_1);
    RUN_TEST(test_exactly_100_cm_is_level_2);
    RUN_TEST(test_just_below_level_3_is_level_2);
    RUN_TEST(test_exactly_300_cm_is_level_3);
    RUN_TEST(test_no_alert_and_no_quake_means_stay);
    RUN_TEST(test_level_1_without_a_quake_means_stay);
    RUN_TEST(test_level_2_means_evacuate);
    RUN_TEST(test_level_3_means_evacuate);
    RUN_TEST(test_a_strong_quake_means_evacuate_without_waiting);
    RUN_TEST(test_level_0_is_silent);
    RUN_TEST(test_level_1_sounds_for_2_minutes);
    RUN_TEST(test_level_2_sounds_for_5_minutes);
    RUN_TEST(test_level_3_sounds_for_10_minutes);
    return UNITY_END();
}
