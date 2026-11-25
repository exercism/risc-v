#include "vendor/unity.h"

#include <stdint.h>

extern int32_t is_earlier(int32_t a, int32_t b);
extern int32_t alibis_agree(int32_t a, int32_t b);
extern int32_t was_present(int32_t arrive, int32_t leave, int32_t t);
extern int32_t is_suspect(int32_t present, int32_t has_motive);
extern int32_t is_cleared(int32_t away, int32_t witnesses);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_a_morning_time_is_earlier(void) {
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, is_earlier(600, 615), "is_earlier(600, 615)");
}

void test_a_later_time_is_not_earlier(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, is_earlier(855, 570), "is_earlier(855, 570)");
}

void test_the_same_time_is_not_earlier(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, is_earlier(720, 720), "is_earlier(720, 720)");
}

void test_one_minute_before_is_earlier(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, is_earlier(719, 720), "is_earlier(719, 720)");
}

void test_midnight_is_earlier_than_any_other_time(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, is_earlier(0, 1439), "is_earlier(0, 1439)");
}

// TASK: 2
void test_matching_times_agree(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, alibis_agree(720, 720), "alibis_agree(720, 720)");
}

void test_different_times_disagree(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, alibis_agree(720, 725), "alibis_agree(720, 725)");
}

void test_one_minute_apart_disagree(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, alibis_agree(601, 600), "alibis_agree(601, 600)");
}

void test_both_at_midnight_agree(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, alibis_agree(0, 0), "alibis_agree(0, 0)");
}

// TASK: 3
void test_present_in_the_middle_of_a_visit(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, was_present(600, 660, 630), "was_present(600, 660, 630)");
}

void test_present_at_the_moment_of_arrival(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, was_present(600, 660, 600), "was_present(600, 660, 600)");
}

void test_absent_at_the_moment_of_leaving(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, was_present(600, 660, 660), "was_present(600, 660, 660)");
}

void test_absent_before_arriving(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, was_present(600, 660, 599), "was_present(600, 660, 599)");
}

void test_absent_after_leaving(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, was_present(600, 660, 700), "was_present(600, 660, 700)");
}

// TASK: 4
void test_present_with_a_motive_is_a_suspect(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, is_suspect(1, 1), "is_suspect(1, 1)");
}

void test_present_without_a_motive_is_not_a_suspect(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, is_suspect(1, 0), "is_suspect(1, 0)");
}

void test_a_motive_but_absent_is_not_a_suspect(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, is_suspect(0, 1), "is_suspect(0, 1)");
}

void test_neither_present_nor_a_motive(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, is_suspect(0, 0), "is_suspect(0, 0)");
}

// TASK: 5
void test_away_with_no_witnesses_is_cleared(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, is_cleared(1, 0), "is_cleared(1, 0)");
}

void test_two_witnesses_clear_a_student(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, is_cleared(0, 2), "is_cleared(0, 2)");
}

void test_one_witness_clears_a_student(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, is_cleared(0, 1), "is_cleared(0, 1)");
}

void test_not_away_and_no_witnesses_is_not_cleared(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, is_cleared(0, 0), "is_cleared(0, 0)");
}

void test_away_with_three_witnesses_is_cleared(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, is_cleared(1, 3), "is_cleared(1, 3)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_a_morning_time_is_earlier);
    RUN_TEST(test_a_later_time_is_not_earlier);
    RUN_TEST(test_the_same_time_is_not_earlier);
    RUN_TEST(test_one_minute_before_is_earlier);
    RUN_TEST(test_midnight_is_earlier_than_any_other_time);
    RUN_TEST(test_matching_times_agree);
    RUN_TEST(test_different_times_disagree);
    RUN_TEST(test_one_minute_apart_disagree);
    RUN_TEST(test_both_at_midnight_agree);
    RUN_TEST(test_present_in_the_middle_of_a_visit);
    RUN_TEST(test_present_at_the_moment_of_arrival);
    RUN_TEST(test_absent_at_the_moment_of_leaving);
    RUN_TEST(test_absent_before_arriving);
    RUN_TEST(test_absent_after_leaving);
    RUN_TEST(test_present_with_a_motive_is_a_suspect);
    RUN_TEST(test_present_without_a_motive_is_not_a_suspect);
    RUN_TEST(test_a_motive_but_absent_is_not_a_suspect);
    RUN_TEST(test_neither_present_nor_a_motive);
    RUN_TEST(test_away_with_no_witnesses_is_cleared);
    RUN_TEST(test_two_witnesses_clear_a_student);
    RUN_TEST(test_one_witness_clears_a_student);
    RUN_TEST(test_not_away_and_no_witnesses_is_not_cleared);
    RUN_TEST(test_away_with_three_witnesses_is_cleared);
    return UNITY_END();
}
