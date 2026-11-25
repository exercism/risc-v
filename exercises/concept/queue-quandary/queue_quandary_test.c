#include "vendor/unity.h"

#include <stdint.h>

extern uint32_t next_ticket(uint32_t last);
extern uint32_t tickets_waiting(uint32_t now_serving, uint32_t last_printed);
extern int32_t is_valid_counter(uint32_t counter);
extern int32_t served_first(uint32_t a_open, uint32_t b_open);
extern uint32_t average_wait(uint32_t total, uint32_t customers);
extern uint32_t seconds_part(uint32_t time);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_the_ticket_after_41(void) {
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(42, next_ticket(41), "next_ticket(41)");
}

void test_the_ticket_before_the_wrap(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0xFFFFFFFF, next_ticket(0xFFFFFFFE), "next_ticket(0xFFFFFFFE)");
}

void test_the_counter_wraps_to_zero(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, next_ticket(0xFFFFFFFF), "next_ticket(0xFFFFFFFF)");
}

void test_the_counter_passes_the_signed_limit(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0x80000000, next_ticket(0x7FFFFFFF), "next_ticket(0x7FFFFFFF)");
}

// TASK: 2
void test_seven_people_waiting(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(7, tickets_waiting(40, 47), "tickets_waiting(40, 47)");
}

void test_nobody_waiting(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, tickets_waiting(12, 12), "tickets_waiting(12, 12)");
}

void test_waiting_across_the_wrap(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(5, tickets_waiting(0xFFFFFFFE, 3), "tickets_waiting(0xFFFFFFFE, 3)");
}

void test_waiting_just_after_the_wrap(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, tickets_waiting(0xFFFFFFFF, 0), "tickets_waiting(0xFFFFFFFF, 0)");
}

void test_waiting_across_the_signed_limit(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(32, tickets_waiting(0x7FFFFFF0, 0x80000010), "tickets_waiting(0x7FFFFFF0, 0x80000010)");
}

// TASK: 3
void test_counter_1_is_valid(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, is_valid_counter(1), "is_valid_counter(1)");
}

void test_counter_12_is_valid(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, is_valid_counter(12), "is_valid_counter(12)");
}

void test_counter_0_is_not_valid(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, is_valid_counter(0), "is_valid_counter(0)");
}

void test_counter_13_is_not_valid(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, is_valid_counter(13), "is_valid_counter(13)");
}

void test_a_huge_counter_number_is_not_valid(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, is_valid_counter(0x80000005), "is_valid_counter(0x80000005)");
}

// TASK: 4
void test_a_has_been_open_longer(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, served_first(3000000000, 1000000), "served_first(3000000000, 1000000)");
}

void test_b_has_been_open_longer(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, served_first(1000000, 3000000000), "served_first(1000000, 3000000000)");
}

void test_both_open_over_half_an_hour(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, served_first(2500000000, 2400000000), "served_first(2500000000, 2400000000)");
}

void test_open_for_the_same_time(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, served_first(5000000, 5000000), "served_first(5000000, 5000000)");
}

void test_times_either_side_of_the_signed_limit(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, served_first(0x80000000, 0x7FFFFFFF), "served_first(0x80000000, 0x7FFFFFFF)");
}

// TASK: 5
void test_a_huge_total(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(400000000, average_wait(4000000000, 10), "average_wait(4000000000, 10)");
}

void test_a_small_total(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(30000000, average_wait(90000000, 3), "average_wait(90000000, 3)");
}

void test_the_average_is_rounded_down(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(14, average_wait(100, 7), "average_wait(100, 7)");
}

void test_no_customers(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, average_wait(3000000000, 0), "average_wait(3000000000, 0)");
}

void test_one_customer_and_the_largest_total(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0xFFFFFFFF, average_wait(0xFFFFFFFF, 1), "average_wait(0xFFFFFFFF, 1)");
}

// TASK: 6
void test_sixty_six_minutes_forty_seconds(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(40, seconds_part(4000000000), "seconds_part(4000000000)");
}

void test_just_under_a_minute(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(59, seconds_part(59999999), "seconds_part(59999999)");
}

void test_exactly_one_minute(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, seconds_part(60000000), "seconds_part(60000000)");
}

void test_two_minutes_five_seconds(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(5, seconds_part(125000000), "seconds_part(125000000)");
}

void test_the_longest_possible_time(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(34, seconds_part(0xFFFFFFFF), "seconds_part(0xFFFFFFFF)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_the_ticket_after_41);
    RUN_TEST(test_the_ticket_before_the_wrap);
    RUN_TEST(test_the_counter_wraps_to_zero);
    RUN_TEST(test_the_counter_passes_the_signed_limit);
    RUN_TEST(test_seven_people_waiting);
    RUN_TEST(test_nobody_waiting);
    RUN_TEST(test_waiting_across_the_wrap);
    RUN_TEST(test_waiting_just_after_the_wrap);
    RUN_TEST(test_waiting_across_the_signed_limit);
    RUN_TEST(test_counter_1_is_valid);
    RUN_TEST(test_counter_12_is_valid);
    RUN_TEST(test_counter_0_is_not_valid);
    RUN_TEST(test_counter_13_is_not_valid);
    RUN_TEST(test_a_huge_counter_number_is_not_valid);
    RUN_TEST(test_a_has_been_open_longer);
    RUN_TEST(test_b_has_been_open_longer);
    RUN_TEST(test_both_open_over_half_an_hour);
    RUN_TEST(test_open_for_the_same_time);
    RUN_TEST(test_times_either_side_of_the_signed_limit);
    RUN_TEST(test_a_huge_total);
    RUN_TEST(test_a_small_total);
    RUN_TEST(test_the_average_is_rounded_down);
    RUN_TEST(test_no_customers);
    RUN_TEST(test_one_customer_and_the_largest_total);
    RUN_TEST(test_sixty_six_minutes_forty_seconds);
    RUN_TEST(test_just_under_a_minute);
    RUN_TEST(test_exactly_one_minute);
    RUN_TEST(test_two_minutes_five_seconds);
    RUN_TEST(test_the_longest_possible_time);
    return UNITY_END();
}
