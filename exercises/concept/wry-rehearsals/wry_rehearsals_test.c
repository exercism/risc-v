#include "vendor/unity.h"

#include <stdint.h>
#include <string.h>

extern int32_t line_length(const char *line);
extern int32_t count_char(const char *line, char ch);
extern int32_t first_word_length(const char *line);
extern int32_t is_shouted(const char *line);
extern void whisper(char *line);
extern int32_t copy_cue(char *dest, const char *src);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_length_of_a_wry_greeting(void) {
    TEST_ASSERT_EQUAL_INT32_MESSAGE(27, line_length("Oh good, another sunny day."), "line_length(\"Oh good, another sunny day.\")");
}

void test_length_of_a_one_word_line(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, line_length("Well"), "line_length(\"Well\")");
}

void test_length_of_a_single_character(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, line_length("?"), "line_length(\"?\")");
}

void test_length_of_an_empty_line(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, line_length(""), "line_length(\"\")");
}

void test_length_counts_every_byte(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(5, line_length("Caf\xC3\xA9"), "line_length(\"Caf\\xC3\\xA9\")");
}

// TASK: 2
void test_count_two_pauses(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, count_char("Lovely. / Just lovely. / Truly.", '/'),
                                    "count_char(\"Lovely. / Just lovely. / Truly.\", '/')");
}

void test_count_a_line_with_no_pauses(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, count_char("Splendid!", '/'), "count_char(\"Splendid!\", '/')");
}

void test_count_pauses_at_both_ends(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, count_char("/ Yes. // No.", '/'), "count_char(\"/ Yes. // No.\", '/')");
}

void test_count_in_an_empty_line(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, count_char("", '/'), "count_char(\"\", '/')");
}

void test_count_a_byte_above_127(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, count_char("Caf\xC3\xA9, caf\xC3\xA9", '\xA9'),
                                    "count_char(\"Caf\\xC3\\xA9, caf\\xC3\\xA9\", '\\xA9')");
}

// TASK: 3
void test_first_word_followed_by_a_comma(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(8, first_word_length("Perfect, the fish are only shy."),
                                    "first_word_length(\"Perfect, the fish are only shy.\")");
}

void test_first_word_of_a_one_word_line(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(6, first_word_length("Bravo!"), "first_word_length(\"Bravo!\")");
}

void test_first_word_of_a_line_starting_with_a_space(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, first_word_length(" and then"), "first_word_length(\" and then\")");
}

void test_first_word_of_an_empty_line(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, first_word_length(""), "first_word_length(\"\")");
}

void test_first_word_of_a_single_letter(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, first_word_length("I suppose"), "first_word_length(\"I suppose\")");
}

// TASK: 4
void test_a_shouted_line(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, is_shouted("WHAT A SURPRISE!"), "is_shouted(\"WHAT A SURPRISE!\")");
}

void test_a_spoken_line(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, is_shouted("What a surprise!"), "is_shouted(\"What a surprise!\")");
}

void test_letters_at_the_ends_of_the_alphabet(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, is_shouted("A-Z"), "is_shouted(\"A-Z\")");
}

void test_no_letters_is_not_a_shout(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, is_shouted("@[`{ 123 !"), "is_shouted(\"@[`{ 123 !\")");
}

void test_an_empty_line_is_not_a_shout(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, is_shouted(""), "is_shouted(\"\")");
}

// TASK: 5
void test_whisper_a_mixed_line(void) {
    TEST_IGNORE();
    char line[] = "HUSH, Uncle Bomai!";
    whisper(line);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("hush, uncle bomai!", line, "whisper(\"HUSH, Uncle Bomai!\")");
}

void test_whisper_a_line_already_quiet(void) {
    TEST_IGNORE();
    char line[] = "shh, here it lands";
    whisper(line);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("shh, here it lands", line, "whisper(\"shh, here it lands\")");
}

void test_whisper_the_whole_alphabet(void) {
    TEST_IGNORE();
    char line[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    whisper(line);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("abcdefghijklmnopqrstuvwxyz", line, "whisper(\"ABCDEFGHIJKLMNOPQRSTUVWXYZ\")");
}

void test_whisper_leaves_other_characters_alone(void) {
    TEST_IGNORE();
    char line[] = "@[`{ 42 ACT II!";
    whisper(line);
    TEST_ASSERT_EQUAL_STRING_MESSAGE("@[`{ 42 act ii!", line, "whisper(\"@[`{ 42 ACT II!\")");
}

void test_whisper_an_empty_line(void) {
    TEST_IGNORE();
    char line[] = "\0ZZ";
    whisper(line);
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE("\0ZZ", line, 4, "whisper(\"\") must not change the bytes after the end");
}

// TASK: 6
void test_copy_a_cue(void) {
    TEST_IGNORE();
    char notes[40];
    memset(notes, '#', sizeof notes - 1);
    notes[sizeof notes - 1] = 0;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(24, copy_cue(notes, "Has anyone seen my hook?"), "copy_cue(notes, \"Has anyone seen my hook?\")");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Has anyone seen my hook?", notes, "copy_cue(notes, \"Has anyone seen my hook?\")");
}

void test_copy_a_short_cue(void) {
    TEST_IGNORE();
    char notes[8];
    memset(notes, '#', sizeof notes - 1);
    notes[sizeof notes - 1] = 0;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, copy_cue(notes, "Go!"), "copy_cue(notes, \"Go!\")");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("Go!", notes, "copy_cue(notes, \"Go!\")");
}

void test_copy_stops_after_the_zero_byte(void) {
    TEST_IGNORE();
    char notes[8];
    memset(notes, '#', sizeof notes);
    copy_cue(notes, "Well");
    TEST_ASSERT_EQUAL_MEMORY_MESSAGE("Well\0###", notes, 8, "copy_cue(notes, \"Well\") must not write past the zero byte");
}

void test_copy_an_empty_cue(void) {
    TEST_IGNORE();
    char notes[4];
    memset(notes, '#', sizeof notes - 1);
    notes[sizeof notes - 1] = 0;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, copy_cue(notes, ""), "copy_cue(notes, \"\")");
    TEST_ASSERT_EQUAL_STRING_MESSAGE("", notes, "copy_cue(notes, \"\")");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_length_of_a_wry_greeting);
    RUN_TEST(test_length_of_a_one_word_line);
    RUN_TEST(test_length_of_a_single_character);
    RUN_TEST(test_length_of_an_empty_line);
    RUN_TEST(test_length_counts_every_byte);
    RUN_TEST(test_count_two_pauses);
    RUN_TEST(test_count_a_line_with_no_pauses);
    RUN_TEST(test_count_pauses_at_both_ends);
    RUN_TEST(test_count_in_an_empty_line);
    RUN_TEST(test_count_a_byte_above_127);
    RUN_TEST(test_first_word_followed_by_a_comma);
    RUN_TEST(test_first_word_of_a_one_word_line);
    RUN_TEST(test_first_word_of_a_line_starting_with_a_space);
    RUN_TEST(test_first_word_of_an_empty_line);
    RUN_TEST(test_first_word_of_a_single_letter);
    RUN_TEST(test_a_shouted_line);
    RUN_TEST(test_a_spoken_line);
    RUN_TEST(test_letters_at_the_ends_of_the_alphabet);
    RUN_TEST(test_no_letters_is_not_a_shout);
    RUN_TEST(test_an_empty_line_is_not_a_shout);
    RUN_TEST(test_whisper_a_mixed_line);
    RUN_TEST(test_whisper_a_line_already_quiet);
    RUN_TEST(test_whisper_the_whole_alphabet);
    RUN_TEST(test_whisper_leaves_other_characters_alone);
    RUN_TEST(test_whisper_an_empty_line);
    RUN_TEST(test_copy_a_cue);
    RUN_TEST(test_copy_a_short_cue);
    RUN_TEST(test_copy_stops_after_the_zero_byte);
    RUN_TEST(test_copy_an_empty_cue);
    return UNITY_END();
}
