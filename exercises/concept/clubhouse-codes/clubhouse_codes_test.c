#include "vendor/unity.h"

#include <stdint.h>

extern uint32_t first_four(uint32_t code);
extern uint32_t add_knocks(uint32_t code, uint32_t extra);
extern uint32_t flip_knocks(uint32_t code, uint32_t chosen);
extern uint32_t remove_knocks(uint32_t code, uint32_t marked);
extern uint32_t matching_knocks(uint32_t heard, uint32_t code);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_keeps_the_first_four_beats(void) {
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b0110, first_four(0b10110110), "first_four(0b10110110)");
}

void test_a_code_of_only_four_beats_is_unchanged(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b1111, first_four(0b1111), "first_four(0b1111)");
}

void test_knocks_only_on_later_beats(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b0000, first_four(0b11110000), "first_four(0b11110000)");
}

void test_every_bit_set(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0xF, first_four(0xFFFFFFFF), "first_four(0xFFFFFFFF)");
}

// TASK: 2
void test_adds_new_knocks(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b01010011, add_knocks(0b00010011, 0b01000010), "add_knocks(0b00010011, 0b01000010)");
}

void test_adds_knocks_to_silence(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b00100100, add_knocks(0b00000000, 0b00100100), "add_knocks(0b00000000, 0b00100100)");
}

void test_knocks_already_in_the_code(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b00110001, add_knocks(0b00110000, 0b00010001), "add_knocks(0b00110000, 0b00010001)");
}

void test_no_extra_knocks(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b00010011, add_knocks(0b00010011, 0b00000000), "add_knocks(0b00010011, 0b00000000)");
}

// TASK: 3
void test_flips_two_beats(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b00010101, flip_knocks(0b00010011, 0b00000110), "flip_knocks(0b00010011, 0b00000110)");
}

void test_flips_every_beat(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b11101100, flip_knocks(0b00010011, 0b11111111), "flip_knocks(0b00010011, 0b11111111)");
}

void test_flips_knocks_into_pauses(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b10000010, flip_knocks(0b11000011, 0b01000001), "flip_knocks(0b11000011, 0b01000001)");
}

void test_flips_no_beats(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b00010011, flip_knocks(0b00010011, 0b00000000), "flip_knocks(0b00010011, 0b00000000)");
}

// TASK: 4
void test_removes_two_knocks(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b00000001, remove_knocks(0b00010011, 0b00010010), "remove_knocks(0b00010011, 0b00010010)");
}

void test_removes_the_first_beat(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b11111110, remove_knocks(0b11111111, 0b00000001), "remove_knocks(0b11111111, 0b00000001)");
}

void test_removes_every_knock(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b00000000, remove_knocks(0b10101010, 0b11111111), "remove_knocks(0b10101010, 0b11111111)");
}

void test_marked_beats_that_were_already_pauses(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b00010011, remove_knocks(0b00010011, 0b01001100), "remove_knocks(0b00010011, 0b01001100)");
}

void test_removes_some_marked_beats_but_not_others(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b01100000, remove_knocks(0b01110110, 0b00011111), "remove_knocks(0b01110110, 0b00011111)");
}

// TASK: 5
void test_some_beats_match(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b11111010, matching_knocks(0b00010011, 0b00010110), "matching_knocks(0b00010011, 0b00010110)");
}

void test_every_beat_matches(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b11111111, matching_knocks(0b00010011, 0b00010011), "matching_knocks(0b00010011, 0b00010011)");
}

void test_no_beat_matches(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b00000000, matching_knocks(0b01010101, 0b10101010), "matching_knocks(0b01010101, 0b10101010)");
}

void test_silence_at_the_door(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0b11101100, matching_knocks(0b00000000, 0b00010011), "matching_knocks(0b00000000, 0b00010011)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_keeps_the_first_four_beats);
    RUN_TEST(test_a_code_of_only_four_beats_is_unchanged);
    RUN_TEST(test_knocks_only_on_later_beats);
    RUN_TEST(test_every_bit_set);
    RUN_TEST(test_adds_new_knocks);
    RUN_TEST(test_adds_knocks_to_silence);
    RUN_TEST(test_knocks_already_in_the_code);
    RUN_TEST(test_no_extra_knocks);
    RUN_TEST(test_flips_two_beats);
    RUN_TEST(test_flips_every_beat);
    RUN_TEST(test_flips_knocks_into_pauses);
    RUN_TEST(test_flips_no_beats);
    RUN_TEST(test_removes_two_knocks);
    RUN_TEST(test_removes_the_first_beat);
    RUN_TEST(test_removes_every_knock);
    RUN_TEST(test_marked_beats_that_were_already_pauses);
    RUN_TEST(test_removes_some_marked_beats_but_not_others);
    RUN_TEST(test_some_beats_match);
    RUN_TEST(test_every_beat_matches);
    RUN_TEST(test_no_beat_matches);
    RUN_TEST(test_silence_at_the_door);
    return UNITY_END();
}
