#include "vendor/unity.h"

#include <stdint.h>

extern uint64_t add_delay(uint64_t time_us, uint32_t delay_us);
extern uint64_t journey_time(uint64_t depart_us, uint64_t arrive_us);
extern int32_t departs_before(uint64_t first_us, uint64_t second_us);
extern uint64_t minutes_to_us(uint32_t minutes);
extern uint64_t arrival_time(uint64_t depart_us, uint32_t minutes);

// Unity's 64-bit assertions are not enabled on RV32, so compare the high and low halves.
#define ASSERT_EQUAL_UINT64_MESSAGE(expected, actual, message)                                                                    \
    do {                                                                                                                          \
        uint64_t expected_value = (expected);                                                                                     \
        uint64_t actual_value = (actual);                                                                                         \
        TEST_ASSERT_EQUAL_HEX32_MESSAGE((uint32_t)(expected_value >> 32), (uint32_t)(actual_value >> 32), message ", high half"); \
        TEST_ASSERT_EQUAL_HEX32_MESSAGE((uint32_t)expected_value, (uint32_t)actual_value, message ", low half");                  \
    } while (0)

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_a_short_delay(void) {
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(1795540500000000), add_delay(UINT64_C(1795539600000000), 900000000),
                                "add_delay(1795539600000000, 900000000)");
}

void test_a_delay_that_carries_into_the_high_half(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(1795545600000000), add_delay(UINT64_C(1795545000000000), 600000000),
                                "add_delay(1795545000000000, 600000000)");
}

void test_a_one_microsecond_delay_that_carries(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(0x0006610900000000), add_delay(UINT64_C(0x00066108FFFFFFFF), 1),
                                "add_delay(0x00066108FFFFFFFF, 1)");
}

void test_the_longest_delay(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(0x00066109FFFFFFFE), add_delay(UINT64_C(0x00066108FFFFFFFF), 0xFFFFFFFF),
                                "add_delay(0x00066108FFFFFFFF, 0xFFFFFFFF)");
}

void test_no_delay(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(1795539600000000), add_delay(UINT64_C(1795539600000000), 0), "add_delay(1795539600000000, 0)");
}

// TASK: 2
void test_a_journey_with_no_borrow(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(900000000), journey_time(UINT64_C(1795539600000000), UINT64_C(1795540500000000)),
                                "journey_time(1795539600000000, 1795540500000000)");
}

void test_a_journey_that_borrows_from_the_high_half(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(600000000), journey_time(UINT64_C(1795545000000000), UINT64_C(1795545600000000)),
                                "journey_time(1795545000000000, 1795545600000000)");
}

void test_the_whole_voyage_to_vavau(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(68400000000), journey_time(UINT64_C(1795539600000000), UINT64_C(1795608000000000)),
                                "journey_time(1795539600000000, 1795608000000000)");
}

void test_two_microseconds_across_the_boundary(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(2), journey_time(UINT64_C(0x00066108FFFFFFFF), UINT64_C(0x0006610900000001)),
                                "journey_time(0x00066108FFFFFFFF, 0x0006610900000001)");
}

void test_a_journey_of_no_time(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(0), journey_time(UINT64_C(1795539600000000), UINT64_C(1795539600000000)),
                                "journey_time(1795539600000000, 1795539600000000)");
}

// TASK: 3
void test_a_departure_on_an_earlier_day(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, departs_before(UINT64_C(1795539600000000), UINT64_C(1795608000000000)),
                                    "departs_before(1795539600000000, 1795608000000000)");
}

void test_a_departure_on_a_later_day(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, departs_before(UINT64_C(1795608000000000), UINT64_C(1795539600000000)),
                                    "departs_before(1795608000000000, 1795539600000000)");
}

void test_low_halves_are_compared_as_unsigned(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, departs_before(UINT64_C(0x000661087FFFFFFF), UINT64_C(0x0006610880000000)),
                                    "departs_before(0x000661087FFFFFFF, 0x0006610880000000)");
}

void test_the_high_halves_decide_first(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, departs_before(UINT64_C(0x00066108FFFFFFFF), UINT64_C(0x0006610900000000)),
                                    "departs_before(0x00066108FFFFFFFF, 0x0006610900000000)");
}

void test_the_same_departure_time(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, departs_before(UINT64_C(1795539600000000), UINT64_C(1795539600000000)),
                                    "departs_before(1795539600000000, 1795539600000000)");
}

// TASK: 4
void test_one_minute(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(60000000), minutes_to_us(1), "minutes_to_us(1)");
}

void test_71_minutes_still_fit_in_32_bits(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(4260000000), minutes_to_us(71), "minutes_to_us(71)");
}

void test_72_minutes_need_the_high_half(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(4320000000), minutes_to_us(72), "minutes_to_us(72)");
}

void test_the_voyage_to_vavau_in_minutes(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(68400000000), minutes_to_us(1140), "minutes_to_us(1140)");
}

void test_the_most_minutes(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(257698037700000000), minutes_to_us(0xFFFFFFFF), "minutes_to_us(0xFFFFFFFF)");
}

// TASK: 5
void test_a_short_hop(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(1795540500000000), arrival_time(UINT64_C(1795539600000000), 15),
                                "arrival_time(1795539600000000, 15)");
}

void test_the_voyage_to_vavau(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(1795608000000000), arrival_time(UINT64_C(1795539600000000), 1140),
                                "arrival_time(1795539600000000, 1140)");
}

void test_an_arrival_that_carries_into_the_high_half(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(1795545600000000), arrival_time(UINT64_C(1795545000000000), 10),
                                "arrival_time(1795545000000000, 10)");
}

void test_a_long_journey_that_also_carries(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(0x0006610A017DF7FF), arrival_time(UINT64_C(0x00066108FFFFFFFF), 72),
                                "arrival_time(0x00066108FFFFFFFF, 72)");
}

void test_a_journey_of_no_minutes(void) {
    TEST_IGNORE();
    ASSERT_EQUAL_UINT64_MESSAGE(UINT64_C(1795539600000000), arrival_time(UINT64_C(1795539600000000), 0),
                                "arrival_time(1795539600000000, 0)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_a_short_delay);
    RUN_TEST(test_a_delay_that_carries_into_the_high_half);
    RUN_TEST(test_a_one_microsecond_delay_that_carries);
    RUN_TEST(test_the_longest_delay);
    RUN_TEST(test_no_delay);
    RUN_TEST(test_a_journey_with_no_borrow);
    RUN_TEST(test_a_journey_that_borrows_from_the_high_half);
    RUN_TEST(test_the_whole_voyage_to_vavau);
    RUN_TEST(test_two_microseconds_across_the_boundary);
    RUN_TEST(test_a_journey_of_no_time);
    RUN_TEST(test_a_departure_on_an_earlier_day);
    RUN_TEST(test_a_departure_on_a_later_day);
    RUN_TEST(test_low_halves_are_compared_as_unsigned);
    RUN_TEST(test_the_high_halves_decide_first);
    RUN_TEST(test_the_same_departure_time);
    RUN_TEST(test_one_minute);
    RUN_TEST(test_71_minutes_still_fit_in_32_bits);
    RUN_TEST(test_72_minutes_need_the_high_half);
    RUN_TEST(test_the_voyage_to_vavau_in_minutes);
    RUN_TEST(test_the_most_minutes);
    RUN_TEST(test_a_short_hop);
    RUN_TEST(test_the_voyage_to_vavau);
    RUN_TEST(test_an_arrival_that_carries_into_the_high_half);
    RUN_TEST(test_a_long_journey_that_also_carries);
    RUN_TEST(test_a_journey_of_no_minutes);
    return UNITY_END();
}
