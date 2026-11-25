#include "vendor/unity.h"

#include <stdint.h>

extern int32_t count_colour(const int32_t *colours, int32_t count, int32_t colour);
extern int32_t first_unmasked(const int32_t *colours, int32_t count);
extern void cap_donations(int32_t *donations, int32_t count, int32_t cap);
extern void discount_juniors(int32_t *prices, const int32_t *ages, int32_t count, int32_t age_limit, int32_t discount);
extern int32_t junior_donations(const int32_t *donations, const int32_t *ages, int32_t count, int32_t age_limit);
extern int32_t count_winners(const uint32_t *tickets, int32_t count, uint32_t low, uint32_t high);

// Every array in these tests has one extra element after the last one the function is given.
// It holds a sentinel that would change the answer if the function read it,
// and the tests check that the function did not change it.
#define SENTINEL 0x5EA5EA
#define NO_MASK 0
#define JUNIOR_AGE 5

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_count_gold_masks_among_five_guests(void) {
    const int32_t colours[] = {3, 1, 3, 0, 3, 3};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, count_colour(colours, 5, 3), "count_colour({3, 1, 3, 0, 3}, 5, 3)");
}

void test_count_colour_of_one_guest(void) {
    TEST_IGNORE();
    const int32_t colours[] = {2, 2};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, count_colour(colours, 1, 2), "count_colour({2}, 1, 2)");
}

void test_count_colour_of_no_guests(void) {
    TEST_IGNORE();
    const int32_t colours[] = {4};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, count_colour(colours, 0, 4), "count_colour({}, 0, 4)");
}

void test_count_colour_of_seventeen_guests(void) {
    TEST_IGNORE();
    int32_t colours[18];
    for (int32_t i = 0; i < 17; i++) {
        colours[i] = i % 4;
    }
    colours[17] = 1;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, count_colour(colours, 17, 1), "count_colour(colours, 17, 1) with colours[i] = i % 4");
}

void test_count_colour_of_a_hundred_guests(void) {
    TEST_IGNORE();
    int32_t colours[101];
    for (int32_t i = 0; i < 100; i++) {
        colours[i] = i % 6;
    }
    colours[100] = 5;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(16, count_colour(colours, 100, 5), "count_colour(colours, 100, 5) with colours[i] = i % 6");
}

// TASK: 2
void test_first_unmasked_of_five_guests(void) {
    TEST_IGNORE();
    const int32_t colours[] = {3, 1, 0, 2, 0, NO_MASK};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, first_unmasked(colours, 5), "first_unmasked({3, 1, 0, 2, 0}, 5)");
}

void test_first_unmasked_when_every_guest_has_a_mask(void) {
    TEST_IGNORE();
    const int32_t colours[] = {1, 2, 3, 4, NO_MASK};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, first_unmasked(colours, 4), "first_unmasked({1, 2, 3, 4}, 4)");
}

void test_first_unmasked_of_no_guests(void) {
    TEST_IGNORE();
    const int32_t colours[] = {NO_MASK};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, first_unmasked(colours, 0), "first_unmasked({}, 0)");
}

void test_first_unmasked_is_the_last_of_seventeen_guests(void) {
    TEST_IGNORE();
    int32_t colours[18];
    for (int32_t i = 0; i < 16; i++) {
        colours[i] = 1 + i % 3;
    }
    colours[16] = NO_MASK;
    colours[17] = NO_MASK;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(16, first_unmasked(colours, 17),
                                    "first_unmasked(colours, 17) with colours[i] = 1 + i % 3, but colours[16] = 0");
}

void test_first_unmasked_of_a_hundred_guests(void) {
    TEST_IGNORE();
    int32_t colours[101];
    for (int32_t i = 0; i < 100; i++) {
        colours[i] = 2;
    }
    colours[73] = NO_MASK;
    colours[90] = NO_MASK;
    colours[100] = NO_MASK;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(73, first_unmasked(colours, 100),
                                    "first_unmasked(colours, 100) with colours[i] = 2, but colours[73] = colours[90] = 0");
}

// TASK: 3
void test_cap_five_donations(void) {
    TEST_IGNORE();
    int32_t donations[] = {20, 150, 50, 300, 100, SENTINEL};
    const int32_t expected[] = {20, 100, 50, 100, 100, SENTINEL};
    cap_donations(donations, 5, 100);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, donations, 6,
                                          "cap_donations(donations, 5, 100) with donations = {20, 150, 50, 300, 100}");
}

void test_cap_three_large_donations(void) {
    TEST_IGNORE();
    int32_t donations[] = {500, 501, 2000, SENTINEL};
    const int32_t expected[] = {200, 200, 200, SENTINEL};
    cap_donations(donations, 3, 200);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, donations, 4, "cap_donations(donations, 3, 200) with donations = {500, 501, 2000}");
}

void test_cap_seventeen_donations(void) {
    TEST_IGNORE();
    int32_t donations[18];
    int32_t expected[18];
    for (int32_t i = 0; i < 17; i++) {
        donations[i] = 10 * i;
        expected[i] = 10 * i > 85 ? 85 : 10 * i;
    }
    donations[17] = SENTINEL;
    expected[17] = SENTINEL;
    cap_donations(donations, 17, 85);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, donations, 18, "cap_donations(donations, 17, 85) with donations[i] = 10 * i");
}

void test_cap_a_hundred_donations(void) {
    TEST_IGNORE();
    int32_t donations[101];
    int32_t expected[101];
    for (int32_t i = 0; i < 100; i++) {
        donations[i] = (i * 37) % 100 * 5;
        expected[i] = donations[i] > 250 ? 250 : donations[i];
    }
    donations[100] = SENTINEL;
    expected[100] = SENTINEL;
    cap_donations(donations, 100, 250);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, donations, 101,
                                          "cap_donations(donations, 100, 250) with donations[i] = (i * 37) % 100 * 5");
}

void test_cap_no_donations(void) {
    TEST_IGNORE();
    int32_t donations[] = {SENTINEL};
    const int32_t expected[] = {SENTINEL};
    cap_donations(donations, 0, 100);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, donations, 1, "cap_donations(donations, 0, 100)");
}

// TASK: 4
void test_discount_juniors_among_four_guests(void) {
    TEST_IGNORE();
    int32_t prices[] = {30, 30, 30, 30, SENTINEL};
    const int32_t ages[] = {10, 16, 12, 45, JUNIOR_AGE};
    const int32_t expected[] = {20, 30, 20, 30, SENTINEL};
    discount_juniors(prices, ages, 4, 13, 10);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, prices, 5,
                                          "discount_juniors(prices, {10, 16, 12, 45}, 4, 13, 10) with prices = {30, 30, 30, 30}");
}

void test_no_discount_at_the_age_limit(void) {
    TEST_IGNORE();
    int32_t prices[] = {25, SENTINEL};
    const int32_t ages[] = {13, JUNIOR_AGE};
    const int32_t expected[] = {25, SENTINEL};
    discount_juniors(prices, ages, 1, 13, 10);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, prices, 2, "discount_juniors(prices, {13}, 1, 13, 10) with prices = {25}");
}

void test_discount_juniors_among_no_guests(void) {
    TEST_IGNORE();
    int32_t prices[] = {SENTINEL};
    const int32_t ages[] = {JUNIOR_AGE};
    const int32_t expected[] = {SENTINEL};
    discount_juniors(prices, ages, 0, 13, 10);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, prices, 1, "discount_juniors(prices, {}, 0, 13, 10)");
}

void test_discount_juniors_among_seventeen_guests(void) {
    TEST_IGNORE();
    int32_t prices[18];
    int32_t ages[18];
    int32_t expected[18];
    for (int32_t i = 0; i < 17; i++) {
        prices[i] = 30 + i;
        ages[i] = 5 + i;
        expected[i] = i < 8 ? 25 + i : 30 + i;
    }
    prices[17] = SENTINEL;
    ages[17] = JUNIOR_AGE;
    expected[17] = SENTINEL;
    discount_juniors(prices, ages, 17, 13, 5);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(expected, prices, 18,
                                          "discount_juniors(prices, ages, 17, 13, 5) with prices[i] = 30 + i, ages[i] = 5 + i");
}

void test_discount_juniors_among_a_hundred_guests(void) {
    TEST_IGNORE();
    int32_t prices[101];
    int32_t ages[101];
    int32_t expected[101];
    for (int32_t i = 0; i < 100; i++) {
        prices[i] = 20 + i % 3 * 5;
        ages[i] = 6 + i % 50;
        expected[i] = ages[i] < 18 ? prices[i] - 15 : prices[i];
    }
    prices[100] = SENTINEL;
    ages[100] = JUNIOR_AGE;
    expected[100] = SENTINEL;
    discount_juniors(prices, ages, 100, 18, 15);
    TEST_ASSERT_EQUAL_INT32_ARRAY_MESSAGE(
        expected, prices, 101, "discount_juniors(prices, ages, 100, 18, 15) with prices[i] = 20 + i % 3 * 5, ages[i] = 6 + i % 50");
}

// TASK: 5
void test_junior_donations_among_five_guests(void) {
    TEST_IGNORE();
    const int32_t donations[] = {10, 20, 30, 40, 50, SENTINEL};
    const int32_t ages[] = {12, 40, 8, 17, 11, JUNIOR_AGE};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(90, junior_donations(donations, ages, 5, 13),
                                    "junior_donations({10, 20, 30, 40, 50}, {12, 40, 8, 17, 11}, 5, 13)");
}

void test_junior_donations_when_there_are_no_juniors(void) {
    TEST_IGNORE();
    const int32_t donations[] = {10, 20, 30, SENTINEL};
    const int32_t ages[] = {30, 40, 13, JUNIOR_AGE};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, junior_donations(donations, ages, 3, 13), "junior_donations({10, 20, 30}, {30, 40, 13}, 3, 13)");
}

void test_junior_donations_among_no_guests(void) {
    TEST_IGNORE();
    const int32_t donations[] = {SENTINEL};
    const int32_t ages[] = {JUNIOR_AGE};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, junior_donations(donations, ages, 0, 13), "junior_donations({}, {}, 0, 13)");
}

void test_junior_donations_among_seventeen_guests(void) {
    TEST_IGNORE();
    int32_t donations[18];
    int32_t ages[18];
    for (int32_t i = 0; i < 17; i++) {
        donations[i] = i + 1;
        ages[i] = 10 + i;
    }
    donations[17] = SENTINEL;
    ages[17] = JUNIOR_AGE;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(36, junior_donations(donations, ages, 17, 18),
                                    "junior_donations(donations, ages, 17, 18) with donations[i] = i + 1, ages[i] = 10 + i");
}

void test_junior_donations_among_a_hundred_guests(void) {
    TEST_IGNORE();
    int32_t donations[101];
    int32_t ages[101];
    for (int32_t i = 0; i < 100; i++) {
        donations[i] = 5 * (i % 4 + 1);
        ages[i] = 5 + i % 20;
    }
    donations[100] = SENTINEL;
    ages[100] = JUNIOR_AGE;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(400, junior_donations(donations, ages, 100, 12),
                                    "junior_donations(donations, ages, 100, 12) with donations[i] = 5 * (i % 4 + 1), ages[i] = 5 + i % 20");
}

// TASK: 6
void test_count_winners_among_five_tickets(void) {
    TEST_IGNORE();
    const uint32_t tickets[] = {2500000000u, 1000u, 2147483647u, 3000000001u, 2000000000u, 2500000001u};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, count_winners(tickets, 5, 2000000000u, 3000000000u),
                                    "count_winners({2500000000, 1000, 2147483647, 3000000001, 2000000000}, 5, 2000000000, 3000000000)");
}

void test_count_winners_at_the_top_of_the_range(void) {
    TEST_IGNORE();
    const uint32_t tickets[] = {4294967295u, 4294967294u, 7u, 4294967290u};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, count_winners(tickets, 3, 4294967294u, 4294967295u),
                                    "count_winners({4294967295, 4294967294, 7}, 3, 4294967294, 4294967295)");
}

void test_count_winners_among_no_tickets(void) {
    TEST_IGNORE();
    const uint32_t tickets[] = {50u};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, count_winners(tickets, 0, 1u, 100u), "count_winners({}, 0, 1, 100)");
}

void test_count_winners_among_seventeen_tickets(void) {
    TEST_IGNORE();
    uint32_t tickets[18];
    for (uint32_t i = 0; i < 17; i++) {
        tickets[i] = 2147483640u + i;
    }
    tickets[17] = 2147483648u;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(6, count_winners(tickets, 17, 2147483645u, 2147483650u),
                                    "count_winners(tickets, 17, 2147483645, 2147483650) with tickets[i] = 2147483640 + i");
}

void test_count_winners_among_a_hundred_tickets(void) {
    TEST_IGNORE();
    uint32_t tickets[101];
    for (uint32_t i = 0; i < 100; i++) {
        tickets[i] = i * 43000000u;
    }
    tickets[100] = 2000000000u;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(58, count_winners(tickets, 100, 1000000000u, 3500000000u),
                                    "count_winners(tickets, 100, 1000000000, 3500000000) with tickets[i] = i * 43000000");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_count_gold_masks_among_five_guests);
    RUN_TEST(test_count_colour_of_one_guest);
    RUN_TEST(test_count_colour_of_no_guests);
    RUN_TEST(test_count_colour_of_seventeen_guests);
    RUN_TEST(test_count_colour_of_a_hundred_guests);
    RUN_TEST(test_first_unmasked_of_five_guests);
    RUN_TEST(test_first_unmasked_when_every_guest_has_a_mask);
    RUN_TEST(test_first_unmasked_of_no_guests);
    RUN_TEST(test_first_unmasked_is_the_last_of_seventeen_guests);
    RUN_TEST(test_first_unmasked_of_a_hundred_guests);
    RUN_TEST(test_cap_five_donations);
    RUN_TEST(test_cap_three_large_donations);
    RUN_TEST(test_cap_seventeen_donations);
    RUN_TEST(test_cap_a_hundred_donations);
    RUN_TEST(test_cap_no_donations);
    RUN_TEST(test_discount_juniors_among_four_guests);
    RUN_TEST(test_no_discount_at_the_age_limit);
    RUN_TEST(test_discount_juniors_among_no_guests);
    RUN_TEST(test_discount_juniors_among_seventeen_guests);
    RUN_TEST(test_discount_juniors_among_a_hundred_guests);
    RUN_TEST(test_junior_donations_among_five_guests);
    RUN_TEST(test_junior_donations_when_there_are_no_juniors);
    RUN_TEST(test_junior_donations_among_no_guests);
    RUN_TEST(test_junior_donations_among_seventeen_guests);
    RUN_TEST(test_junior_donations_among_a_hundred_guests);
    RUN_TEST(test_count_winners_among_five_tickets);
    RUN_TEST(test_count_winners_at_the_top_of_the_range);
    RUN_TEST(test_count_winners_among_no_tickets);
    RUN_TEST(test_count_winners_among_seventeen_tickets);
    RUN_TEST(test_count_winners_among_a_hundred_tickets);
    return UNITY_END();
}
