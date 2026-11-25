#include "vendor/unity.h"

#include <stddef.h>
#include <stdint.h>

extern int32_t record_count(const char *const *records);
extern int32_t longest_record(const char *const *records, int32_t count);
extern int32_t find_name(const char *const *names, int32_t count, const char *name);
extern int32_t count_starting_with(const char *const *records, int32_t count, char letter);
extern char grid_char(const char *const *rows, int32_t row, int32_t col);
extern int32_t collect_short(const char **out, const char *const *records, int32_t count, int32_t max_len);

static const char unused[] = "unused";

static void fill_unused(const char **out, int32_t count) {
    for (int32_t i = 0; i < count; i++) {
        out[i] = unused;
    }
}

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_count_three_records(void) {
    const char *records[] = {"Missing canoe at Point Cruz", "Goat in the church hall", "Lights over Savo", NULL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(
        3, record_count(records),
        "record_count({\"Missing canoe at Point Cruz\", \"Goat in the church hall\", \"Lights over Savo\", NULL})");
}

void test_count_a_single_record(void) {
    TEST_IGNORE();
    const char *records[] = {"Lost key", NULL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, record_count(records), "record_count({\"Lost key\", NULL})");
}

void test_count_an_empty_box(void) {
    TEST_IGNORE();
    const char *records[] = {NULL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, record_count(records), "record_count({NULL})");
}

void test_an_empty_record_still_counts(void) {
    TEST_IGNORE();
    const char *records[] = {"Lost key", "", "Odd footprints", NULL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, record_count(records), "record_count({\"Lost key\", \"\", \"Odd footprints\", NULL})");
}

void test_count_six_records(void) {
    TEST_IGNORE();
    const char *records[] = {"A", "B", "C", "D", "E", "F", NULL};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(6, record_count(records), "record_count({\"A\", \"B\", \"C\", \"D\", \"E\", \"F\", NULL})");
}

// TASK: 2
void test_longest_record_in_the_middle(void) {
    TEST_IGNORE();
    const char *records[] = {"Lost key", "Missing canoe at Point Cruz", "Goat in the church hall"};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, longest_record(records, 3),
                                    "longest_record({\"Lost key\", \"Missing canoe at Point Cruz\", \"Goat in the church hall\"}, 3)");
}

void test_longest_record_at_the_end(void) {
    TEST_IGNORE();
    const char *records[] = {"Fish", "Lost key", "Who took the mayor's umbrella?"};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, longest_record(records, 3),
                                    "longest_record({\"Fish\", \"Lost key\", \"Who took the mayor's umbrella?\"}, 3)");
}

void test_longest_record_at_the_start(void) {
    TEST_IGNORE();
    const char *records[] = {"Market clock stopped at noon", "Lost key", "Fish", "Odd note"};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, longest_record(records, 4),
                                    "longest_record({\"Market clock stopped at noon\", \"Lost key\", \"Fish\", \"Odd note\"}, 4)");
}

void test_a_tie_goes_to_the_earlier_record(void) {
    TEST_IGNORE();
    const char *records[] = {"Fish", "Lost key", "Odd note", "Red boat"};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, longest_record(records, 4),
                                    "longest_record({\"Fish\", \"Lost key\", \"Odd note\", \"Red boat\"}, 4)");
}

void test_longest_record_among_empty_ones(void) {
    TEST_IGNORE();
    const char *records[] = {"", "?", ""};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, longest_record(records, 3), "longest_record({\"\", \"?\", \"\"}, 3)");
}

// TASK: 3
void test_find_a_name_in_the_middle(void) {
    TEST_IGNORE();
    const char *names[] = {"Ruth Kiko", "John Maeli", "Grace Tovua", "Peter Ramo"};
    char name[] = "Grace Tovua";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, find_name(names, 4, name),
                                    "find_name({\"Ruth Kiko\", \"John Maeli\", \"Grace Tovua\", \"Peter Ramo\"}, 4, \"Grace Tovua\")");
}

void test_find_the_last_name(void) {
    TEST_IGNORE();
    const char *names[] = {"Ruth Kiko", "John Maeli", "Grace Tovua", "Peter Ramo"};
    char name[] = "Peter Ramo";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, find_name(names, 4, name),
                                    "find_name({\"Ruth Kiko\", \"John Maeli\", \"Grace Tovua\", \"Peter Ramo\"}, 4, \"Peter Ramo\")");
}

void test_a_shorter_name_is_not_a_match(void) {
    TEST_IGNORE();
    const char *names[] = {"Ruth Kiko", "John Maeli", "Grace Tovua", "Peter Ramo"};
    char name[] = "Grace";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, find_name(names, 4, name),
                                    "find_name({\"Ruth Kiko\", \"John Maeli\", \"Grace Tovua\", \"Peter Ramo\"}, 4, \"Grace\")");
}

void test_a_longer_name_is_not_a_match(void) {
    TEST_IGNORE();
    const char *names[] = {"Ruth Kiko", "John Maeli", "Grace Tovua", "Peter Ramo"};
    char name[] = "John Maelisa";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, find_name(names, 4, name),
                                    "find_name({\"Ruth Kiko\", \"John Maeli\", \"Grace Tovua\", \"Peter Ramo\"}, 4, \"John Maelisa\")");
}

void test_no_names_to_search(void) {
    TEST_IGNORE();
    const char *names[] = {"Ruth Kiko"};
    char name[] = "Ruth Kiko";
    TEST_ASSERT_EQUAL_INT32_MESSAGE(-1, find_name(names, 0, name), "find_name({}, 0, \"Ruth Kiko\")");
}

// TASK: 4
void test_count_records_starting_with_m(void) {
    TEST_IGNORE();
    const char *records[] = {"Missing canoe at Point Cruz",  "Goat in the church hall",
                             "Market clock stopped at noon", "Lights over Savo",
                             "Mystery of the red hat",       ""};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, count_starting_with(records, 6, 'M'),
                                    "count_starting_with({\"Missing canoe at Point Cruz\", \"Goat in the church hall\", \"Market clock "
                                    "stopped at noon\", \"Lights over Savo\", \"Mystery of the red hat\", \"\"}, 6, 'M')");
}

void test_count_records_starting_with_g(void) {
    TEST_IGNORE();
    const char *records[] = {"Missing canoe at Point Cruz",  "Goat in the church hall",
                             "Market clock stopped at noon", "Lights over Savo",
                             "Mystery of the red hat",       ""};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, count_starting_with(records, 6, 'G'),
                                    "count_starting_with({\"Missing canoe at Point Cruz\", \"Goat in the church hall\", \"Market clock "
                                    "stopped at noon\", \"Lights over Savo\", \"Mystery of the red hat\", \"\"}, 6, 'G')");
}

void test_no_records_start_with_z(void) {
    TEST_IGNORE();
    const char *records[] = {"Missing canoe at Point Cruz",  "Goat in the church hall",
                             "Market clock stopped at noon", "Lights over Savo",
                             "Mystery of the red hat",       ""};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, count_starting_with(records, 6, 'Z'),
                                    "count_starting_with({\"Missing canoe at Point Cruz\", \"Goat in the church hall\", \"Market clock "
                                    "stopped at noon\", \"Lights over Savo\", \"Mystery of the red hat\", \"\"}, 6, 'Z')");
}

void test_case_matters(void) {
    TEST_IGNORE();
    const char *records[] = {"Missing canoe at Point Cruz",  "Goat in the church hall",
                             "Market clock stopped at noon", "Lights over Savo",
                             "Mystery of the red hat",       ""};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, count_starting_with(records, 6, 'm'),
                                    "count_starting_with({\"Missing canoe at Point Cruz\", \"Goat in the church hall\", \"Market clock "
                                    "stopped at noon\", \"Lights over Savo\", \"Mystery of the red hat\", \"\"}, 6, 'm')");
}

void test_only_the_first_count_records(void) {
    TEST_IGNORE();
    const char *records[] = {"Missing canoe at Point Cruz",  "Goat in the church hall",
                             "Market clock stopped at noon", "Lights over Savo",
                             "Mystery of the red hat",       ""};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, count_starting_with(records, 2, 'M'),
                                    "count_starting_with({\"Missing canoe at Point Cruz\", \"Goat in the church hall\", \"Market clock "
                                    "stopped at noon\", \"Lights over Savo\", \"Mystery of the red hat\", \"\"}, 2, 'M')");
}

// TASK: 5
void test_the_top_left_corner(void) {
    TEST_IGNORE();
    const char *rows[] = {"#########", "#..X....#", "#.......#", "####.####"};
    TEST_ASSERT_EQUAL_INT32_MESSAGE('#', grid_char(rows, 0, 0),
                                    "grid_char({\"#########\", \"#..X....#\", \"#.......#\", \"####.####\"}, 0, 0)");
}

void test_the_x_on_the_map(void) {
    TEST_IGNORE();
    const char *rows[] = {"#########", "#..X....#", "#.......#", "####.####"};
    TEST_ASSERT_EQUAL_INT32_MESSAGE('X', grid_char(rows, 1, 3),
                                    "grid_char({\"#########\", \"#..X....#\", \"#.......#\", \"####.####\"}, 1, 3)");
}

void test_the_doorway(void) {
    TEST_IGNORE();
    const char *rows[] = {"#########", "#..X....#", "#.......#", "####.####"};
    TEST_ASSERT_EQUAL_INT32_MESSAGE('.', grid_char(rows, 3, 4),
                                    "grid_char({\"#########\", \"#..X....#\", \"#.......#\", \"####.####\"}, 3, 4)");
}

void test_the_right_wall(void) {
    TEST_IGNORE();
    const char *rows[] = {"#########", "#..X....#", "#.......#", "####.####"};
    TEST_ASSERT_EQUAL_INT32_MESSAGE('#', grid_char(rows, 2, 8),
                                    "grid_char({\"#########\", \"#..X....#\", \"#.......#\", \"####.####\"}, 2, 8)");
}

void test_a_floor_square(void) {
    TEST_IGNORE();
    const char *rows[] = {"#########", "#..X....#", "#.......#", "####.####"};
    TEST_ASSERT_EQUAL_INT32_MESSAGE('.', grid_char(rows, 2, 1),
                                    "grid_char({\"#########\", \"#..X....#\", \"#.......#\", \"####.####\"}, 2, 1)");
}

// TASK: 6
void test_collect_records_of_up_to_8_characters(void) {
    TEST_IGNORE();
    const char *records[] = {"Lost key", "Missing canoe at Point Cruz", "Odd note", "Goat in the church hall", "Fish"};
    const char *out[6];
    fill_unused(out, 6);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(
        3, collect_short(out, records, 5, 8),
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 8)");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(
        records[0], out[0],
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 8)");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(
        records[2], out[1],
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 8)");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(
        records[4], out[2],
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 8)");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(
        unused, out[3],
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 8)");
}

void test_collect_records_of_up_to_4_characters(void) {
    TEST_IGNORE();
    const char *records[] = {"Lost key", "Missing canoe at Point Cruz", "Odd note", "Goat in the church hall", "Fish"};
    const char *out[6];
    fill_unused(out, 6);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(
        1, collect_short(out, records, 5, 4),
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 4)");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(
        records[4], out[0],
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 4)");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(
        unused, out[1],
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 4)");
}

void test_no_record_is_short_enough(void) {
    TEST_IGNORE();
    const char *records[] = {"Lost key", "Missing canoe at Point Cruz", "Odd note", "Goat in the church hall", "Fish"};
    const char *out[6];
    fill_unused(out, 6);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(
        0, collect_short(out, records, 5, 3),
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 3)");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(
        unused, out[0],
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 3)");
}

void test_every_record_is_short_enough(void) {
    TEST_IGNORE();
    const char *records[] = {"Lost key", "Missing canoe at Point Cruz", "Odd note", "Goat in the church hall", "Fish"};
    const char *out[6];
    fill_unused(out, 6);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(
        5, collect_short(out, records, 5, 100),
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 100)");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(
        records[0], out[0],
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 100)");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(
        records[1], out[1],
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 100)");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(
        records[2], out[2],
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 100)");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(
        records[3], out[3],
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 100)");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(
        records[4], out[4],
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 100)");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(
        unused, out[5],
        "collect_short(out, {\"Lost key\", \"Missing canoe at Point Cruz\", \"Odd note\", \"Goat in the church hall\", \"Fish\"}, 5, 100)");
}

void test_collect_from_an_empty_box(void) {
    TEST_IGNORE();
    const char *records[] = {"Fish"};
    const char *out[1];
    fill_unused(out, 1);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, collect_short(out, records, 0, 10), "collect_short(out, {}, 0, 10)");
    TEST_ASSERT_EQUAL_PTR_MESSAGE(unused, out[0], "collect_short(out, {}, 0, 10)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_count_three_records);
    RUN_TEST(test_count_a_single_record);
    RUN_TEST(test_count_an_empty_box);
    RUN_TEST(test_an_empty_record_still_counts);
    RUN_TEST(test_count_six_records);
    RUN_TEST(test_longest_record_in_the_middle);
    RUN_TEST(test_longest_record_at_the_end);
    RUN_TEST(test_longest_record_at_the_start);
    RUN_TEST(test_a_tie_goes_to_the_earlier_record);
    RUN_TEST(test_longest_record_among_empty_ones);
    RUN_TEST(test_find_a_name_in_the_middle);
    RUN_TEST(test_find_the_last_name);
    RUN_TEST(test_a_shorter_name_is_not_a_match);
    RUN_TEST(test_a_longer_name_is_not_a_match);
    RUN_TEST(test_no_names_to_search);
    RUN_TEST(test_count_records_starting_with_m);
    RUN_TEST(test_count_records_starting_with_g);
    RUN_TEST(test_no_records_start_with_z);
    RUN_TEST(test_case_matters);
    RUN_TEST(test_only_the_first_count_records);
    RUN_TEST(test_the_top_left_corner);
    RUN_TEST(test_the_x_on_the_map);
    RUN_TEST(test_the_doorway);
    RUN_TEST(test_the_right_wall);
    RUN_TEST(test_a_floor_square);
    RUN_TEST(test_collect_records_of_up_to_8_characters);
    RUN_TEST(test_collect_records_of_up_to_4_characters);
    RUN_TEST(test_no_record_is_short_enough);
    RUN_TEST(test_every_record_is_short_enough);
    RUN_TEST(test_collect_from_an_empty_box);
    return UNITY_END();
}
