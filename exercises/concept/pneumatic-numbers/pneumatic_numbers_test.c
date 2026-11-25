#include "vendor/unity.h"

#include <stdint.h>

typedef int32_t (*binary_op)(int32_t, int32_t);

extern char operator_symbol(int32_t op);
extern int32_t calculate(int32_t op, int32_t a, int32_t b);
extern int32_t run_program(const int32_t *codes, int32_t count, int32_t start);
extern int32_t apply(int32_t (*fn)(int32_t, int32_t), int32_t a, int32_t b);
extern binary_op operation(int32_t op);

void setUp(void) {
}

void tearDown(void) {
}

static int32_t larger(int32_t a, int32_t b) {
    return a > b ? a : b;
}

static int32_t second(int32_t a, int32_t b) {
    (void)a;
    return b;
}

static int32_t take_away(int32_t a, int32_t b) {
    return a - b;
}

static int32_t sum_of_squares(int32_t a, int32_t b) {
    return a * a + b * b;
}

// Calls the function that operation(op) returns, after checking that it looks like a code address.
static int32_t call_operation(int32_t op, int32_t a, int32_t b, const char *message) {
    binary_op fn = operation(op);
    TEST_ASSERT_TRUE_MESSAGE((uintptr_t)fn >= 4096, message);
    return fn(a, b);
}

// TASK: 1
void test_the_symbol_for_add(void) {
    TEST_ASSERT_EQUAL_INT32_MESSAGE('+', operator_symbol(0), "operator_symbol(0)");
}

void test_the_symbol_for_subtract(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE('-', operator_symbol(1), "operator_symbol(1)");
}

void test_the_symbols_for_multiply_and_divide(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE('*', operator_symbol(2), "operator_symbol(2)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE('/', operator_symbol(3), "operator_symbol(3)");
}

void test_the_symbol_for_a_code_past_the_end(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE('?', operator_symbol(4), "operator_symbol(4)");
}

void test_the_symbol_for_a_negative_code(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE('?', operator_symbol(-1), "operator_symbol(-1)");
}

// TASK: 2
void test_calculate_adds(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(19, calculate(0, 12, 7), "calculate(0, 12, 7)");
}

void test_calculate_subtracts(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(5, calculate(1, 12, 7), "calculate(1, 12, 7)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-5, calculate(1, 7, 12), "calculate(1, 7, 12)");
}

void test_calculate_multiplies(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(84, calculate(2, 12, 7), "calculate(2, 12, 7)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-36, calculate(2, -4, 9), "calculate(2, -4, 9)");
}

void test_calculate_divides(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, calculate(3, 30, 7), "calculate(3, 30, 7)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-4, calculate(3, -30, 7), "calculate(3, -30, 7)");
}

void test_calculate_divides_by_zero(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, calculate(3, 30, 0), "calculate(3, 30, 0)");
}

void test_calculate_with_an_unknown_code(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, calculate(4, 12, 7), "calculate(4, 12, 7)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, calculate(-1, 12, 7), "calculate(-1, 12, 7)");
}

// TASK: 3
void test_an_empty_tape(void) {
    TEST_IGNORE();
    const int32_t codes[] = {0};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(42, run_program(codes, 0, 42), "run_program({}, 0, 42)");
}

void test_add_one_and_double(void) {
    TEST_IGNORE();
    const int32_t codes[] = {0, 1, 0};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(9, run_program(codes, 3, 3), "run_program({0, 1, 0}, 3, 3)");
}

void test_change_sign(void) {
    TEST_IGNORE();
    const int32_t codes[] = {2, 0, 2};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, run_program(codes, 3, 5), "run_program({2, 0, 2}, 3, 5)");
}

void test_clear_then_count_up(void) {
    TEST_IGNORE();
    const int32_t codes[] = {1, 3, 0, 0, 1, 1};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(8, run_program(codes, 6, 100), "run_program({1, 3, 0, 0, 1, 1}, 6, 100)");
}

void test_unknown_codes_are_skipped(void) {
    TEST_IGNORE();
    const int32_t codes[] = {4, 0, -1, 1, 99, 2};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-22, run_program(codes, 6, 10), "run_program({4, 0, -1, 1, 99, 2}, 6, 10)");
}

// TASK: 4
void test_apply_larger(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(9, apply(larger, 4, 9), "apply(larger, 4, 9)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(9, apply(larger, 9, 4), "apply(larger, 9, 4)");
}

void test_apply_second(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(17, apply(second, 3, 17), "apply(second, 3, 17)");
}

void test_apply_keeps_the_order_of_the_numbers(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-15, apply(take_away, 5, 20), "apply(take_away, 5, 20)");
}

void test_apply_sum_of_squares(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(25, apply(sum_of_squares, 3, -4), "apply(sum_of_squares, 3, -4)");
}

// TASK: 5
void test_the_operation_for_add(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(19, call_operation(0, 12, 7, "operation(0)"), "operation(0)(12, 7)");
}

void test_the_operations_for_subtract_and_multiply(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(5, call_operation(1, 12, 7, "operation(1)"), "operation(1)(12, 7)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(84, call_operation(2, 12, 7, "operation(2)"), "operation(2)(12, 7)");
}

void test_the_operation_for_divide(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-4, call_operation(3, -30, 7, "operation(3)"), "operation(3)(-30, 7)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, call_operation(3, 30, 0, "operation(3)"), "operation(3)(30, 0)");
}

void test_unknown_codes_have_no_operation(void) {
    TEST_IGNORE();
    TEST_ASSERT_NULL_MESSAGE(operation(4), "operation(4)");
    TEST_ASSERT_NULL_MESSAGE(operation(-1), "operation(-1)");
}

void test_an_operation_can_be_applied(void) {
    TEST_IGNORE();
    binary_op fn = operation(1);
    TEST_ASSERT_TRUE_MESSAGE((uintptr_t)fn >= 4096, "operation(1)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-5, apply(fn, 7, 12), "apply(operation(1), 7, 12)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_the_symbol_for_add);
    RUN_TEST(test_the_symbol_for_subtract);
    RUN_TEST(test_the_symbols_for_multiply_and_divide);
    RUN_TEST(test_the_symbol_for_a_code_past_the_end);
    RUN_TEST(test_the_symbol_for_a_negative_code);
    RUN_TEST(test_calculate_adds);
    RUN_TEST(test_calculate_subtracts);
    RUN_TEST(test_calculate_multiplies);
    RUN_TEST(test_calculate_divides);
    RUN_TEST(test_calculate_divides_by_zero);
    RUN_TEST(test_calculate_with_an_unknown_code);
    RUN_TEST(test_an_empty_tape);
    RUN_TEST(test_add_one_and_double);
    RUN_TEST(test_change_sign);
    RUN_TEST(test_clear_then_count_up);
    RUN_TEST(test_unknown_codes_are_skipped);
    RUN_TEST(test_apply_larger);
    RUN_TEST(test_apply_second);
    RUN_TEST(test_apply_keeps_the_order_of_the_numbers);
    RUN_TEST(test_apply_sum_of_squares);
    RUN_TEST(test_the_operation_for_add);
    RUN_TEST(test_the_operations_for_subtract_and_multiply);
    RUN_TEST(test_the_operation_for_divide);
    RUN_TEST(test_unknown_codes_have_no_operation);
    RUN_TEST(test_an_operation_can_be_applied);
    return UNITY_END();
}
