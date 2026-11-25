#include "vendor/unity.h"

#include <stdint.h>

extern uint32_t sight(uint32_t log, int32_t boat);
extern int32_t has_passed(uint32_t log, int32_t boat);
extern uint32_t missing(uint32_t expected, uint32_t seen);
extern int32_t boats_seen(uint32_t log);
extern int32_t first_boat(uint32_t log);
extern uint32_t log_from_flags(const char *flags);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_the_first_boat_of_the_day(void) {
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x00000001, sight(0x00000000, 0), "sight(0x00000000, 0)");
}

void test_a_boat_joins_the_log(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x00000018, sight(0x00000010, 3), "sight(0x00000010, 3)");
}

void test_a_boat_seen_twice_is_logged_once(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x00000005, sight(0x00000005, 2), "sight(0x00000005, 2)");
}

void test_boat_20_joins_a_busy_log(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x0010FFFF, sight(0x0000FFFF, 20), "sight(0x0000FFFF, 20)");
}

void test_boat_31_is_the_top_bit(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x80000000, sight(0x00000000, 31), "sight(0x00000000, 31)");
}

// TASK: 2
void test_a_boat_in_the_log(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, has_passed(0x00000018, 3), "has_passed(0x00000018, 3)");
}

void test_a_boat_not_in_the_log(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, has_passed(0x00000018, 2), "has_passed(0x00000018, 2)");
}

void test_boat_0_is_missing_from_a_busy_log(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, has_passed(0xFFFFFFFE, 0), "has_passed(0xFFFFFFFE, 0)");
}

void test_boat_31_has_passed(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, has_passed(0x80000000, 31), "has_passed(0x80000000, 31)");
}

void test_boat_31_has_not_passed(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, has_passed(0x7FFFFFFF, 31), "has_passed(0x7FFFFFFF, 31)");
}

// TASK: 3
void test_half_the_boats_are_missing(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x000000F0, missing(0x000000FF, 0x0000000F), "missing(0x000000FF, 0x0000000F)");
}

void test_every_boat_came_home(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x00000000, missing(0x0000000F, 0x000000FF), "missing(0x0000000F, 0x000000FF)");
}

void test_no_boat_seen_yet(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x00001234, missing(0x00001234, 0x00000000), "missing(0x00001234, 0x00000000)");
}

void test_boat_31_is_still_at_sea(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x80000000, missing(0x80000001, 0x00000001), "missing(0x80000001, 0x00000001)");
}

void test_no_boats_went_out(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x00000000, missing(0x00000000, 0xFFFFFFFF), "missing(0x00000000, 0xFFFFFFFF)");
}

// TASK: 4
void test_three_boats_seen(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, boats_seen(0x00000019), "boats_seen(0x00000019)");
}

void test_no_boats_seen(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, boats_seen(0x00000000), "boats_seen(0x00000000)");
}

void test_only_boat_31_seen(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, boats_seen(0x80000000), "boats_seen(0x80000000)");
}

void test_scattered_boats_seen(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(8, boats_seen(0x80F0000E), "boats_seen(0x80F0000E)");
}

void test_every_boat_seen(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(32, boats_seen(0xFFFFFFFF), "boats_seen(0xFFFFFFFF)");
}

// TASK: 5
void test_the_first_boat_is_13(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(13, first_boat(0x00012000), "first_boat(0x00012000)");
}

void test_boat_0_comes_first(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, first_boat(0x00000001), "first_boat(0x00000001)");
}

void test_an_empty_log_has_no_first_boat(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, first_boat(0x00000000), "first_boat(0x00000000)");
}

void test_boat_31_on_its_own_comes_first(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(31, first_boat(0x80000000), "first_boat(0x80000000)");
}

void test_a_full_log_starts_at_boat_0(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, first_boat(0xFFFFFFFF), "first_boat(0xFFFFFFFF)");
}

// TASK: 6
void test_a_message_with_commas(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x02000003, log_from_flags("a, b, z"), "log_from_flags(\"a, b, z\")");
}

void test_an_empty_message(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x00000000, log_from_flags(""), "log_from_flags(\"\")");
}

void test_a_boat_named_twice(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x00000001, log_from_flags("a a a"), "log_from_flags(\"a a a\")");
}

void test_capitals_and_punctuation_name_no_boat(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x00000000, log_from_flags("ABZ `{@[ 019"), "log_from_flags(\"ABZ `{@[ 019\")");
}

void test_bytes_above_127_name_no_boat(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x00000004, log_from_flags("\341c\372"), "log_from_flags(\"\\341c\\372\")");
}

void test_every_letter(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x03FFFFFF, log_from_flags("the quick brown fox jumps over the lazy dog"),
                                    "log_from_flags(\"the quick brown fox jumps over the lazy dog\")");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_the_first_boat_of_the_day);
    RUN_TEST(test_a_boat_joins_the_log);
    RUN_TEST(test_a_boat_seen_twice_is_logged_once);
    RUN_TEST(test_boat_20_joins_a_busy_log);
    RUN_TEST(test_boat_31_is_the_top_bit);
    RUN_TEST(test_a_boat_in_the_log);
    RUN_TEST(test_a_boat_not_in_the_log);
    RUN_TEST(test_boat_0_is_missing_from_a_busy_log);
    RUN_TEST(test_boat_31_has_passed);
    RUN_TEST(test_boat_31_has_not_passed);
    RUN_TEST(test_half_the_boats_are_missing);
    RUN_TEST(test_every_boat_came_home);
    RUN_TEST(test_no_boat_seen_yet);
    RUN_TEST(test_boat_31_is_still_at_sea);
    RUN_TEST(test_no_boats_went_out);
    RUN_TEST(test_three_boats_seen);
    RUN_TEST(test_no_boats_seen);
    RUN_TEST(test_only_boat_31_seen);
    RUN_TEST(test_scattered_boats_seen);
    RUN_TEST(test_every_boat_seen);
    RUN_TEST(test_the_first_boat_is_13);
    RUN_TEST(test_boat_0_comes_first);
    RUN_TEST(test_an_empty_log_has_no_first_boat);
    RUN_TEST(test_boat_31_on_its_own_comes_first);
    RUN_TEST(test_a_full_log_starts_at_boat_0);
    RUN_TEST(test_a_message_with_commas);
    RUN_TEST(test_an_empty_message);
    RUN_TEST(test_a_boat_named_twice);
    RUN_TEST(test_capitals_and_punctuation_name_no_boat);
    RUN_TEST(test_bytes_above_127_name_no_boat);
    RUN_TEST(test_every_letter);
    return UNITY_END();
}
