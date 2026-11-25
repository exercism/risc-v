#include "vendor/unity.h"

#include <stdint.h>

extern void fill_trays(int32_t *trays, int32_t count, int32_t portions);
extern void add_extra(int32_t *trays, int32_t count, int32_t extra);
extern void scale_recipe(int32_t *grams, int32_t count, int32_t batches);
extern void add_portions(int32_t *totals, const int32_t *first, const int32_t *second, int32_t count);
extern void leftovers(int32_t *left, const int32_t *made, const int32_t *eaten, int32_t count);
extern void room_on_trays(int32_t *room, const int32_t *on_tray, int32_t count, int32_t capacity);

// Every array in these tests has one extra element after the last one the function is given.
// It holds SENTINEL, and the tests check that the function did not change it.
#define SENTINEL 0x5EA5EA

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_fill_five_trays(void) {
    int32_t trays[] = {0, 0, 0, 0, 0, SENTINEL};
    const int32_t expected[] = {12, 12, 12, 12, 12, SENTINEL};
    fill_trays(trays, 5, 12);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, trays, 6, "fill_trays(trays, 5, 12)");
}

void test_fill_one_tray(void) {
    TEST_IGNORE();
    int32_t trays[] = {3, SENTINEL};
    const int32_t expected[] = {8, SENTINEL};
    fill_trays(trays, 1, 8);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, trays, 2, "fill_trays(trays, 1, 8)");
}

void test_fill_no_trays(void) {
    TEST_IGNORE();
    int32_t trays[] = {SENTINEL};
    const int32_t expected[] = {SENTINEL};
    fill_trays(trays, 0, 8);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, trays, 1, "fill_trays(trays, 0, 8)");
}

void test_fill_seventeen_trays(void) {
    TEST_IGNORE();
    int32_t trays[18];
    int32_t expected[18];
    for (int32_t i = 0; i < 17; i++) {
        trays[i] = i;
        expected[i] = 6;
    }
    trays[17] = SENTINEL;
    expected[17] = SENTINEL;
    fill_trays(trays, 17, 6);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, trays, 18, "fill_trays(trays, 17, 6)");
}

void test_fill_a_hundred_trays(void) {
    TEST_IGNORE();
    int32_t trays[101];
    int32_t expected[101];
    for (int32_t i = 0; i < 100; i++) {
        trays[i] = 0;
        expected[i] = 25;
    }
    trays[100] = SENTINEL;
    expected[100] = SENTINEL;
    fill_trays(trays, 100, 25);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, trays, 101, "fill_trays(trays, 100, 25)");
}

// TASK: 2
void test_extra_portions_on_four_trays(void) {
    TEST_IGNORE();
    int32_t trays[] = {10, 12, 9, 11, SENTINEL};
    const int32_t expected[] = {13, 15, 12, 14, SENTINEL};
    add_extra(trays, 4, 3);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, trays, 5, "add_extra(trays, 4, 3) with trays = {10, 12, 9, 11}");
}

void test_no_extra_portions(void) {
    TEST_IGNORE();
    int32_t trays[] = {7, 0, 4, SENTINEL};
    const int32_t expected[] = {7, 0, 4, SENTINEL};
    add_extra(trays, 3, 0);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, trays, 4, "add_extra(trays, 3, 0) with trays = {7, 0, 4}");
}

void test_extra_portions_on_seventeen_trays(void) {
    TEST_IGNORE();
    int32_t trays[18];
    int32_t expected[18];
    for (int32_t i = 0; i < 17; i++) {
        trays[i] = i;
        expected[i] = i + 5;
    }
    trays[17] = SENTINEL;
    expected[17] = SENTINEL;
    add_extra(trays, 17, 5);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, trays, 18, "add_extra(trays, 17, 5) with trays = {0, 1, 2, ..., 16}");
}

void test_one_portion_taken_from_a_hundred_trays(void) {
    TEST_IGNORE();
    int32_t trays[101];
    int32_t expected[101];
    for (int32_t i = 0; i < 100; i++) {
        trays[i] = 10 + i % 7;
        expected[i] = 9 + i % 7;
    }
    trays[100] = SENTINEL;
    expected[100] = SENTINEL;
    add_extra(trays, 100, -1);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, trays, 101, "add_extra(trays, 100, -1) with trays[i] = 10 + i % 7");
}

void test_extra_portions_with_no_trays(void) {
    TEST_IGNORE();
    int32_t trays[] = {SENTINEL};
    const int32_t expected[] = {SENTINEL};
    add_extra(trays, 0, 3);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, trays, 1, "add_extra(trays, 0, 3)");
}

// TASK: 3
void test_scale_five_ingredients_for_four_batches(void) {
    TEST_IGNORE();
    int32_t grams[] = {1000, 300, 50, 20, 5, SENTINEL};
    const int32_t expected[] = {4000, 1200, 200, 80, 20, SENTINEL};
    scale_recipe(grams, 5, 4);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, grams, 6, "scale_recipe(grams, 5, 4) with grams = {1000, 300, 50, 20, 5}");
}

void test_scale_one_ingredient_for_one_batch(void) {
    TEST_IGNORE();
    int32_t grams[] = {250, SENTINEL};
    const int32_t expected[] = {250, SENTINEL};
    scale_recipe(grams, 1, 1);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, grams, 2, "scale_recipe(grams, 1, 1) with grams = {250}");
}

void test_scale_three_ingredients_for_ten_batches(void) {
    TEST_IGNORE();
    int32_t grams[] = {750, 225, 15, SENTINEL};
    const int32_t expected[] = {7500, 2250, 150, SENTINEL};
    scale_recipe(grams, 3, 10);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, grams, 4, "scale_recipe(grams, 3, 10) with grams = {750, 225, 15}");
}

void test_scale_seventeen_ingredients_for_three_batches(void) {
    TEST_IGNORE();
    int32_t grams[18];
    int32_t expected[18];
    for (int32_t i = 0; i < 17; i++) {
        grams[i] = 10 * i + 5;
        expected[i] = 30 * i + 15;
    }
    grams[17] = SENTINEL;
    expected[17] = SENTINEL;
    scale_recipe(grams, 17, 3);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, grams, 18, "scale_recipe(grams, 17, 3) with grams[i] = 10 * i + 5");
}

void test_scale_no_ingredients(void) {
    TEST_IGNORE();
    int32_t grams[] = {SENTINEL};
    const int32_t expected[] = {SENTINEL};
    scale_recipe(grams, 0, 6);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, grams, 1, "scale_recipe(grams, 0, 6)");
}

// TASK: 4
void test_add_portions_for_four_sauces(void) {
    TEST_IGNORE();
    const int32_t first[] = {14, 9, 20, 6};
    const int32_t second[] = {11, 15, 0, 8};
    int32_t totals[] = {0, 0, 0, 0, SENTINEL};
    const int32_t expected[] = {25, 24, 20, 14, SENTINEL};
    add_portions(totals, first, second, 4);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, totals, 5, "add_portions(totals, {14, 9, 20, 6}, {11, 15, 0, 8}, 4)");
}

void test_add_portions_for_one_sauce(void) {
    TEST_IGNORE();
    const int32_t first[] = {30};
    const int32_t second[] = {12};
    int32_t totals[] = {0, SENTINEL};
    const int32_t expected[] = {42, SENTINEL};
    add_portions(totals, first, second, 1);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, totals, 2, "add_portions(totals, {30}, {12}, 1)");
}

void test_add_portions_for_seventeen_sauces(void) {
    TEST_IGNORE();
    int32_t first[17];
    int32_t second[17];
    int32_t totals[18];
    int32_t expected[18];
    for (int32_t i = 0; i < 17; i++) {
        first[i] = i;
        second[i] = 100 * i;
        totals[i] = 0;
        expected[i] = 101 * i;
    }
    totals[17] = SENTINEL;
    expected[17] = SENTINEL;
    add_portions(totals, first, second, 17);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, totals, 18,
                                          "add_portions(totals, first, second, 17) with first[i] = i, second[i] = 100 * i");
}

void test_add_portions_for_a_hundred_sauces(void) {
    TEST_IGNORE();
    int32_t first[100];
    int32_t second[100];
    int32_t totals[101];
    int32_t expected[101];
    for (int32_t i = 0; i < 100; i++) {
        first[i] = 3 * i;
        second[i] = 200 - i;
        totals[i] = 0;
        expected[i] = 200 + 2 * i;
    }
    totals[100] = SENTINEL;
    expected[100] = SENTINEL;
    add_portions(totals, first, second, 100);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, totals, 101,
                                          "add_portions(totals, first, second, 100) with first[i] = 3 * i, second[i] = 200 - i");
}

void test_add_portions_for_no_sauces(void) {
    TEST_IGNORE();
    const int32_t first[] = {SENTINEL};
    const int32_t second[] = {SENTINEL};
    int32_t totals[] = {SENTINEL};
    const int32_t expected[] = {SENTINEL};
    add_portions(totals, first, second, 0);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, totals, 1, "add_portions(totals, {}, {}, 0)");
}

// TASK: 5
void test_leftovers_for_five_sauces(void) {
    TEST_IGNORE();
    const int32_t made[] = {40, 25, 30, 18, 12};
    const int32_t eaten[] = {37, 25, 21, 2, 12};
    int32_t left[] = {0, 0, 0, 0, 0, SENTINEL};
    const int32_t expected[] = {3, 0, 9, 16, 0, SENTINEL};
    leftovers(left, made, eaten, 5);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, left, 6, "leftovers(left, {40, 25, 30, 18, 12}, {37, 25, 21, 2, 12}, 5)");
}

void test_no_leftovers(void) {
    TEST_IGNORE();
    const int32_t made[] = {20, 20, 20};
    const int32_t eaten[] = {20, 20, 20};
    int32_t left[] = {-1, -1, -1, SENTINEL};
    const int32_t expected[] = {0, 0, 0, SENTINEL};
    leftovers(left, made, eaten, 3);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, left, 4, "leftovers(left, {20, 20, 20}, {20, 20, 20}, 3)");
}

void test_leftovers_for_seventeen_sauces(void) {
    TEST_IGNORE();
    int32_t made[17];
    int32_t eaten[17];
    int32_t left[18];
    int32_t expected[18];
    for (int32_t i = 0; i < 17; i++) {
        made[i] = 50;
        eaten[i] = 2 * i;
        left[i] = 0;
        expected[i] = 50 - 2 * i;
    }
    left[17] = SENTINEL;
    expected[17] = SENTINEL;
    leftovers(left, made, eaten, 17);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, left, 18, "leftovers(left, made, eaten, 17) with made[i] = 50, eaten[i] = 2 * i");
}

void test_leftovers_for_a_hundred_sauces(void) {
    TEST_IGNORE();
    int32_t made[100];
    int32_t eaten[100];
    int32_t left[101];
    int32_t expected[101];
    for (int32_t i = 0; i < 100; i++) {
        made[i] = 100 + i;
        eaten[i] = 100 - i;
        left[i] = 0;
        expected[i] = 2 * i;
    }
    left[100] = SENTINEL;
    expected[100] = SENTINEL;
    leftovers(left, made, eaten, 100);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, left, 101,
                                          "leftovers(left, made, eaten, 100) with made[i] = 100 + i, eaten[i] = 100 - i");
}

void test_leftovers_for_no_sauces(void) {
    TEST_IGNORE();
    const int32_t made[] = {SENTINEL};
    const int32_t eaten[] = {SENTINEL};
    int32_t left[] = {SENTINEL};
    const int32_t expected[] = {SENTINEL};
    leftovers(left, made, eaten, 0);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, left, 1, "leftovers(left, {}, {}, 0)");
}

// TASK: 6
void test_room_on_four_trays(void) {
    TEST_IGNORE();
    const int32_t on_tray[] = {12, 3, 20, 0};
    int32_t room[] = {0, 0, 0, 0, SENTINEL};
    const int32_t expected[] = {8, 17, 0, 20, SENTINEL};
    room_on_trays(room, on_tray, 4, 20);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, room, 5, "room_on_trays(room, {12, 3, 20, 0}, 4, 20)");
}

void test_no_room_on_a_full_tray(void) {
    TEST_IGNORE();
    const int32_t on_tray[] = {24};
    int32_t room[] = {-1, SENTINEL};
    const int32_t expected[] = {0, SENTINEL};
    room_on_trays(room, on_tray, 1, 24);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, room, 2, "room_on_trays(room, {24}, 1, 24)");
}

void test_room_on_seventeen_trays(void) {
    TEST_IGNORE();
    int32_t on_tray[17];
    int32_t room[18];
    int32_t expected[18];
    for (int32_t i = 0; i < 17; i++) {
        on_tray[i] = i;
        room[i] = 0;
        expected[i] = 30 - i;
    }
    room[17] = SENTINEL;
    expected[17] = SENTINEL;
    room_on_trays(room, on_tray, 17, 30);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, room, 18, "room_on_trays(room, on_tray, 17, 30) with on_tray[i] = i");
}

void test_room_on_a_hundred_trays(void) {
    TEST_IGNORE();
    int32_t on_tray[100];
    int32_t room[101];
    int32_t expected[101];
    for (int32_t i = 0; i < 100; i++) {
        on_tray[i] = i % 25;
        room[i] = 0;
        expected[i] = 25 - i % 25;
    }
    room[100] = SENTINEL;
    expected[100] = SENTINEL;
    room_on_trays(room, on_tray, 100, 25);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, room, 101, "room_on_trays(room, on_tray, 100, 25) with on_tray[i] = i % 25");
}

void test_room_on_no_trays(void) {
    TEST_IGNORE();
    const int32_t on_tray[] = {SENTINEL};
    int32_t room[] = {SENTINEL};
    const int32_t expected[] = {SENTINEL};
    room_on_trays(room, on_tray, 0, 25);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, room, 1, "room_on_trays(room, {}, 0, 25)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_fill_five_trays);
    RUN_TEST(test_fill_one_tray);
    RUN_TEST(test_fill_no_trays);
    RUN_TEST(test_fill_seventeen_trays);
    RUN_TEST(test_fill_a_hundred_trays);
    RUN_TEST(test_extra_portions_on_four_trays);
    RUN_TEST(test_no_extra_portions);
    RUN_TEST(test_extra_portions_on_seventeen_trays);
    RUN_TEST(test_one_portion_taken_from_a_hundred_trays);
    RUN_TEST(test_extra_portions_with_no_trays);
    RUN_TEST(test_scale_five_ingredients_for_four_batches);
    RUN_TEST(test_scale_one_ingredient_for_one_batch);
    RUN_TEST(test_scale_three_ingredients_for_ten_batches);
    RUN_TEST(test_scale_seventeen_ingredients_for_three_batches);
    RUN_TEST(test_scale_no_ingredients);
    RUN_TEST(test_add_portions_for_four_sauces);
    RUN_TEST(test_add_portions_for_one_sauce);
    RUN_TEST(test_add_portions_for_seventeen_sauces);
    RUN_TEST(test_add_portions_for_a_hundred_sauces);
    RUN_TEST(test_add_portions_for_no_sauces);
    RUN_TEST(test_leftovers_for_five_sauces);
    RUN_TEST(test_no_leftovers);
    RUN_TEST(test_leftovers_for_seventeen_sauces);
    RUN_TEST(test_leftovers_for_a_hundred_sauces);
    RUN_TEST(test_leftovers_for_no_sauces);
    RUN_TEST(test_room_on_four_trays);
    RUN_TEST(test_no_room_on_a_full_tray);
    RUN_TEST(test_room_on_seventeen_trays);
    RUN_TEST(test_room_on_a_hundred_trays);
    RUN_TEST(test_room_on_no_trays);
    return UNITY_END();
}
