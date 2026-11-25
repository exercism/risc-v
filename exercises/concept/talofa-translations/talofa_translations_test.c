#include "vendor/unity.h"

#include <stdint.h>
#include <string.h>

extern const char *greeting(void);
extern const char *thank_you(int32_t language);
extern int32_t days_in_month(int32_t month);
extern int32_t day_of_year(int32_t month, int32_t day);
extern int32_t visitors(void);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_greeting_says_talofa_lava(void) {
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Talofa lava!", greeting(), "greeting()");
}

void test_greeting_has_12_characters(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(12, (int32_t)strlen(greeting()), "strlen(greeting())");
}

void test_greeting_is_the_same_string_every_time(void) {
    TEST_IGNORE();
    const char *first = greeting();
    TEST_ASSERT_EQUAL_PTR_MESSAGE(first, greeting(), "greeting()");
}

// TASK: 2
void test_thank_you_in_samoan(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Faʻafetai", thank_you(0), "thank_you(0)");
}

void test_thank_you_in_tongan(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Mālō", thank_you(1), "thank_you(1)");
}

void test_thank_you_in_tok_pisin(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Tenkyu", thank_you(2), "thank_you(2)");
}

void test_thank_you_in_an_unknown_language(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Thank you", thank_you(3), "thank_you(3)");
}

void test_thank_you_for_a_negative_code(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Thank you", thank_you(-1), "thank_you(-1)");
}

// TASK: 3
void test_days_in_january(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(31, days_in_month(1), "days_in_month(1)");
}

void test_days_in_february(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(28, days_in_month(2), "days_in_month(2)");
}

void test_days_in_april(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(30, days_in_month(4), "days_in_month(4)");
}

void test_days_in_september(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(30, days_in_month(9), "days_in_month(9)");
}

void test_days_in_december(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(31, days_in_month(12), "days_in_month(12)");
}

// TASK: 4
void test_new_years_day_is_day_1(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, day_of_year(1, 1), "day_of_year(1, 1)");
}

void test_the_last_day_of_january(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(31, day_of_year(1, 31), "day_of_year(1, 31)");
}

void test_a_day_in_february(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(45, day_of_year(2, 14), "day_of_year(2, 14)");
}

void test_a_day_in_june(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(166, day_of_year(6, 15), "day_of_year(6, 15)");
}

void test_the_last_day_of_the_year(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(365, day_of_year(12, 31), "day_of_year(12, 31)");
}

// TASK: 5
// No earlier test calls visitors, so this is its first call.
void test_the_first_visitor_is_number_1(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, visitors(), "visitors()");
}

void test_the_next_visitor_is_counted(void) {
    TEST_IGNORE();
    int32_t before = visitors();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(before + 1, visitors(), "visitors()");
}

void test_three_more_visitors(void) {
    TEST_IGNORE();
    int32_t before = visitors();
    visitors();
    visitors();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(before + 3, visitors(), "visitors()");
}

void test_a_busy_lunchtime(void) {
    TEST_IGNORE();
    int32_t before = visitors();
    for (int32_t i = 1; i <= 50; i++) {
        TEST_ASSERT_EQUAL_INT32_MESSAGE(before + i, visitors(), "visitors()");
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_greeting_says_talofa_lava);
    RUN_TEST(test_greeting_has_12_characters);
    RUN_TEST(test_greeting_is_the_same_string_every_time);
    RUN_TEST(test_thank_you_in_samoan);
    RUN_TEST(test_thank_you_in_tongan);
    RUN_TEST(test_thank_you_in_tok_pisin);
    RUN_TEST(test_thank_you_in_an_unknown_language);
    RUN_TEST(test_thank_you_for_a_negative_code);
    RUN_TEST(test_days_in_january);
    RUN_TEST(test_days_in_february);
    RUN_TEST(test_days_in_april);
    RUN_TEST(test_days_in_september);
    RUN_TEST(test_days_in_december);
    RUN_TEST(test_new_years_day_is_day_1);
    RUN_TEST(test_the_last_day_of_january);
    RUN_TEST(test_a_day_in_february);
    RUN_TEST(test_a_day_in_june);
    RUN_TEST(test_the_last_day_of_the_year);
    RUN_TEST(test_the_first_visitor_is_number_1);
    RUN_TEST(test_the_next_visitor_is_counted);
    RUN_TEST(test_three_more_visitors);
    RUN_TEST(test_a_busy_lunchtime);
    return UNITY_END();
}
