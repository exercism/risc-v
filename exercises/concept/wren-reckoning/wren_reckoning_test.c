#include "vendor/unity.h"

#include <stdint.h>

extern int32_t total_sightings(int32_t days);
extern int32_t weeks_to_train(int32_t needed);
extern int32_t digit_count(int32_t count);
extern int32_t digit_sum(int32_t band);
extern int32_t reverse_digits(int32_t number);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_total_sightings_four_days(void) {
    TEST_ASSERT_EQUAL_INT32_MESSAGE(10, total_sightings(4), "total_sightings(4)");
}

void test_total_sightings_no_days(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, total_sightings(0), "total_sightings(0)");
}

void test_total_sightings_one_day(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, total_sightings(1), "total_sightings(1)");
}

void test_total_sightings_ten_days(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(55, total_sightings(10), "total_sightings(10)");
}

void test_total_sightings_a_full_term(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(5050, total_sightings(100), "total_sightings(100)");
}

// TASK: 2
void test_weeks_to_train_five_spotters(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, weeks_to_train(5), "weeks_to_train(5)");
}

void test_weeks_to_train_only_wren_is_needed(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, weeks_to_train(1), "weeks_to_train(1)");
}

void test_weeks_to_train_exactly_sixteen_spotters(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, weeks_to_train(16), "weeks_to_train(16)");
}

void test_weeks_to_train_seventeen_spotters(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(5, weeks_to_train(17), "weeks_to_train(17)");
}

void test_weeks_to_train_a_thousand_spotters(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(10, weeks_to_train(1000), "weeks_to_train(1000)");
}

// TASK: 3
void test_digit_count_three_digits(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, digit_count(345), "digit_count(345)");
}

void test_digit_count_zero_needs_one_digit(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, digit_count(0), "digit_count(0)");
}

void test_digit_count_a_single_digit(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, digit_count(7), "digit_count(7)");
}

void test_digit_count_ten_has_two_digits(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, digit_count(10), "digit_count(10)");
}

void test_digit_count_the_largest_count(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(10, digit_count(2147483647), "digit_count(2147483647)");
}

// TASK: 4
void test_digit_sum_a_four_digit_band(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(14, digit_sum(4721), "digit_sum(4721)");
}

void test_digit_sum_band_zero(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, digit_sum(0), "digit_sum(0)");
}

void test_digit_sum_a_single_digit_band(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(9, digit_sum(9), "digit_sum(9)");
}

void test_digit_sum_all_nines(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(45, digit_sum(99999), "digit_sum(99999)");
}

void test_digit_sum_mostly_zeros(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, digit_sum(1000000), "digit_sum(1000000)");
}

// TASK: 5
void test_reverse_digits_four_digits(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4321, reverse_digits(1234), "reverse_digits(1234)");
}

void test_reverse_digits_trailing_zeros_disappear(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(21, reverse_digits(1200), "reverse_digits(1200)");
}

void test_reverse_digits_a_zero_in_the_middle(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(509, reverse_digits(905), "reverse_digits(905)");
}

void test_reverse_digits_a_single_digit(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(7, reverse_digits(7), "reverse_digits(7)");
}

void test_reverse_digits_zero(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, reverse_digits(0), "reverse_digits(0)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_total_sightings_four_days);
    RUN_TEST(test_total_sightings_no_days);
    RUN_TEST(test_total_sightings_one_day);
    RUN_TEST(test_total_sightings_ten_days);
    RUN_TEST(test_total_sightings_a_full_term);
    RUN_TEST(test_weeks_to_train_five_spotters);
    RUN_TEST(test_weeks_to_train_only_wren_is_needed);
    RUN_TEST(test_weeks_to_train_exactly_sixteen_spotters);
    RUN_TEST(test_weeks_to_train_seventeen_spotters);
    RUN_TEST(test_weeks_to_train_a_thousand_spotters);
    RUN_TEST(test_digit_count_three_digits);
    RUN_TEST(test_digit_count_zero_needs_one_digit);
    RUN_TEST(test_digit_count_a_single_digit);
    RUN_TEST(test_digit_count_ten_has_two_digits);
    RUN_TEST(test_digit_count_the_largest_count);
    RUN_TEST(test_digit_sum_a_four_digit_band);
    RUN_TEST(test_digit_sum_band_zero);
    RUN_TEST(test_digit_sum_a_single_digit_band);
    RUN_TEST(test_digit_sum_all_nines);
    RUN_TEST(test_digit_sum_mostly_zeros);
    RUN_TEST(test_reverse_digits_four_digits);
    RUN_TEST(test_reverse_digits_trailing_zeros_disappear);
    RUN_TEST(test_reverse_digits_a_zero_in_the_middle);
    RUN_TEST(test_reverse_digits_a_single_digit);
    RUN_TEST(test_reverse_digits_zero);
    return UNITY_END();
}
