#include "vendor/unity.h"

#include <stdint.h>

extern int32_t total_time(const int32_t *laps, int32_t count);
extern int32_t fastest_lap(const int32_t *laps, int32_t count);
extern int32_t best_improvement(const int32_t *changes, int32_t count);
extern uint32_t longest_paddle(const uint32_t *millimetres, int32_t count);
extern int32_t team_total(const int32_t *first, const int32_t *second, int32_t count);
extern int32_t lap_spread(const int32_t *laps, int32_t count);

// Every array in these tests has one extra element after the last one the function is given.
// It holds a sentinel that would change the answer if the function read it.
#define HIGH_SENTINEL 0x5EA5EA
#define LOW_SENTINEL (-0x5EA5EA)
#define UNSIGNED_SENTINEL 0xFFFFFFFFu

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_total_time_of_four_laps(void) {
    const int32_t laps[] = {600, 612, 598, 605, HIGH_SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2415, total_time(laps, 4), "total_time({600, 612, 598, 605}, 4)");
}

void test_total_time_of_one_lap(void) {
    TEST_IGNORE();
    const int32_t laps[] = {587, HIGH_SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(587, total_time(laps, 1), "total_time({587}, 1)");
}

void test_total_time_of_no_laps(void) {
    TEST_IGNORE();
    const int32_t laps[] = {HIGH_SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, total_time(laps, 0), "total_time({}, 0)");
}

void test_total_time_of_seventeen_laps(void) {
    TEST_IGNORE();
    int32_t laps[18];
    for (int32_t i = 0; i < 17; i++) {
        laps[i] = 600 + i;
    }
    laps[17] = HIGH_SENTINEL;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(10336, total_time(laps, 17), "total_time(laps, 17) with laps[i] = 600 + i");
}

void test_total_time_of_a_hundred_laps(void) {
    TEST_IGNORE();
    int32_t laps[101];
    for (int32_t i = 0; i < 100; i++) {
        laps[i] = 500 + i % 10;
    }
    laps[100] = HIGH_SENTINEL;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(50450, total_time(laps, 100), "total_time(laps, 100) with laps[i] = 500 + i % 10");
}

// TASK: 2
void test_fastest_of_five_laps(void) {
    TEST_IGNORE();
    const int32_t laps[] = {612, 598, 605, 600, 601, LOW_SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(598, fastest_lap(laps, 5), "fastest_lap({612, 598, 605, 600, 601}, 5)");
}

void test_fastest_of_one_lap(void) {
    TEST_IGNORE();
    const int32_t laps[] = {640, LOW_SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(640, fastest_lap(laps, 1), "fastest_lap({640}, 1)");
}

void test_fastest_lap_is_the_last_of_three(void) {
    TEST_IGNORE();
    const int32_t laps[] = {620, 615, 590, LOW_SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(590, fastest_lap(laps, 3), "fastest_lap({620, 615, 590}, 3)");
}

void test_fastest_of_seventeen_laps(void) {
    TEST_IGNORE();
    int32_t laps[18];
    for (int32_t i = 0; i < 17; i++) {
        laps[i] = 700 - i;
    }
    laps[17] = LOW_SENTINEL;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(684, fastest_lap(laps, 17), "fastest_lap(laps, 17) with laps[i] = 700 - i");
}

void test_fastest_of_a_hundred_laps(void) {
    TEST_IGNORE();
    int32_t laps[101];
    for (int32_t i = 0; i < 100; i++) {
        laps[i] = 500 + (i + 50) * 37 % 100;
    }
    laps[100] = LOW_SENTINEL;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(500, fastest_lap(laps, 100), "fastest_lap(laps, 100) with laps[i] = 500 + (i + 50) * 37 % 100");
}

// TASK: 3
void test_best_of_four_improvements(void) {
    TEST_IGNORE();
    const int32_t changes[] = {-12, 30, 5, -4, HIGH_SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(30, best_improvement(changes, 4), "best_improvement({-12, 30, 5, -4}, 4)");
}

void test_best_improvement_when_every_crew_was_slower(void) {
    TEST_IGNORE();
    const int32_t changes[] = {-15, -3, -8, HIGH_SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-3, best_improvement(changes, 3), "best_improvement({-15, -3, -8}, 3)");
}

void test_best_improvement_of_one_crew(void) {
    TEST_IGNORE();
    const int32_t changes[] = {-7, HIGH_SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-7, best_improvement(changes, 1), "best_improvement({-7}, 1)");
}

void test_best_of_seventeen_improvements(void) {
    TEST_IGNORE();
    int32_t changes[18];
    for (int32_t i = 0; i < 17; i++) {
        changes[i] = i - 20;
    }
    changes[17] = HIGH_SENTINEL;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-4, best_improvement(changes, 17), "best_improvement(changes, 17) with changes[i] = i - 20");
}

void test_best_of_a_hundred_improvements(void) {
    TEST_IGNORE();
    int32_t changes[101];
    for (int32_t i = 0; i < 100; i++) {
        changes[i] = 25 - (i - 60) * (i - 60);
    }
    changes[100] = HIGH_SENTINEL;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(25, best_improvement(changes, 100),
                                    "best_improvement(changes, 100) with changes[i] = 25 - (i - 60) * (i - 60)");
}

// TASK: 4
void test_longest_of_four_paddles(void) {
    TEST_IGNORE();
    const uint32_t millimetres[] = {1200000000u, 3000000000u, 2500000000u, 900000000u, UNSIGNED_SENTINEL};
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(3000000000u, longest_paddle(millimetres, 4),
                                     "longest_paddle({1200000000, 3000000000, 2500000000, 900000000}, 4)");
}

void test_longest_of_one_paddle(void) {
    TEST_IGNORE();
    const uint32_t millimetres[] = {2147483648u, UNSIGNED_SENTINEL};
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(2147483648u, longest_paddle(millimetres, 1), "longest_paddle({2147483648}, 1)");
}

void test_longest_of_no_paddles(void) {
    TEST_IGNORE();
    const uint32_t millimetres[] = {UNSIGNED_SENTINEL};
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(0, longest_paddle(millimetres, 0), "longest_paddle({}, 0)");
}

void test_longest_of_seventeen_paddles(void) {
    TEST_IGNORE();
    uint32_t millimetres[18];
    for (uint32_t i = 0; i < 17; i++) {
        millimetres[i] = 2000000000u + i * 100000000u;
    }
    millimetres[17] = UNSIGNED_SENTINEL;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(3600000000u, longest_paddle(millimetres, 17),
                                     "longest_paddle(millimetres, 17) with millimetres[i] = 2000000000 + i * 100000000");
}

void test_longest_of_a_hundred_paddles(void) {
    TEST_IGNORE();
    uint32_t millimetres[101];
    for (uint32_t i = 0; i < 100; i++) {
        millimetres[i] = (i * 37 % 100) * 40000000u;
    }
    millimetres[100] = UNSIGNED_SENTINEL;
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(3960000000u, longest_paddle(millimetres, 100),
                                     "longest_paddle(millimetres, 100) with millimetres[i] = (i * 37 % 100) * 40000000");
}

// TASK: 5
void test_team_total_of_four_laps(void) {
    TEST_IGNORE();
    const int32_t first[] = {600, 610, 605, 598, HIGH_SENTINEL};
    const int32_t second[] = {620, 615, 600, 611, HIGH_SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4859, team_total(first, second, 4), "team_total({600, 610, 605, 598}, {620, 615, 600, 611}, 4)");
}

void test_team_total_of_one_lap(void) {
    TEST_IGNORE();
    const int32_t first[] = {633, HIGH_SENTINEL};
    const int32_t second[] = {641, HIGH_SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1274, team_total(first, second, 1), "team_total({633}, {641}, 1)");
}

void test_team_total_of_no_laps(void) {
    TEST_IGNORE();
    const int32_t first[] = {HIGH_SENTINEL};
    const int32_t second[] = {HIGH_SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, team_total(first, second, 0), "team_total({}, {}, 0)");
}

void test_team_total_of_seventeen_laps(void) {
    TEST_IGNORE();
    int32_t first[18];
    int32_t second[18];
    for (int32_t i = 0; i < 17; i++) {
        first[i] = 600 + i;
        second[i] = 650 - 2 * i;
    }
    first[17] = HIGH_SENTINEL;
    second[17] = HIGH_SENTINEL;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(21114, team_total(first, second, 17),
                                    "team_total(first, second, 17) with first[i] = 600 + i, second[i] = 650 - 2 * i");
}

void test_team_total_of_a_hundred_laps(void) {
    TEST_IGNORE();
    int32_t first[101];
    int32_t second[101];
    for (int32_t i = 0; i < 100; i++) {
        first[i] = 500 + i % 10;
        second[i] = 520 - i % 5;
    }
    first[100] = HIGH_SENTINEL;
    second[100] = HIGH_SENTINEL;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(102250, team_total(first, second, 100),
                                    "team_total(first, second, 100) with first[i] = 500 + i % 10, second[i] = 520 - i % 5");
}

// TASK: 6
void test_spread_of_five_laps(void) {
    TEST_IGNORE();
    const int32_t laps[] = {612, 598, 605, 640, 601, LOW_SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(42, lap_spread(laps, 5), "lap_spread({612, 598, 605, 640, 601}, 5)");
}

void test_spread_of_one_lap(void) {
    TEST_IGNORE();
    const int32_t laps[] = {615, LOW_SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, lap_spread(laps, 1), "lap_spread({615}, 1)");
}

void test_spread_of_three_equal_laps(void) {
    TEST_IGNORE();
    const int32_t laps[] = {600, 600, 600, HIGH_SENTINEL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, lap_spread(laps, 3), "lap_spread({600, 600, 600}, 3)");
}

void test_spread_of_seventeen_laps(void) {
    TEST_IGNORE();
    int32_t laps[18];
    for (int32_t i = 0; i < 17; i++) {
        laps[i] = 600 + (i * 5) % 17;
    }
    laps[17] = HIGH_SENTINEL;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(16, lap_spread(laps, 17), "lap_spread(laps, 17) with laps[i] = 600 + (i * 5) % 17");
}

void test_spread_of_a_hundred_laps(void) {
    TEST_IGNORE();
    int32_t laps[101];
    for (int32_t i = 0; i < 100; i++) {
        laps[i] = 550 + (i + 50) * 37 % 100;
    }
    laps[100] = LOW_SENTINEL;
    TEST_ASSERT_EQUAL_INT32_MESSAGE(99, lap_spread(laps, 100), "lap_spread(laps, 100) with laps[i] = 550 + (i + 50) * 37 % 100");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_total_time_of_four_laps);
    RUN_TEST(test_total_time_of_one_lap);
    RUN_TEST(test_total_time_of_no_laps);
    RUN_TEST(test_total_time_of_seventeen_laps);
    RUN_TEST(test_total_time_of_a_hundred_laps);
    RUN_TEST(test_fastest_of_five_laps);
    RUN_TEST(test_fastest_of_one_lap);
    RUN_TEST(test_fastest_lap_is_the_last_of_three);
    RUN_TEST(test_fastest_of_seventeen_laps);
    RUN_TEST(test_fastest_of_a_hundred_laps);
    RUN_TEST(test_best_of_four_improvements);
    RUN_TEST(test_best_improvement_when_every_crew_was_slower);
    RUN_TEST(test_best_improvement_of_one_crew);
    RUN_TEST(test_best_of_seventeen_improvements);
    RUN_TEST(test_best_of_a_hundred_improvements);
    RUN_TEST(test_longest_of_four_paddles);
    RUN_TEST(test_longest_of_one_paddle);
    RUN_TEST(test_longest_of_no_paddles);
    RUN_TEST(test_longest_of_seventeen_paddles);
    RUN_TEST(test_longest_of_a_hundred_paddles);
    RUN_TEST(test_team_total_of_four_laps);
    RUN_TEST(test_team_total_of_one_lap);
    RUN_TEST(test_team_total_of_no_laps);
    RUN_TEST(test_team_total_of_seventeen_laps);
    RUN_TEST(test_team_total_of_a_hundred_laps);
    RUN_TEST(test_spread_of_five_laps);
    RUN_TEST(test_spread_of_one_lap);
    RUN_TEST(test_spread_of_three_equal_laps);
    RUN_TEST(test_spread_of_seventeen_laps);
    RUN_TEST(test_spread_of_a_hundred_laps);
    return UNITY_END();
}
