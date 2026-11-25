#include "vendor/unity.h"

#include <stdint.h>

extern uint32_t octave_up(uint32_t frequency, uint32_t octaves);
extern uint32_t octave_down(uint32_t frequency, uint32_t octaves);
extern uint32_t note_octave(uint32_t note);
extern uint32_t note_duration(uint32_t note);
extern uint32_t pack_note(uint32_t pitch, uint32_t octave, uint32_t duration);
extern int32_t offset_after_turns(int32_t cents, uint32_t turns);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_one_octave_up(void) {
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(880, octave_up(440, 1), "octave_up(440, 1)");
}

void test_three_octaves_up(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(880, octave_up(110, 3), "octave_up(110, 3)");
}

void test_no_octaves_up(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(55, octave_up(55, 0), "octave_up(55, 0)");
}

void test_two_octaves_up_from_b(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(988, octave_up(247, 2), "octave_up(247, 2)");
}

void test_seven_octaves_up(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(3456, octave_up(27, 7), "octave_up(27, 7)");
}

// TASK: 2
void test_three_octaves_down(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(55, octave_down(440, 3), "octave_down(440, 3)");
}

void test_one_octave_down(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(220, octave_down(440, 1), "octave_down(440, 1)");
}

void test_one_octave_down_rounds_down(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(123, octave_down(247, 1), "octave_down(247, 1)");
}

void test_no_octaves_down(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(440, octave_down(440, 0), "octave_down(440, 0)");
}

void test_a_very_high_frequency_stays_positive(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0x08000000, octave_down(0x80000000, 4), "octave_down(0x80000000, 4)");
}

// TASK: 3
void test_octave_of_a_b_in_bar_3(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(4, note_octave(0x0003240B), "note_octave(0x0003240B)");
}

void test_octave_of_a_c_in_bar_1(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(5, note_octave(0x00011500), "note_octave(0x00011500)");
}

void test_octave_of_a_long_note(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(6, note_octave(0x0040F60B), "note_octave(0x0040F60B)");
}

void test_octave_zero(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, note_octave(0x0007F009), "note_octave(0x0007F009)");
}

void test_highest_octave(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(15, note_octave(0xFFFFFF0B), "note_octave(0xFFFFFF0B)");
}

// TASK: 4
void test_duration_of_a_b_in_bar_3(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(2, note_duration(0x0003240B), "note_duration(0x0003240B)");
}

void test_duration_of_a_long_note(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(15, note_duration(0x0040F60B), "note_duration(0x0040F60B)");
}

void test_duration_of_one_beat(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(1, note_duration(0x00011500), "note_duration(0x00011500)");
}

void test_duration_in_the_last_bar(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(4, note_duration(0xFFFF4300), "note_duration(0xFFFF4300)");
}

// TASK: 5
void test_pack_an_a(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0x00002409, pack_note(9, 4, 2), "pack_note(9, 4, 2)");
}

void test_pack_a_b(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0x0000150B, pack_note(11, 5, 1), "pack_note(11, 5, 1)");
}

void test_pack_a_c(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0x00004300, pack_note(0, 3, 4), "pack_note(0, 3, 4)");
}

void test_pack_the_largest_fields(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0x0000FF0B, pack_note(11, 15, 15), "pack_note(11, 15, 15)");
}

void test_pack_all_zeros(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0x00000000, pack_note(0, 0, 0), "pack_note(0, 0, 0)");
}

// TASK: 6
void test_flat_string_after_three_turns(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-5, offset_after_turns(-40, 3), "offset_after_turns(-40, 3)");
}

void test_flat_string_after_one_turn(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-20, offset_after_turns(-40, 1), "offset_after_turns(-40, 1)");
}

void test_sharp_string_after_two_turns(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(9, offset_after_turns(36, 2), "offset_after_turns(36, 2)");
}

void test_odd_flat_offset_rounds_down(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-3, offset_after_turns(-5, 1), "offset_after_turns(-5, 1)");
}

void test_odd_sharp_offset_rounds_down(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, offset_after_turns(5, 1), "offset_after_turns(5, 1)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_one_octave_up);
    RUN_TEST(test_three_octaves_up);
    RUN_TEST(test_no_octaves_up);
    RUN_TEST(test_two_octaves_up_from_b);
    RUN_TEST(test_seven_octaves_up);
    RUN_TEST(test_three_octaves_down);
    RUN_TEST(test_one_octave_down);
    RUN_TEST(test_one_octave_down_rounds_down);
    RUN_TEST(test_no_octaves_down);
    RUN_TEST(test_a_very_high_frequency_stays_positive);
    RUN_TEST(test_octave_of_a_b_in_bar_3);
    RUN_TEST(test_octave_of_a_c_in_bar_1);
    RUN_TEST(test_octave_of_a_long_note);
    RUN_TEST(test_octave_zero);
    RUN_TEST(test_highest_octave);
    RUN_TEST(test_duration_of_a_b_in_bar_3);
    RUN_TEST(test_duration_of_a_long_note);
    RUN_TEST(test_duration_of_one_beat);
    RUN_TEST(test_duration_in_the_last_bar);
    RUN_TEST(test_pack_an_a);
    RUN_TEST(test_pack_a_b);
    RUN_TEST(test_pack_a_c);
    RUN_TEST(test_pack_the_largest_fields);
    RUN_TEST(test_pack_all_zeros);
    RUN_TEST(test_flat_string_after_three_turns);
    RUN_TEST(test_flat_string_after_one_turn);
    RUN_TEST(test_sharp_string_after_two_turns);
    RUN_TEST(test_odd_flat_offset_rounds_down);
    RUN_TEST(test_odd_sharp_offset_rounds_down);
    return UNITY_END();
}
