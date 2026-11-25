#include "vendor/unity.h"

#include <stdint.h>

extern int32_t plank_length(int32_t boards, int32_t board_cm);
extern int32_t hull_length(int32_t boards, int32_t board_cm);
extern int32_t timber_needed(int32_t hull_boards, int32_t float_boards, int32_t board_cm);
extern int32_t fleet_length(int32_t canoes, int32_t boards, int32_t board_cm);
extern int32_t fleet_cost(int32_t canoes, int32_t boards, int32_t board_cm, int32_t price_per_metre);

// Provided for the student to call. Like any function, it may change every
// a and t register, so it does: a value kept in one of them is lost.
int32_t trim_board(int32_t cm);

int32_t trim_board(int32_t cm) {
    __asm__ volatile(
        "li t0, 0x0bad0000\n"
        "li t1, 0x0bad0001\n"
        "li t2, 0x0bad0002\n"
        "li t3, 0x0bad0003\n"
        "li t4, 0x0bad0004\n"
        "li t5, 0x0bad0005\n"
        "li t6, 0x0bad0006\n"
        "li a1, 0x0bad0011\n"
        "li a2, 0x0bad0012\n"
        "li a3, 0x0bad0013\n"
        "li a4, 0x0bad0014\n"
        "li a5, 0x0bad0015\n"
        "li a6, 0x0bad0016\n"
        "li a7, 0x0bad0017\n" ::
            : "t0", "t1", "t2", "t3", "t4", "t5", "t6", "a1", "a2", "a3", "a4", "a5", "a6", "a7");
    return cm - 5;
}

// call_checked(function, arguments...) calls function with up to four
// arguments and returns its result. Before the call it puts a known value in
// each of s0-s11. Afterwards it sets clobbered_registers: bit N is set if sN
// was not restored, and bit 12 is set if sp was not restored.
extern int32_t call_checked(void (*function)(void), ...);
extern uint32_t clobbered_registers;

__asm__(
    ".data\n"
    ".align 2\n"
    ".global clobbered_registers\n"
    "clobbered_registers: .word 0\n"
    "expected_sp: .word 0\n"
    "\n"
    ".macro set_s n\n"
    "    li s\\n, 0x5a5a0000 + \\n\n"
    ".endm\n"
    ".macro check_s n\n"
    "    li t1, 0x5a5a0000 + \\n\n"
    "    beq s\\n, t1, 1f\n"
    "    li t1, 1 << \\n\n"
    "    or t2, t2, t1\n"
    "1:\n"
    ".endm\n"
    "\n"
    ".text\n"
    ".global call_checked\n"
    "call_checked:\n"
    "    addi sp, sp, -64\n"
    "    sw ra, 60(sp)\n"
    "    sw s0, 56(sp)\n"
    "    sw s1, 52(sp)\n"
    "    sw s2, 48(sp)\n"
    "    sw s3, 44(sp)\n"
    "    sw s4, 40(sp)\n"
    "    sw s5, 36(sp)\n"
    "    sw s6, 32(sp)\n"
    "    sw s7, 28(sp)\n"
    "    sw s8, 24(sp)\n"
    "    sw s9, 20(sp)\n"
    "    sw s10, 16(sp)\n"
    "    sw s11, 12(sp)\n"
    "    la t0, expected_sp\n"
    "    sw sp, 0(t0)\n"
    "    mv t0, a0\n"  // the function to call
    "    mv a0, a1\n"  // shift its arguments into a0-a3
    "    mv a1, a2\n"
    "    mv a2, a3\n"
    "    mv a3, a4\n"
    "    set_s 0\n    set_s 1\n    set_s 2\n    set_s 3\n"
    "    set_s 4\n    set_s 5\n    set_s 6\n    set_s 7\n"
    "    set_s 8\n    set_s 9\n    set_s 10\n    set_s 11\n"
    "    jalr t0\n"
    "    li t2, 0\n"  // t2 collects the clobbered bits
    "    check_s 0\n    check_s 1\n    check_s 2\n    check_s 3\n"
    "    check_s 4\n    check_s 5\n    check_s 6\n    check_s 7\n"
    "    check_s 8\n    check_s 9\n    check_s 10\n    check_s 11\n"
    "    la t0, expected_sp\n"
    "    lw t0, 0(t0)\n"
    "    beq sp, t0, 2f\n"
    "    mv sp, t0\n"  // put sp back, so our own frame can be found
    "    li t1, 1 << 12\n"
    "    or t2, t2, t1\n"
    "2:\n"
    "    la t0, clobbered_registers\n"
    "    sw t2, 0(t0)\n"
    "    lw s11, 12(sp)\n"
    "    lw s10, 16(sp)\n"
    "    lw s9, 20(sp)\n"
    "    lw s8, 24(sp)\n"
    "    lw s7, 28(sp)\n"
    "    lw s6, 32(sp)\n"
    "    lw s5, 36(sp)\n"
    "    lw s4, 40(sp)\n"
    "    lw s3, 44(sp)\n"
    "    lw s2, 48(sp)\n"
    "    lw s1, 52(sp)\n"
    "    lw s0, 56(sp)\n"
    "    lw ra, 60(sp)\n"
    "    addi sp, sp, 64\n"
    "    ret\n");

#define CHECKED(function, ...) call_checked((void (*)(void))(function), __VA_ARGS__)

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_four_boards_make_a_plank(void) {
    TEST_ASSERT_EQUAL_INT32_MESSAGE(600, CHECKED(plank_length, 4, 150), "plank_length(4, 150)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "plank_length(4, 150) must restore s0-s11 and sp");
}

void test_a_single_board(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(200, CHECKED(plank_length, 1, 200), "plank_length(1, 200)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "plank_length(1, 200) must restore s0-s11 and sp");
}

void test_no_boards_make_no_plank(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, CHECKED(plank_length, 0, 150), "plank_length(0, 150)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "plank_length(0, 150) must restore s0-s11 and sp");
}

void test_twelve_short_boards(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1020, CHECKED(plank_length, 12, 85), "plank_length(12, 85)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "plank_length(12, 85) must restore s0-s11 and sp");
}

// TASK: 2
void test_a_hull_of_four_boards(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(640, CHECKED(hull_length, 4, 150), "hull_length(4, 150)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "hull_length(4, 150) must restore s0-s11 and sp");
}

void test_a_hull_of_one_board(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(255, CHECKED(hull_length, 1, 200), "hull_length(1, 200)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "hull_length(1, 200) must restore s0-s11 and sp");
}

void test_only_the_carved_pieces(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(60, CHECKED(hull_length, 0, 150), "hull_length(0, 150)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "hull_length(0, 150) must restore s0-s11 and sp");
}

void test_a_long_hull(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(960, CHECKED(hull_length, 10, 95), "hull_length(10, 95)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "hull_length(10, 95) must restore s0-s11 and sp");
}

// TASK: 3
void test_timber_for_hull_and_float(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(870, CHECKED(timber_needed, 4, 2, 150), "timber_needed(4, 2, 150)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "timber_needed(4, 2, 150) must restore s0-s11 and sp");
}

void test_timber_for_a_bigger_canoe(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1035, CHECKED(timber_needed, 6, 3, 120), "timber_needed(6, 3, 120)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "timber_needed(6, 3, 120) must restore s0-s11 and sp");
}

void test_a_hull_with_no_float(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(475, CHECKED(timber_needed, 5, 0, 100), "timber_needed(5, 0, 100)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "timber_needed(5, 0, 100) must restore s0-s11 and sp");
}

void test_float_as_long_as_the_hull(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(490, CHECKED(timber_needed, 1, 1, 250), "timber_needed(1, 1, 250)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "timber_needed(1, 1, 250) must restore s0-s11 and sp");
}

void test_no_timber_at_all(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, CHECKED(timber_needed, 0, 0, 150), "timber_needed(0, 0, 150)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "timber_needed(0, 0, 150) must restore s0-s11 and sp");
}

// TASK: 4
void test_a_fleet_of_three(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2355, CHECKED(fleet_length, 3, 4, 150), "fleet_length(3, 4, 150)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "fleet_length(3, 4, 150) must restore s0-s11 and sp");
}

void test_a_fleet_of_one(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(640, CHECKED(fleet_length, 1, 4, 150), "fleet_length(1, 4, 150)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "fleet_length(1, 4, 150) must restore s0-s11 and sp");
}

void test_a_fleet_of_four(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2490, CHECKED(fleet_length, 4, 6, 80), "fleet_length(4, 6, 80)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "fleet_length(4, 6, 80) must restore s0-s11 and sp");
}

void test_small_canoes(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(405, CHECKED(fleet_length, 2, 1, 100), "fleet_length(2, 1, 100)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "fleet_length(2, 1, 100) must restore s0-s11 and sp");
}

void test_no_canoes_ordered(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, CHECKED(fleet_length, 0, 4, 150), "fleet_length(0, 4, 150)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "fleet_length(0, 4, 150) must restore s0-s11 and sp");
}

// TASK: 5
void test_varnish_for_three_canoes(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(471, CHECKED(fleet_cost, 3, 4, 150, 20), "fleet_cost(3, 4, 150, 20)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "fleet_cost(3, 4, 150, 20) must restore s0-s11 and sp");
}

void test_varnish_for_one_canoe(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(64, CHECKED(fleet_cost, 1, 4, 150, 10), "fleet_cost(1, 4, 150, 10)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "fleet_cost(1, 4, 150, 10) must restore s0-s11 and sp");
}

void test_varnish_for_small_canoes(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(60, CHECKED(fleet_cost, 2, 1, 100, 15), "fleet_cost(2, 1, 100, 15)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "fleet_cost(2, 1, 100, 15) must restore s0-s11 and sp");
}

void test_the_price_is_rounded_down(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(26, CHECKED(fleet_cost, 1, 3, 111, 7), "fleet_cost(1, 3, 111, 7)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "fleet_cost(1, 3, 111, 7) must restore s0-s11 and sp");
}

void test_no_canoes_cost_nothing(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, CHECKED(fleet_cost, 0, 4, 150, 20), "fleet_cost(0, 4, 150, 20)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "fleet_cost(0, 4, 150, 20) must restore s0-s11 and sp");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_four_boards_make_a_plank);
    RUN_TEST(test_a_single_board);
    RUN_TEST(test_no_boards_make_no_plank);
    RUN_TEST(test_twelve_short_boards);
    RUN_TEST(test_a_hull_of_four_boards);
    RUN_TEST(test_a_hull_of_one_board);
    RUN_TEST(test_only_the_carved_pieces);
    RUN_TEST(test_a_long_hull);
    RUN_TEST(test_timber_for_hull_and_float);
    RUN_TEST(test_timber_for_a_bigger_canoe);
    RUN_TEST(test_a_hull_with_no_float);
    RUN_TEST(test_float_as_long_as_the_hull);
    RUN_TEST(test_no_timber_at_all);
    RUN_TEST(test_a_fleet_of_three);
    RUN_TEST(test_a_fleet_of_one);
    RUN_TEST(test_a_fleet_of_four);
    RUN_TEST(test_small_canoes);
    RUN_TEST(test_no_canoes_ordered);
    RUN_TEST(test_varnish_for_three_canoes);
    RUN_TEST(test_varnish_for_one_canoe);
    RUN_TEST(test_varnish_for_small_canoes);
    RUN_TEST(test_the_price_is_rounded_down);
    RUN_TEST(test_no_canoes_cost_nothing);
    return UNITY_END();
}
