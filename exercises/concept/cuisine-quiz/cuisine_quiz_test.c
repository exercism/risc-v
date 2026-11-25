#include "vendor/unity.h"

#include <stdint.h>
#include <string.h>

extern int32_t format_score(char *buf, uint32_t score);
extern int32_t format_signed(char *buf, int32_t points);
extern int32_t format_padded(char *buf, uint32_t n, int32_t width);
extern uint32_t parse_score(const char *text);
extern int32_t parse_signed(const char *text);

#define BUFFER_SIZE 16

// Filled with '#' before each test, so a missing zero terminator shows up.
static char buf[BUFFER_SIZE];

void setUp(void) {
    memset(buf, '#', BUFFER_SIZE - 1);
    buf[BUFFER_SIZE - 1] = 0;
}

void tearDown(void) {
}

// TASK: 1
void test_format_a_two_digit_score(void) {
    int32_t length = format_score(buf, 42);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, length, "format_score(buf, 42)");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("42", buf, "buf after format_score(buf, 42)");
}

void test_format_a_score_of_zero(void) {
    TEST_IGNORE();
    int32_t length = format_score(buf, 0);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, length, "format_score(buf, 0)");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("0", buf, "buf after format_score(buf, 0)");
}

void test_format_a_one_digit_score(void) {
    TEST_IGNORE();
    int32_t length = format_score(buf, 9);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, length, "format_score(buf, 9)");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("9", buf, "buf after format_score(buf, 9)");
}

void test_format_a_score_ending_in_zero(void) {
    TEST_IGNORE();
    int32_t length = format_score(buf, 10);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, length, "format_score(buf, 10)");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("10", buf, "buf after format_score(buf, 10)");
}

void test_format_the_biggest_score(void) {
    TEST_IGNORE();
    int32_t length = format_score(buf, 4294967295u);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(10, length, "format_score(buf, 4294967295)");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("4294967295", buf, "buf after format_score(buf, 4294967295)");
}

// TASK: 2
void test_format_signed_a_penalty(void) {
    TEST_IGNORE();
    int32_t length = format_signed(buf, -15);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, length, "format_signed(buf, -15)");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("-15", buf, "buf after format_signed(buf, -15)");
}

void test_format_signed_a_positive_total(void) {
    TEST_IGNORE();
    int32_t length = format_signed(buf, 27);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, length, "format_signed(buf, 27)");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("27", buf, "buf after format_signed(buf, 27)");
}

void test_format_signed_zero_points(void) {
    TEST_IGNORE();
    int32_t length = format_signed(buf, 0);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, length, "format_signed(buf, 0)");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("0", buf, "buf after format_signed(buf, 0)");
}

void test_format_signed_the_biggest_penalty(void) {
    TEST_IGNORE();
    int32_t length = format_signed(buf, INT32_MIN);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(11, length, "format_signed(buf, INT32_MIN)");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("-2147483648", buf, "buf after format_signed(buf, INT32_MIN)");
}

void test_format_signed_the_biggest_total(void) {
    TEST_IGNORE();
    int32_t length = format_signed(buf, INT32_MAX);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(10, length, "format_signed(buf, INT32_MAX)");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("2147483647", buf, "buf after format_signed(buf, INT32_MAX)");
}

// TASK: 3
void test_pad_table_seven(void) {
    TEST_IGNORE();
    int32_t length = format_padded(buf, 7, 3);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, length, "format_padded(buf, 7, 3)");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("007", buf, "buf after format_padded(buf, 7, 3)");
}

void test_pad_a_two_digit_table(void) {
    TEST_IGNORE();
    int32_t length = format_padded(buf, 42, 3);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, length, "format_padded(buf, 42, 3)");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("042", buf, "buf after format_padded(buf, 42, 3)");
}

void test_pad_a_number_wider_than_the_width(void) {
    TEST_IGNORE();
    int32_t length = format_padded(buf, 1234, 3);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, length, "format_padded(buf, 1234, 3)");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("1234", buf, "buf after format_padded(buf, 1234, 3)");
}

void test_pad_zero(void) {
    TEST_IGNORE();
    int32_t length = format_padded(buf, 0, 4);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, length, "format_padded(buf, 0, 4)");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("0000", buf, "buf after format_padded(buf, 0, 4)");
}

void test_pad_a_width_of_zero(void) {
    TEST_IGNORE();
    int32_t length = format_padded(buf, 5, 0);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, length, "format_padded(buf, 5, 0)");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("5", buf, "buf after format_padded(buf, 5, 0)");
}

// TASK: 4
void test_parse_a_plain_score(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(42, parse_score("42"), "parse_score(\"42\")");
}

void test_parse_a_score_followed_by_a_word(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(120, parse_score("120 points"), "parse_score(\"120 points\")");
}

void test_parse_a_score_with_leading_zeros(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(7, parse_score("007"), "parse_score(\"007\")");
}

void test_parse_an_empty_sheet(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, parse_score(""), "parse_score(\"\")");
}

void test_parse_the_biggest_score(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(4294967295u, parse_score("4294967295"), "parse_score(\"4294967295\")");
}

// TASK: 5
void test_parse_signed_a_penalty(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-15, parse_signed("-15"), "parse_signed(\"-15\")");
}

void test_parse_signed_a_positive_total(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(27, parse_signed("27"), "parse_signed(\"27\")");
}

void test_parse_signed_the_biggest_penalty(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(INT32_MIN, parse_signed("-2147483648"), "parse_signed(\"-2147483648\")");
}

void test_parse_signed_the_biggest_total(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(INT32_MAX, parse_signed("2147483647 points"), "parse_signed(\"2147483647 points\")");
}

void test_parse_signed_minus_zero(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, parse_signed("-0"), "parse_signed(\"-0\")");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_format_a_two_digit_score);
    RUN_TEST(test_format_a_score_of_zero);
    RUN_TEST(test_format_a_one_digit_score);
    RUN_TEST(test_format_a_score_ending_in_zero);
    RUN_TEST(test_format_the_biggest_score);
    RUN_TEST(test_format_signed_a_penalty);
    RUN_TEST(test_format_signed_a_positive_total);
    RUN_TEST(test_format_signed_zero_points);
    RUN_TEST(test_format_signed_the_biggest_penalty);
    RUN_TEST(test_format_signed_the_biggest_total);
    RUN_TEST(test_pad_table_seven);
    RUN_TEST(test_pad_a_two_digit_table);
    RUN_TEST(test_pad_a_number_wider_than_the_width);
    RUN_TEST(test_pad_zero);
    RUN_TEST(test_pad_a_width_of_zero);
    RUN_TEST(test_parse_a_plain_score);
    RUN_TEST(test_parse_a_score_followed_by_a_word);
    RUN_TEST(test_parse_a_score_with_leading_zeros);
    RUN_TEST(test_parse_an_empty_sheet);
    RUN_TEST(test_parse_the_biggest_score);
    RUN_TEST(test_parse_signed_a_penalty);
    RUN_TEST(test_parse_signed_a_positive_total);
    RUN_TEST(test_parse_signed_the_biggest_penalty);
    RUN_TEST(test_parse_signed_the_biggest_total);
    RUN_TEST(test_parse_signed_minus_zero);
    return UNITY_END();
}
