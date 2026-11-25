#define _POSIX_C_SOURCE 200809L

#include "vendor/unity.h"

#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

extern int32_t script_length(const char *line);
extern int32_t count_pauses(const char *line);
extern int32_t count_capitals(const char *line);
extern void shout(char *line);
extern int32_t find_cue(const char *line, char marker);
extern void copy_line(char *dest, const char *src);

// Many lines in these tests have extra bytes after their zero byte.
// They would change the answer if the function read past the end of the line,
// and the tests check that the function did not change them.
// Output buffers are filled with SENTINEL past the end of the expected string.
#define SENTINEL '#'

// Copies text so that its zero byte is the very last byte of a page of memory.
// The next page cannot be read at all: touching it stops the program.
static char *at_page_end(const char *text) {
    size_t page = (size_t)sysconf(_SC_PAGESIZE);
    size_t size = strlen(text) + 1;
    size_t pages = (size + page - 1) / page + 1;
    char *start = mmap(NULL, pages * page, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    TEST_ASSERT_TRUE_MESSAGE(start != MAP_FAILED, "mmap failed");
    TEST_ASSERT_EQUAL_INT_MESSAGE(0, mprotect(start + (pages - 1) * page, page, PROT_NONE), "mprotect failed");
    char *line = start + (pages - 1) * page - size;
    memcpy(line, text, size);
    return line;
}

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_length_of_the_opening_line(void) {
    TEST_ASSERT_EQUAL_INT32_MESSAGE(29, script_length("Bula vinaka, welcome to Suva!"), "script_length(\"Bula vinaka, welcome to Suva!\")");
}

void test_length_of_an_empty_line(void) {
    TEST_IGNORE();
    const char line[] = "\0Hello";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, script_length(line), "script_length(\"\")");
}

void test_length_of_a_single_character(void) {
    TEST_IGNORE();
    const char line[] = "!\0Hello";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, script_length(line), "script_length(\"!\")");
}

void test_length_of_sixty_four_characters(void) {
    TEST_IGNORE();
    char line[70];
    memset(line, 'a', sizeof line);
    line[64] = 0;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(64, script_length(line), "script_length(line) with 64 'a' characters");
}

void test_length_of_a_long_line_at_the_end_of_memory(void) {
    TEST_IGNORE();
    char text[201];
    for (int32_t i = 0; i < 200; i++) {
        text[i] = (char)('a' + i % 26);
    }
    text[200] = 0;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(200, script_length(at_page_end(text)),
                                    "script_length(line) with 200 characters, and its zero byte at the end of readable memory");
}

// TASK: 2
void test_count_two_pauses(void) {
    TEST_IGNORE();
    const char line[] = "Bula! / Welcome / to the show.\0//";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, count_pauses(line), "count_pauses(\"Bula! / Welcome / to the show.\")");
}

void test_count_pauses_in_an_empty_line(void) {
    TEST_IGNORE();
    const char line[] = "\0///";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, count_pauses(line), "count_pauses(\"\")");
}

void test_count_seventeen_pauses_in_a_row(void) {
    TEST_IGNORE();
    char line[20];
    memset(line, '/', sizeof line);
    line[17] = 0;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(17, count_pauses(line), "count_pauses(line) with 17 '/' characters");
}

void test_count_pauses_in_a_hundred_characters(void) {
    TEST_IGNORE();
    char line[104];
    memset(line, '/', sizeof line);
    for (int32_t i = 0; i < 100; i++) {
        line[i] = i % 3 == 0 ? '/' : 'o';
    }
    line[100] = 0;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(34, count_pauses(line), "count_pauses(line) with 100 characters, a '/' at every index divisible by 3");
}

void test_count_pauses_at_the_end_of_memory(void) {
    TEST_IGNORE();
    char text[64];
    for (int32_t i = 0; i < 63; i++) {
        text[i] = i % 7 == 0 ? '/' : '-';
    }
    text[63] = 0;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(
        9, count_pauses(at_page_end(text)),
        "count_pauses(line) with 63 characters, a '/' at every index divisible by 7, and its zero byte at the end of readable memory");
}

// TASK: 3
void test_count_capitals_in_a_scene_heading(void) {
    TEST_IGNORE();
    const char line[] = "Act One, Scene Two: SUVA HARBOUR\0ZZ";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(15, count_capitals(line), "count_capitals(\"Act One, Scene Two: SUVA HARBOUR\")");
}

void test_count_capitals_next_to_the_alphabet(void) {
    TEST_IGNORE();
    const char line[] = "@AZ[`az{\0ZZ";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, count_capitals(line), "count_capitals(\"@AZ[`az{\")");
}

void test_count_capitals_among_large_bytes(void) {
    TEST_IGNORE();
    const char line[] =
        "\xC1\xC9"
        "B\xDA\x81\0ZZ";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, count_capitals(line), "count_capitals(\"\\xC1\\xC9B\\xDA\\x81\")");
}

void test_count_fifteen_capitals(void) {
    TEST_IGNORE();
    const char line[] = "ABCDEFGHIJKLMNO\0ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(15, count_capitals(line), "count_capitals(\"ABCDEFGHIJKLMNO\")");
}

void test_count_capitals_in_a_hundred_characters(void) {
    TEST_IGNORE();
    char line[104];
    memset(line, 'Z', sizeof line);
    int32_t expected = 0;
    for (int32_t i = 0; i < 100; i++) {
        line[i] = (char)(i % 4 == 0 ? 'A' + i % 26 : 'a' + i % 26);
        expected += i % 4 == 0;
    }
    line[100] = 0;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(expected, count_capitals(line),
                                    "count_capitals(line) with 100 letters, a capital at every index divisible by 4");
}

// TASK: 4
void test_shout_a_greeting(void) {
    TEST_IGNORE();
    char line[] = "bula, everyone!\0abc";
    const char expected[] = "BULA, EVERYONE!\0abc";
    shout(line);
    TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, line, sizeof line, "shout(\"bula, everyone!\")");
}

void test_shout_next_to_the_alphabet(void) {
    TEST_IGNORE();
    char line[] = "`az{@AZ[\xE1\xFA\0abc";
    const char expected[] = "`AZ{@AZ[\xE1\xFA\0abc";
    shout(line);
    TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, line, sizeof line, "shout(\"`az{@AZ[\\xE1\\xFA\")");
}

void test_shout_an_empty_line(void) {
    TEST_IGNORE();
    char line[] = "\0abc";
    const char expected[] = "\0abc";
    shout(line);
    TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, line, sizeof line, "shout(\"\")");
}

void test_shout_sixty_five_characters(void) {
    TEST_IGNORE();
    char line[72];
    char expected[72];
    memset(line, 'q', sizeof line);
    memset(expected, 'q', sizeof expected);
    for (int32_t i = 0; i < 65; i++) {
        line[i] = (char)(i % 2 == 0 ? 'a' + i % 26 : 'A' + i % 26);
        expected[i] = (char)('A' + i % 26);
    }
    line[65] = 0;
    expected[65] = 0;
    shout(line);
    TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, line, sizeof line,
                                         "shout(line) with 65 letters, lowercase at every even index and uppercase at every odd index");
}

void test_shout_a_hundred_characters_at_the_end_of_memory(void) {
    TEST_IGNORE();
    char text[101];
    char expected[101];
    for (int32_t i = 0; i < 100; i++) {
        text[i] = i % 5 == 4 ? ' ' : (char)('a' + i % 26);
        expected[i] = i % 5 == 4 ? ' ' : (char)('A' + i % 26);
    }
    text[100] = 0;
    expected[100] = 0;
    char *line = at_page_end(text);
    shout(line);
    TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, line, sizeof expected,
                                         "shout(line) with 100 characters, and its zero byte at the end of readable memory");
}

// TASK: 5
void test_find_the_drum_cue(void) {
    TEST_IGNORE();
    const char line[] = "Wait for the drum * then enter\0*";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(18, find_cue(line, '*'), "find_cue(\"Wait for the drum * then enter\", '*')");
}

void test_find_no_cue_in_a_short_line(void) {
    TEST_IGNORE();
    const char line[] = "Shh.\0****";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, find_cue(line, '*'), "find_cue(\"Shh.\", '*')");
}

void test_find_no_cue_in_an_empty_line(void) {
    TEST_IGNORE();
    const char line[] = "\0*";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, find_cue(line, '*'), "find_cue(\"\", '*')");
}

void test_find_a_large_marker_after_sixteen_characters(void) {
    TEST_IGNORE();
    char line[20];
    memset(line, '\xB6', sizeof line);
    for (int32_t i = 0; i < 16; i++) {
        line[i] = (char)('a' + i);
    }
    line[17] = 0;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(16, find_cue(line, '\xB6'), "find_cue(\"abcdefghijklmnop\\xB6\", '\\xB6')");
}

void test_find_a_cue_at_the_end_of_memory(void) {
    TEST_IGNORE();
    char text[201];
    memset(text, '.', sizeof text);
    text[150] = '~';
    text[180] = '~';
    text[200] = 0;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(
        150, find_cue(at_page_end(text), '~'),
        "find_cue(line, '~') with 200 characters, '~' at indexes 150 and 180, and its zero byte at the end of readable memory");
}

// TASK: 6
void test_copy_a_stage_direction(void) {
    TEST_IGNORE();
    const char src[] = "Exit, stage left.\0XYZ";
    char dest[24];
    memset(dest, SENTINEL, sizeof dest);
    const char expected[] = "Exit, stage left.\0######";
    copy_line(dest, src);
    TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, dest, sizeof dest, "copy_line(dest, \"Exit, stage left.\")");
}

void test_copy_an_empty_line(void) {
    TEST_IGNORE();
    const char src[] = "\0XYZ";
    char dest[5];
    memset(dest, SENTINEL, sizeof dest);
    const char expected[] = "\0####";
    copy_line(dest, src);
    TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, dest, sizeof dest, "copy_line(dest, \"\")");
}

void test_copy_sixteen_characters(void) {
    TEST_IGNORE();
    const char src[] = "M\xC4\x81l\xC5\x8D e lelei! \0XYZ";
    char dest[20];
    memset(dest, SENTINEL, sizeof dest);
    const char expected[] = "M\xC4\x81l\xC5\x8D e lelei! \0###";
    copy_line(dest, src);
    TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, dest, sizeof dest, "copy_line(dest, \"M\\xC4\\x81l\\xC5\\x8D e lelei! \")");
}

void test_copy_sixty_five_characters(void) {
    TEST_IGNORE();
    char src[70];
    char dest[72];
    char expected[72];
    memset(src, 'X', sizeof src);
    memset(dest, SENTINEL, sizeof dest);
    memset(expected, SENTINEL, sizeof expected);
    for (int32_t i = 0; i < 65; i++) {
        src[i] = (char)('A' + i % 26);
        expected[i] = src[i];
    }
    src[65] = 0;
    expected[65] = 0;
    copy_line(dest, src);
    TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, dest, sizeof dest, "copy_line(dest, src) with 65 characters in src");
}

void test_copy_a_long_line_from_the_end_of_memory(void) {
    TEST_IGNORE();
    char text[201];
    char dest[208];
    char expected[208];
    memset(dest, SENTINEL, sizeof dest);
    memset(expected, SENTINEL, sizeof expected);
    for (int32_t i = 0; i < 200; i++) {
        text[i] = (char)(' ' + i % 95);
        expected[i] = text[i];
    }
    text[200] = 0;
    expected[200] = 0;
    copy_line(dest, at_page_end(text));
    TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(
        expected, dest, sizeof dest, "copy_line(dest, src) with 200 characters in src, and its zero byte at the end of readable memory");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_length_of_the_opening_line);
    RUN_TEST(test_length_of_an_empty_line);
    RUN_TEST(test_length_of_a_single_character);
    RUN_TEST(test_length_of_sixty_four_characters);
    RUN_TEST(test_length_of_a_long_line_at_the_end_of_memory);
    RUN_TEST(test_count_two_pauses);
    RUN_TEST(test_count_pauses_in_an_empty_line);
    RUN_TEST(test_count_seventeen_pauses_in_a_row);
    RUN_TEST(test_count_pauses_in_a_hundred_characters);
    RUN_TEST(test_count_pauses_at_the_end_of_memory);
    RUN_TEST(test_count_capitals_in_a_scene_heading);
    RUN_TEST(test_count_capitals_next_to_the_alphabet);
    RUN_TEST(test_count_capitals_among_large_bytes);
    RUN_TEST(test_count_fifteen_capitals);
    RUN_TEST(test_count_capitals_in_a_hundred_characters);
    RUN_TEST(test_shout_a_greeting);
    RUN_TEST(test_shout_next_to_the_alphabet);
    RUN_TEST(test_shout_an_empty_line);
    RUN_TEST(test_shout_sixty_five_characters);
    RUN_TEST(test_shout_a_hundred_characters_at_the_end_of_memory);
    RUN_TEST(test_find_the_drum_cue);
    RUN_TEST(test_find_no_cue_in_a_short_line);
    RUN_TEST(test_find_no_cue_in_an_empty_line);
    RUN_TEST(test_find_a_large_marker_after_sixteen_characters);
    RUN_TEST(test_find_a_cue_at_the_end_of_memory);
    RUN_TEST(test_copy_a_stage_direction);
    RUN_TEST(test_copy_an_empty_line);
    RUN_TEST(test_copy_sixteen_characters);
    RUN_TEST(test_copy_sixty_five_characters);
    RUN_TEST(test_copy_a_long_line_from_the_end_of_memory);
    return UNITY_END();
}
