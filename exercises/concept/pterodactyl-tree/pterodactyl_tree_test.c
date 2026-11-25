#include "vendor/unity.h"

#include <stddef.h>
#include <stdint.h>

struct pterodactyl {
    int32_t wingspan;
    const struct pterodactyl *first_chick;
    const struct pterodactyl *second_chick;
};

extern int32_t colony_size(int32_t generations);
extern int32_t band_total(int32_t band);
extern int32_t climbing_routes(int32_t ledges);
extern int32_t count_family(const struct pterodactyl *root);
extern int32_t count_generations(const struct pterodactyl *root);
extern int32_t largest_wingspan(const struct pterodactyl *root);

// Provided for the student to call. Like any function, it may change every
// a and t register, so it does: a value kept in one of them is lost.
int32_t larger(int32_t a, int32_t b);

int32_t larger(int32_t a, int32_t b) {
    int32_t result = a > b ? a : b;
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
    return result;
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

// A pterodactyl with no chicks.
static const struct pterodactyl loner = {350, NULL, NULL};

// A parent with one chick.
static const struct pterodactyl only_chick = {280, NULL, NULL};
static const struct pterodactyl small_family = {410, &only_chick, NULL};

// The club's colony, four generations:
//
//   Balus 520 has chicks Kumul 480 and Tarangau 610.
//   Kumul has one chick, Kokomo 300.
//   Tarangau has chicks Muruk 450 and Pisin 390.
//   Muruk has one chick, Kakaruk 200.
static const struct pterodactyl kakaruk = {200, NULL, NULL};
static const struct pterodactyl kokomo = {300, NULL, NULL};
static const struct pterodactyl muruk = {450, &kakaruk, NULL};
static const struct pterodactyl pisin = {390, NULL, NULL};
static const struct pterodactyl kumul = {480, &kokomo, NULL};
static const struct pterodactyl tarangau = {610, &muruk, &pisin};
static const struct pterodactyl colony = {520, &kumul, &tarangau};

// The deepest line is through the first chick, and the widest wings are on
// the second chick.
static const struct pterodactyl grandchick = {250, NULL, NULL};
static const struct pterodactyl elder_chick = {300, &grandchick, NULL};
static const struct pterodactyl younger_chick = {500, NULL, NULL};
static const struct pterodactyl lopsided = {400, &elder_chick, &younger_chick};

// Five generations, each with one chick, and the youngest has the widest wings.
static const struct pterodactyl line_5 = {700, NULL, NULL};
static const struct pterodactyl line_4 = {330, NULL, &line_5};
static const struct pterodactyl line_3 = {340, NULL, &line_4};
static const struct pterodactyl line_2 = {350, NULL, &line_3};
static const struct pterodactyl line_1 = {360, NULL, &line_2};

// The parent has the widest wings.
static const struct pterodactyl small_twin = {290, NULL, NULL};
static const struct pterodactyl other_twin = {310, NULL, NULL};
static const struct pterodactyl big_parent = {640, &small_twin, &other_twin};

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_no_generations_is_just_the_founder(void) {
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, CHECKED(colony_size, 0), "colony_size(0)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "colony_size(0) must restore s0-s11 and sp");
}

void test_one_generation(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, CHECKED(colony_size, 1), "colony_size(1)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "colony_size(1) must restore s0-s11 and sp");
}

void test_two_generations(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(7, CHECKED(colony_size, 2), "colony_size(2)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "colony_size(2) must restore s0-s11 and sp");
}

void test_five_generations(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(63, CHECKED(colony_size, 5), "colony_size(5)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "colony_size(5) must restore s0-s11 and sp");
}

void test_ten_generations(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2047, CHECKED(colony_size, 10), "colony_size(10)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "colony_size(10) must restore s0-s11 and sp");
}

// TASK: 2
void test_a_two_digit_band(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(6, CHECKED(band_total, 42), "band_total(42)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "band_total(42) must restore s0-s11 and sp");
}

void test_a_single_digit_band(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(7, CHECKED(band_total, 7), "band_total(7)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "band_total(7) must restore s0-s11 and sp");
}

void test_band_zero(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, CHECKED(band_total, 0), "band_total(0)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "band_total(0) must restore s0-s11 and sp");
}

void test_a_band_with_zeros(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, CHECKED(band_total, 1000), "band_total(1000)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "band_total(1000) must restore s0-s11 and sp");
}

void test_a_five_digit_band(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(35, CHECKED(band_total, 98765), "band_total(98765)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "band_total(98765) must restore s0-s11 and sp");
}

// TASK: 3
void test_five_ledges(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(8, CHECKED(climbing_routes, 5), "climbing_routes(5)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "climbing_routes(5) must restore s0-s11 and sp");
}

void test_no_ledges(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, CHECKED(climbing_routes, 0), "climbing_routes(0)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "climbing_routes(0) must restore s0-s11 and sp");
}

void test_one_ledge(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, CHECKED(climbing_routes, 1), "climbing_routes(1)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "climbing_routes(1) must restore s0-s11 and sp");
}

void test_two_ledges(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, CHECKED(climbing_routes, 2), "climbing_routes(2)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "climbing_routes(2) must restore s0-s11 and sp");
}

void test_twenty_ledges(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(10946, CHECKED(climbing_routes, 20), "climbing_routes(20)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "climbing_routes(20) must restore s0-s11 and sp");
}

// TASK: 4
void test_nobody_in_an_empty_family(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, CHECKED(count_family, NULL), "count_family(NULL)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "count_family(NULL) must restore s0-s11 and sp");
}

void test_a_pterodactyl_with_no_chicks(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, CHECKED(count_family, &loner), "count_family(&loner)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "count_family(&loner) must restore s0-s11 and sp");
}

void test_a_parent_with_one_chick(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, CHECKED(count_family, &small_family), "count_family(&small_family)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "count_family(&small_family) must restore s0-s11 and sp");
}

void test_the_whole_colony(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(7, CHECKED(count_family, &colony), "count_family(&colony)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "count_family(&colony) must restore s0-s11 and sp");
}

void test_a_long_line(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(5, CHECKED(count_family, &line_1), "count_family(&line_1)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "count_family(&line_1) must restore s0-s11 and sp");
}

// TASK: 5
void test_no_generations_in_an_empty_family(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, CHECKED(count_generations, NULL), "count_generations(NULL)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "count_generations(NULL) must restore s0-s11 and sp");
}

void test_one_generation_with_no_chicks(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, CHECKED(count_generations, &loner), "count_generations(&loner)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "count_generations(&loner) must restore s0-s11 and sp");
}

void test_generations_in_the_colony(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, CHECKED(count_generations, &colony), "count_generations(&colony)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "count_generations(&colony) must restore s0-s11 and sp");
}

void test_the_deepest_line_is_the_first_chick(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, CHECKED(count_generations, &lopsided), "count_generations(&lopsided)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "count_generations(&lopsided) must restore s0-s11 and sp");
}

void test_generations_in_a_long_line(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(5, CHECKED(count_generations, &line_1), "count_generations(&line_1)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "count_generations(&line_1) must restore s0-s11 and sp");
}

// TASK: 6
void test_no_wings_in_an_empty_family(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, CHECKED(largest_wingspan, NULL), "largest_wingspan(NULL)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "largest_wingspan(NULL) must restore s0-s11 and sp");
}

void test_the_only_wingspan(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(350, CHECKED(largest_wingspan, &loner), "largest_wingspan(&loner)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "largest_wingspan(&loner) must restore s0-s11 and sp");
}

void test_the_widest_in_the_colony(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(610, CHECKED(largest_wingspan, &colony), "largest_wingspan(&colony)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "largest_wingspan(&colony) must restore s0-s11 and sp");
}

void test_the_widest_is_the_second_chick(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(500, CHECKED(largest_wingspan, &lopsided), "largest_wingspan(&lopsided)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "largest_wingspan(&lopsided) must restore s0-s11 and sp");
}

void test_the_widest_is_the_youngest(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(700, CHECKED(largest_wingspan, &line_1), "largest_wingspan(&line_1)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "largest_wingspan(&line_1) must restore s0-s11 and sp");
}

void test_the_widest_is_the_parent(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(640, CHECKED(largest_wingspan, &big_parent), "largest_wingspan(&big_parent)");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, clobbered_registers, "largest_wingspan(&big_parent) must restore s0-s11 and sp");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_no_generations_is_just_the_founder);
    RUN_TEST(test_one_generation);
    RUN_TEST(test_two_generations);
    RUN_TEST(test_five_generations);
    RUN_TEST(test_ten_generations);
    RUN_TEST(test_a_two_digit_band);
    RUN_TEST(test_a_single_digit_band);
    RUN_TEST(test_band_zero);
    RUN_TEST(test_a_band_with_zeros);
    RUN_TEST(test_a_five_digit_band);
    RUN_TEST(test_five_ledges);
    RUN_TEST(test_no_ledges);
    RUN_TEST(test_one_ledge);
    RUN_TEST(test_two_ledges);
    RUN_TEST(test_twenty_ledges);
    RUN_TEST(test_nobody_in_an_empty_family);
    RUN_TEST(test_a_pterodactyl_with_no_chicks);
    RUN_TEST(test_a_parent_with_one_chick);
    RUN_TEST(test_the_whole_colony);
    RUN_TEST(test_a_long_line);
    RUN_TEST(test_no_generations_in_an_empty_family);
    RUN_TEST(test_one_generation_with_no_chicks);
    RUN_TEST(test_generations_in_the_colony);
    RUN_TEST(test_the_deepest_line_is_the_first_chick);
    RUN_TEST(test_generations_in_a_long_line);
    RUN_TEST(test_no_wings_in_an_empty_family);
    RUN_TEST(test_the_only_wingspan);
    RUN_TEST(test_the_widest_in_the_colony);
    RUN_TEST(test_the_widest_is_the_second_chick);
    RUN_TEST(test_the_widest_is_the_youngest);
    RUN_TEST(test_the_widest_is_the_parent);
    return UNITY_END();
}
