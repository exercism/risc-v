#include "vendor/unity.h"

#include <stdint.h>

extern int32_t reading_at(const int16_t *log, int32_t index);
extern int32_t coolest(const int16_t *log, int32_t count);
extern int32_t total_change(const int16_t *changes, int32_t count);
extern int32_t deepest(const uint16_t *depths, int32_t count);
extern void calibrate(int16_t *log, int32_t count, int32_t offset);
extern int32_t count_bleaching_risk(const int16_t *log, int32_t count, int32_t threshold);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_reading_at_a_cool_hour(void) {
    const int16_t log[] = {120, -35, 210, 88};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-35, reading_at(log, 1), "reading_at({120, -35, 210, 88}, 1)");
}

void test_reading_at_the_first_hour(void) {
    TEST_IGNORE();
    const int16_t log[] = {120, -35, 210, 88};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(120, reading_at(log, 0), "reading_at({120, -35, 210, 88}, 0)");
}

void test_reading_at_the_last_hour(void) {
    TEST_IGNORE();
    const int16_t log[] = {120, -35, 210, 88};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(88, reading_at(log, 3), "reading_at({120, -35, 210, 88}, 3)");
}

void test_reading_at_the_lowest_possible_value(void) {
    TEST_IGNORE();
    const int16_t log[] = {5, 32767, -32768, -1};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-32768, reading_at(log, 2), "reading_at({5, 32767, -32768, -1}, 2)");
}

void test_reading_at_the_highest_possible_value(void) {
    TEST_IGNORE();
    const int16_t log[] = {5, 32767, -32768, -1};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(32767, reading_at(log, 1), "reading_at({5, 32767, -32768, -1}, 1)");
}

// TASK: 2
void test_coolest_of_five_readings(void) {
    TEST_IGNORE();
    const int16_t log[] = {120, -35, 210, -80, 88};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-80, coolest(log, 5), "coolest({120, -35, 210, -80, 88}, 5)");
}

void test_coolest_of_one_reading(void) {
    TEST_IGNORE();
    const int16_t log[] = {-150};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-150, coolest(log, 1), "coolest({-150}, 1)");
}

void test_coolest_when_all_are_warm(void) {
    TEST_IGNORE();
    const int16_t log[] = {310, 95, 140, 260};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(95, coolest(log, 4), "coolest({310, 95, 140, 260}, 4)");
}

void test_coolest_is_the_last_reading(void) {
    TEST_IGNORE();
    const int16_t log[] = {-10, -20, -30, -400};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-400, coolest(log, 4), "coolest({-10, -20, -30, -400}, 4)");
}

void test_coolest_is_the_first_reading(void) {
    TEST_IGNORE();
    const int16_t log[] = {-32768, 0, 32767, -1};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-32768, coolest(log, 4), "coolest({-32768, 0, 32767, -1}, 4)");
}

// TASK: 3
void test_total_of_four_changes(void) {
    TEST_IGNORE();
    const int16_t changes[] = {25, -10, 40, -5};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(50, total_change(changes, 4), "total_change({25, -10, 40, -5}, 4)");
}

void test_total_of_no_changes(void) {
    TEST_IGNORE();
    const int16_t changes[] = {75};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, total_change(changes, 0), "total_change({}, 0)");
}

void test_total_of_cooling_changes(void) {
    TEST_IGNORE();
    const int16_t changes[] = {-12, -30, 4, -18, -1};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-57, total_change(changes, 5), "total_change({-12, -30, 4, -18, -1}, 5)");
}

void test_total_too_big_for_a_halfword(void) {
    TEST_IGNORE();
    const int16_t changes[] = {30000, 30000, 2000};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(62000, total_change(changes, 3), "total_change({30000, 30000, 2000}, 3)");
}

void test_total_too_low_for_a_halfword(void) {
    TEST_IGNORE();
    const int16_t changes[] = {-30000, -32768, 100};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-62668, total_change(changes, 3), "total_change({-30000, -32768, 100}, 3)");
}

// TASK: 4
void test_deepest_of_three_loggers(void) {
    TEST_IGNORE();
    const uint16_t depths[] = {350, 42000, 1800};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(42000, deepest(depths, 3), "deepest({350, 42000, 1800}, 3)");
}

void test_deepest_of_one_logger(void) {
    TEST_IGNORE();
    const uint16_t depths[] = {60000};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(60000, deepest(depths, 1), "deepest({60000}, 1)");
}

void test_deepest_of_shallow_loggers(void) {
    TEST_IGNORE();
    const uint16_t depths[] = {500, 1200, 900};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1200, deepest(depths, 3), "deepest({500, 1200, 900}, 3)");
}

void test_deepest_just_past_the_halfway_mark(void) {
    TEST_IGNORE();
    const uint16_t depths[] = {32767, 32768, 120};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(32768, deepest(depths, 3), "deepest({32767, 32768, 120}, 3)");
}

void test_deepest_on_the_outer_wall(void) {
    TEST_IGNORE();
    const uint16_t depths[] = {59000, 33000, 12, 60000};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(60000, deepest(depths, 4), "deepest({59000, 33000, 12, 60000}, 4)");
}

// TASK: 5
void test_calibrate_a_logger_that_reads_high(void) {
    TEST_IGNORE();
    int16_t log[] = {120, -35, 210};
    const int16_t expected[] = {100, -55, 190};
    calibrate(log, 3, -20);
    TEST_ASSERT_EQUAL_INT16_ARRAY_MESSAGE(expected, log, 3, "calibrate(log, 3, -20) with log = {120, -35, 210}");
}

void test_calibrate_a_logger_that_reads_low(void) {
    TEST_IGNORE();
    int16_t log[] = {-300, -5, 0, 47};
    const int16_t expected[] = {-285, 10, 15, 62};
    calibrate(log, 4, 15);
    TEST_ASSERT_EQUAL_INT16_ARRAY_MESSAGE(expected, log, 4, "calibrate(log, 4, 15) with log = {-300, -5, 0, 47}");
}

void test_calibrate_with_no_readings(void) {
    TEST_IGNORE();
    int16_t log[] = {80, 90};
    const int16_t expected[] = {80, 90};
    calibrate(log, 0, 25);
    TEST_ASSERT_EQUAL_INT16_ARRAY_MESSAGE(expected, log, 2, "calibrate(log, 0, 25) with log = {80, 90}");
}

void test_calibrate_only_the_readings_counted(void) {
    TEST_IGNORE();
    int16_t log[] = {10, 20, 30, 40};
    const int16_t expected[] = {7, 17, 27, 40};
    calibrate(log, 3, -3);
    TEST_ASSERT_EQUAL_INT16_ARRAY_MESSAGE(expected, log, 4, "calibrate(log, 3, -3) with log = {10, 20, 30, 40}");
}

void test_calibrate_large_readings(void) {
    TEST_IGNORE();
    int16_t log[] = {32000, -32000, 1};
    const int16_t expected[] = {32700, -31300, 701};
    calibrate(log, 3, 700);
    TEST_ASSERT_EQUAL_INT16_ARRAY_MESSAGE(expected, log, 3, "calibrate(log, 3, 700) with log = {32000, -32000, 1}");
}

// TASK: 6
void test_three_risky_readings(void) {
    TEST_IGNORE();
    const int16_t log[] = {120, 250, 90, 300, 250};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, count_bleaching_risk(log, 5, 250), "count_bleaching_risk({120, 250, 90, 300, 250}, 5, 250)");
}

void test_no_risky_readings(void) {
    TEST_IGNORE();
    const int16_t log[] = {-40, 15, 200, 249};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, count_bleaching_risk(log, 4, 250), "count_bleaching_risk({-40, 15, 200, 249}, 4, 250)");
}

void test_risky_readings_with_a_negative_threshold(void) {
    TEST_IGNORE();
    const int16_t log[] = {-200, -50, -100, 30, -101};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, count_bleaching_risk(log, 5, -100), "count_bleaching_risk({-200, -50, -100, 30, -101}, 5, -100)");
}

void test_risky_readings_in_an_empty_log(void) {
    TEST_IGNORE();
    const int16_t log[] = {400};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, count_bleaching_risk(log, 0, 250), "count_bleaching_risk({}, 0, 250)");
}

void test_every_reading_is_risky(void) {
    TEST_IGNORE();
    const int16_t log[] = {320, 410, 275};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, count_bleaching_risk(log, 3, 250), "count_bleaching_risk({320, 410, 275}, 3, 250)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_reading_at_a_cool_hour);
    RUN_TEST(test_reading_at_the_first_hour);
    RUN_TEST(test_reading_at_the_last_hour);
    RUN_TEST(test_reading_at_the_lowest_possible_value);
    RUN_TEST(test_reading_at_the_highest_possible_value);
    RUN_TEST(test_coolest_of_five_readings);
    RUN_TEST(test_coolest_of_one_reading);
    RUN_TEST(test_coolest_when_all_are_warm);
    RUN_TEST(test_coolest_is_the_last_reading);
    RUN_TEST(test_coolest_is_the_first_reading);
    RUN_TEST(test_total_of_four_changes);
    RUN_TEST(test_total_of_no_changes);
    RUN_TEST(test_total_of_cooling_changes);
    RUN_TEST(test_total_too_big_for_a_halfword);
    RUN_TEST(test_total_too_low_for_a_halfword);
    RUN_TEST(test_deepest_of_three_loggers);
    RUN_TEST(test_deepest_of_one_logger);
    RUN_TEST(test_deepest_of_shallow_loggers);
    RUN_TEST(test_deepest_just_past_the_halfway_mark);
    RUN_TEST(test_deepest_on_the_outer_wall);
    RUN_TEST(test_calibrate_a_logger_that_reads_high);
    RUN_TEST(test_calibrate_a_logger_that_reads_low);
    RUN_TEST(test_calibrate_with_no_readings);
    RUN_TEST(test_calibrate_only_the_readings_counted);
    RUN_TEST(test_calibrate_large_readings);
    RUN_TEST(test_three_risky_readings);
    RUN_TEST(test_no_risky_readings);
    RUN_TEST(test_risky_readings_with_a_negative_threshold);
    RUN_TEST(test_risky_readings_in_an_empty_log);
    RUN_TEST(test_every_reading_is_risky);
    return UNITY_END();
}
