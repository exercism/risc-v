#include "vendor/unity.h"

#include <stdint.h>

extern int32_t count_distinct_motifs(const int32_t *row, int32_t n);
extern int32_t most_common_motif(const int32_t *row, int32_t n);
extern int32_t dye_for_row(const int32_t *row, int32_t n);
extern void rotate_row(int32_t *row, int32_t n, int32_t k);
extern void smooth_row(int32_t *row, int32_t n);

// Provided for the student to call. tally[m] is how many times motif m
// appears, and kinds must be 16. It returns the spoonfuls of dye needed, or -1
// if kinds is wrong or sp is not a multiple of 16. Like any function, it may
// change every a and t register, so it does: a value kept in one of them is
// lost.
int32_t dye_needed(const int32_t *tally, int32_t kinds);

int32_t dye_needed(const int32_t *tally, int32_t kinds) {
    static const int32_t spoonfuls[16] = {0, 2, 3, 1, 2, 4, 1, 3, 2, 2, 5, 1, 3, 2, 4, 1};
    uint32_t sp;
    __asm__ volatile("mv %0, sp" : "=r"(sp));
    int32_t total = -1;
    if (kinds == 16 && sp % 16 == 0) {
        total = 0;
        for (int32_t m = 0; m < 16; m++) {
            total += tally[m] * spoonfuls[m];
        }
    }
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
    return total;
}

// Fills the stack below the caller with junk, so that a local array that is
// not set to zero holds junk too.
__attribute__((noinline)) static void dirty_stack(void) {
    volatile int32_t junk[512];
    for (int32_t i = 0; i < 512; i++) {
        junk[i] = 0x7a7a7a7a;
    }
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

#define CHECKED(function, ...) (dirty_stack(), call_checked((void (*)(void))(function), __VA_ARGS__))

static const int32_t long_row[] = {4,  8,  3, 10, 5,  0,  13, 13, 2,  3, 4, 10, 15, 14, 13, 6,  6, 10, 10, 10, 13, 2,
                                   15, 12, 2, 6,  7,  1,  6,  3,  2,  6, 8, 9,  9,  8,  5,  3,  0, 8,  7,  7,  7,  1,
                                   0,  9,  8, 4,  10, 14, 4,  8,  13, 5, 5, 12, 3,  4,  13, 11, 7, 3,  1,  6};

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_three_motifs_in_a_border(void) {
    const int32_t row[] = {1, 2, 1, 3, 2, 1};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, CHECKED(count_distinct_motifs, row, 6), "count_distinct_motifs({1, 2, 1, 3, 2, 1}, 6)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "count_distinct_motifs({1, 2, 1, 3, 2, 1}, 6) must restore s0-s11 and sp");
}

void test_a_single_motif(void) {
    TEST_IGNORE();
    const int32_t row[] = {7};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, CHECKED(count_distinct_motifs, row, 1), "count_distinct_motifs({7}, 1)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "count_distinct_motifs({7}, 1) must restore s0-s11 and sp");
}

void test_an_empty_row_has_no_motifs(void) {
    TEST_IGNORE();
    const int32_t row[] = {9};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, CHECKED(count_distinct_motifs, row, 0), "count_distinct_motifs({}, 0)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "count_distinct_motifs({}, 0) must restore s0-s11 and sp");
}

void test_plain_cloth_is_one_motif(void) {
    TEST_IGNORE();
    const int32_t row[] = {0, 0, 0, 0};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, CHECKED(count_distinct_motifs, row, 4), "count_distinct_motifs({0, 0, 0, 0}, 4)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "count_distinct_motifs({0, 0, 0, 0}, 4) must restore s0-s11 and sp");
}

void test_every_motif_in_a_long_row(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(16, CHECKED(count_distinct_motifs, long_row, 64), "count_distinct_motifs(long_row, 64)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "count_distinct_motifs(long_row, 64) must restore s0-s11 and sp");
}

// TASK: 2
void test_the_star_is_most_common(void) {
    TEST_IGNORE();
    const int32_t row[] = {4, 9, 4, 2, 9, 4};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, CHECKED(most_common_motif, row, 6), "most_common_motif({4, 9, 4, 2, 9, 4}, 6)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "most_common_motif({4, 9, 4, 2, 9, 4}, 6) must restore s0-s11 and sp");
}

void test_a_tie_goes_to_the_smaller_motif(void) {
    TEST_IGNORE();
    const int32_t row[] = {5, 3, 5, 3};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, CHECKED(most_common_motif, row, 4), "most_common_motif({5, 3, 5, 3}, 4)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "most_common_motif({5, 3, 5, 3}, 4) must restore s0-s11 and sp");
}

void test_a_row_of_one(void) {
    TEST_IGNORE();
    const int32_t row[] = {15};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(15, CHECKED(most_common_motif, row, 1), "most_common_motif({15}, 1)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "most_common_motif({15}, 1) must restore s0-s11 and sp");
}

void test_the_last_motif_wins(void) {
    TEST_IGNORE();
    const int32_t row[] = {0, 0, 15, 15, 15};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(15, CHECKED(most_common_motif, row, 5), "most_common_motif({0, 0, 15, 15, 15}, 5)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "most_common_motif({0, 0, 15, 15, 15}, 5) must restore s0-s11 and sp");
}

void test_most_common_in_a_long_row(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, CHECKED(most_common_motif, long_row, 64), "most_common_motif(long_row, 64)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "most_common_motif(long_row, 64) must restore s0-s11 and sp");
}

// TASK: 3
void test_dye_for_a_border(void) {
    TEST_IGNORE();
    const int32_t row[] = {1, 2, 1, 3, 2, 1};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(13, CHECKED(dye_for_row, row, 6), "dye_for_row({1, 2, 1, 3, 2, 1}, 6)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "dye_for_row({1, 2, 1, 3, 2, 1}, 6) must restore s0-s11 and sp");
}

void test_dye_for_a_short_row(void) {
    TEST_IGNORE();
    const int32_t row[] = {0, 5, 10, 5};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(13, CHECKED(dye_for_row, row, 4), "dye_for_row({0, 5, 10, 5}, 4)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "dye_for_row({0, 5, 10, 5}, 4) must restore s0-s11 and sp");
}

void test_dye_for_one_motif(void) {
    TEST_IGNORE();
    const int32_t row[] = {14};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, CHECKED(dye_for_row, row, 1), "dye_for_row({14}, 1)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "dye_for_row({14}, 1) must restore s0-s11 and sp");
}

void test_plain_cloth_needs_no_dye(void) {
    TEST_IGNORE();
    const int32_t row[] = {0, 0, 0};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, CHECKED(dye_for_row, row, 3), "dye_for_row({0, 0, 0}, 3)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "dye_for_row({0, 0, 0}, 3) must restore s0-s11 and sp");
}

void test_dye_for_a_long_row(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(148, CHECKED(dye_for_row, long_row, 64), "dye_for_row(long_row, 64)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "dye_for_row(long_row, 64) must restore s0-s11 and sp");
}

// TASK: 4
void test_rotate_by_two(void) {
    TEST_IGNORE();
    int32_t row[] = {1, 2, 3, 4, 5, 99};
    const int32_t expected[] = {4, 5, 1, 2, 3, 99};
    CHECKED(rotate_row, row, 5, 2);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 6, "rotate_row({1, 2, 3, 4, 5}, 5, 2)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "rotate_row({1, 2, 3, 4, 5}, 5, 2) must restore s0-s11 and sp");
}

void test_rotate_by_nothing(void) {
    TEST_IGNORE();
    int32_t row[] = {6, 2, 8, 99};
    const int32_t expected[] = {6, 2, 8, 99};
    CHECKED(rotate_row, row, 3, 0);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 4, "rotate_row({6, 2, 8}, 3, 0)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "rotate_row({6, 2, 8}, 3, 0) must restore s0-s11 and sp");
}

void test_rotate_by_a_whole_row(void) {
    TEST_IGNORE();
    int32_t row[] = {6, 2, 8, 1, 99};
    const int32_t expected[] = {6, 2, 8, 1, 99};
    CHECKED(rotate_row, row, 4, 4);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 5, "rotate_row({6, 2, 8, 1}, 4, 4)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "rotate_row({6, 2, 8, 1}, 4, 4) must restore s0-s11 and sp");
}

void test_rotate_further_than_the_row(void) {
    TEST_IGNORE();
    int32_t row[] = {1, 2, 3, 4, 5, 99};
    const int32_t expected[] = {4, 5, 1, 2, 3, 99};
    CHECKED(rotate_row, row, 5, 7);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 6, "rotate_row({1, 2, 3, 4, 5}, 5, 7)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "rotate_row({1, 2, 3, 4, 5}, 5, 7) must restore s0-s11 and sp");
}

void test_rotate_a_single_motif(void) {
    TEST_IGNORE();
    int32_t row[] = {11, 99};
    const int32_t expected[] = {11, 99};
    CHECKED(rotate_row, row, 1, 3);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 2, "rotate_row({11}, 1, 3)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "rotate_row({11}, 1, 3) must restore s0-s11 and sp");
}

void test_rotate_a_long_row(void) {
    TEST_IGNORE();
    int32_t row[] = {1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2, 5, 8, 11, 14, 1, 4, 7, 10, 99};
    const int32_t expected[] = {6, 9, 12, 15, 2, 5, 8, 11, 14, 1, 4, 7, 10, 1, 4, 7, 10, 13, 0, 3, 99};
    CHECKED(rotate_row, row, 20, 13);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 21,
                                          "rotate_row({1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2, 5, 8, 11, 14, 1, 4, 7, 10}, 20, 13)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(
        0, clobbered_registers,
        "rotate_row({1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2, 5, 8, 11, 14, 1, 4, 7, 10}, 20, 13) must restore s0-s11 and sp");
}

// TASK: 5
void test_lone_motifs_are_smoothed(void) {
    TEST_IGNORE();
    int32_t row[] = {1, 2, 1, 2, 1, 99};
    const int32_t expected[] = {1, 1, 2, 1, 1, 99};
    CHECKED(smooth_row, row, 5);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 6, "smooth_row({1, 2, 1, 2, 1}, 5)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "smooth_row({1, 2, 1, 2, 1}, 5) must restore s0-s11 and sp");
}

void test_a_gap_is_filled(void) {
    TEST_IGNORE();
    int32_t row[] = {4, 7, 4, 99};
    const int32_t expected[] = {4, 4, 4, 99};
    CHECKED(smooth_row, row, 3);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 4, "smooth_row({4, 7, 4}, 3)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "smooth_row({4, 7, 4}, 3) must restore s0-s11 and sp");
}

void test_a_plain_row_is_unchanged(void) {
    TEST_IGNORE();
    int32_t row[] = {3, 3, 3, 3, 99};
    const int32_t expected[] = {3, 3, 3, 3, 99};
    CHECKED(smooth_row, row, 4);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 5, "smooth_row({3, 3, 3, 3}, 4)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "smooth_row({3, 3, 3, 3}, 4) must restore s0-s11 and sp");
}

void test_the_original_neighbours_decide(void) {
    TEST_IGNORE();
    int32_t row[] = {2, 9, 2, 9, 2, 9, 2, 9, 99};
    const int32_t expected[] = {2, 2, 9, 2, 9, 2, 9, 9, 99};
    CHECKED(smooth_row, row, 8);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 9, "smooth_row({2, 9, 2, 9, 2, 9, 2, 9}, 8)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "smooth_row({2, 9, 2, 9, 2, 9, 2, 9}, 8) must restore s0-s11 and sp");
}

void test_the_ends_never_change(void) {
    TEST_IGNORE();
    int32_t row[] = {5, 8, 99};
    const int32_t expected[] = {5, 8, 99};
    CHECKED(smooth_row, row, 2);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 3, "smooth_row({5, 8}, 2)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "smooth_row({5, 8}, 2) must restore s0-s11 and sp");
}

void test_a_single_motif_is_unchanged(void) {
    TEST_IGNORE();
    int32_t row[] = {6, 99};
    const int32_t expected[] = {6, 99};
    CHECKED(smooth_row, row, 1);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, row, 2, "smooth_row({6}, 1)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "smooth_row({6}, 1) must restore s0-s11 and sp");
}
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_three_motifs_in_a_border);
    RUN_TEST(test_a_single_motif);
    RUN_TEST(test_an_empty_row_has_no_motifs);
    RUN_TEST(test_plain_cloth_is_one_motif);
    RUN_TEST(test_every_motif_in_a_long_row);
    RUN_TEST(test_the_star_is_most_common);
    RUN_TEST(test_a_tie_goes_to_the_smaller_motif);
    RUN_TEST(test_a_row_of_one);
    RUN_TEST(test_the_last_motif_wins);
    RUN_TEST(test_most_common_in_a_long_row);
    RUN_TEST(test_dye_for_a_border);
    RUN_TEST(test_dye_for_a_short_row);
    RUN_TEST(test_dye_for_one_motif);
    RUN_TEST(test_plain_cloth_needs_no_dye);
    RUN_TEST(test_dye_for_a_long_row);
    RUN_TEST(test_rotate_by_two);
    RUN_TEST(test_rotate_by_nothing);
    RUN_TEST(test_rotate_by_a_whole_row);
    RUN_TEST(test_rotate_further_than_the_row);
    RUN_TEST(test_rotate_a_single_motif);
    RUN_TEST(test_rotate_a_long_row);
    RUN_TEST(test_lone_motifs_are_smoothed);
    RUN_TEST(test_a_gap_is_filled);
    RUN_TEST(test_a_plain_row_is_unchanged);
    RUN_TEST(test_the_original_neighbours_decide);
    RUN_TEST(test_the_ends_never_change);
    RUN_TEST(test_a_single_motif_is_unchanged);
    return UNITY_END();
}
