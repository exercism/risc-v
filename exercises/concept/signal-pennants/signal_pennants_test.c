#include "vendor/unity.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern int32_t phonetic_length(char letter);
extern const char *phonetic_word(char letter);
extern uint32_t flag_colours(char letter);
extern int32_t colour_count(char letter);
extern int32_t spell_length(const char *message);

enum {
    WHITE = 1,
    BLUE = 2,
    RED = 4,
    YELLOW = 8,
    BLACK = 16,
};

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_alfa_has_4_characters(void) {
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, phonetic_length('A'), "phonetic_length('A')");
}

void test_november_is_the_longest_word(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(8, phonetic_length('N'), "phonetic_length('N')");
}

void test_x_ray_counts_its_hyphen(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(5, phonetic_length('X'), "phonetic_length('X')");
}

void test_zulu_is_the_last_letter(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, phonetic_length('Z'), "phonetic_length('Z')");
}

void test_characters_that_are_not_capital_letters(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, phonetic_length('@'), "phonetic_length('@')");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, phonetic_length('['), "phonetic_length('[')");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, phonetic_length('a'), "phonetic_length('a')");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, phonetic_length(' '), "phonetic_length(' ')");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, phonetic_length('7'), "phonetic_length('7')");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, phonetic_length(0), "phonetic_length(0)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, phonetic_length((char)200), "phonetic_length((char)200)");
}

// TASK: 2
void test_alfa(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Alfa", phonetic_word('A'), "phonetic_word('A')");
}

void test_juliett(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Juliett", phonetic_word('J'), "phonetic_word('J')");
}

void test_x_ray(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_STRING_MESSAGE("X-ray", phonetic_word('X'), "phonetic_word('X')");
}

void test_zulu(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Zulu", phonetic_word('Z'), "phonetic_word('Z')");
}

void test_every_word_starts_with_its_letter_and_fits_its_card(void) {
    TEST_IGNORE();
    char message[32];
    for (char letter = 'A'; letter <= 'Z'; letter++) {
        snprintf(message, sizeof message, "phonetic_word('%c')", letter);
        const char *word = phonetic_word(letter);
        TEST_ASSERT_EQUAL_INT32_MESSAGE(letter, word[0], message);
        TEST_ASSERT_EQUAL_INT32_MESSAGE(phonetic_length(letter), (int32_t)strlen(word), message);
    }
}

// TASK: 3
void test_alfa_is_white_and_blue(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(WHITE | BLUE, flag_colours('A'), "flag_colours('A')");
}

void test_oscar_is_red_and_yellow(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(RED | YELLOW, flag_colours('O'), "flag_colours('O')");
}

void test_quebec_is_yellow(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(YELLOW, flag_colours('Q'), "flag_colours('Q')");
}

void test_tango_is_red_white_and_blue(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(RED | WHITE | BLUE, flag_colours('T'), "flag_colours('T')");
}

void test_zulu_has_four_colours(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(YELLOW | BLUE | RED | BLACK, flag_colours('Z'), "flag_colours('Z')");
}

// TASK: 4
void test_bravo_needs_one_colour(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, colour_count('B'), "colour_count('B')");
}

void test_india_needs_two_colours(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, colour_count('I'), "colour_count('I')");
}

void test_whiskey_needs_three_colours(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, colour_count('W'), "colour_count('W')");
}

void test_zulu_needs_four_colours(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, colour_count('Z'), "colour_count('Z')");
}

void test_every_count_matches_the_colours(void) {
    TEST_IGNORE();
    char message[32];
    for (char letter = 'A'; letter <= 'Z'; letter++) {
        snprintf(message, sizeof message, "colour_count('%c')", letter);
        int32_t expected = __builtin_popcount(flag_colours(letter));
        TEST_ASSERT_EQUAL_INT32_MESSAGE(expected, colour_count(letter), message);
    }
}

// TASK: 5
void test_spelling_sos(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(17, spell_length("SOS"), "spell_length(\"SOS\")");
}

void test_spelling_an_empty_message(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, spell_length(""), "spell_length(\"\")");
}

void test_spelling_one_letter(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, spell_length("A"), "spell_length(\"A\")");
}

void test_spaces_add_nothing(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(60, spell_length("MAN OVERBOARD"), "spell_length(\"MAN OVERBOARD\")");
}

void test_digits_add_nothing(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(29, spell_length("RACE 2 AT 10"), "spell_length(\"RACE 2 AT 10\")");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_alfa_has_4_characters);
    RUN_TEST(test_november_is_the_longest_word);
    RUN_TEST(test_x_ray_counts_its_hyphen);
    RUN_TEST(test_zulu_is_the_last_letter);
    RUN_TEST(test_characters_that_are_not_capital_letters);
    RUN_TEST(test_alfa);
    RUN_TEST(test_juliett);
    RUN_TEST(test_x_ray);
    RUN_TEST(test_zulu);
    RUN_TEST(test_every_word_starts_with_its_letter_and_fits_its_card);
    RUN_TEST(test_alfa_is_white_and_blue);
    RUN_TEST(test_oscar_is_red_and_yellow);
    RUN_TEST(test_quebec_is_yellow);
    RUN_TEST(test_tango_is_red_white_and_blue);
    RUN_TEST(test_zulu_has_four_colours);
    RUN_TEST(test_bravo_needs_one_colour);
    RUN_TEST(test_india_needs_two_colours);
    RUN_TEST(test_whiskey_needs_three_colours);
    RUN_TEST(test_zulu_needs_four_colours);
    RUN_TEST(test_every_count_matches_the_colours);
    RUN_TEST(test_spelling_sos);
    RUN_TEST(test_spelling_an_empty_message);
    RUN_TEST(test_spelling_one_letter);
    RUN_TEST(test_spaces_add_nothing);
    RUN_TEST(test_digits_add_nothing);
    return UNITY_END();
}
