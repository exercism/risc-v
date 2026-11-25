#include "vendor/unity.h"

#include <stdint.h>

enum piece {
    PAWN = 1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK = 4,
    QUEEN = 5,
    KING = 6
};

enum result {
    RESULT_WIN = 1,
    RESULT_DRAW = 2,
    RESULT_LOSS = 3
};

extern int32_t piece_value(int32_t piece);
extern int32_t promote_pawn(int32_t material);
extern int32_t empty_squares(int32_t pieces);
extern int32_t on_board(int32_t rank, int32_t file);
extern int32_t result_points(int32_t result);
extern int32_t match_result(int32_t our_points, int32_t their_points);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_a_pawn_is_worth_1(void) {
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, piece_value(PAWN), "piece_value(PAWN)");
}

void test_a_bishop_is_worth_3(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, piece_value(BISHOP), "piece_value(BISHOP)");
}

void test_a_rook_is_worth_5(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(5, piece_value(ROOK), "piece_value(ROOK)");
}

void test_a_queen_is_worth_9(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(9, piece_value(QUEEN), "piece_value(QUEEN)");
}

void test_the_king_is_not_counted(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, piece_value(KING), "piece_value(KING)");
}

// TASK: 2
void test_promoting_with_all_pieces_on_the_board(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(47, promote_pawn(39), "promote_pawn(39)");
}

void test_promoting_with_only_a_pawn_left(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(9, promote_pawn(1), "promote_pawn(1)");
}

void test_promoting_in_the_endgame(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(15, promote_pawn(7), "promote_pawn(7)");
}

void test_promoting_a_second_queen(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(26, promote_pawn(18), "promote_pawn(18)");
}

// TASK: 3
void test_a_full_board_at_the_start(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(32, empty_squares(32), "empty_squares(32)");
}

void test_only_the_two_kings_are_left(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(62, empty_squares(2), "empty_squares(2)");
}

void test_an_empty_board(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(64, empty_squares(0), "empty_squares(0)");
}

void test_a_board_with_every_square_filled(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, empty_squares(64), "empty_squares(64)");
}

// TASK: 4
void test_the_first_corner_is_on_the_board(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, on_board(0, 0), "on_board(0, 0)");
}

void test_the_last_corner_is_on_the_board(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, on_board(7, 7), "on_board(7, 7)");
}

void test_a_rank_past_the_edge_is_off_the_board(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, on_board(8, 3), "on_board(8, 3)");
}

void test_a_negative_file_is_off_the_board(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, on_board(3, -1), "on_board(3, -1)");
}

void test_a_file_past_the_edge_is_off_the_board(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, on_board(4, 8), "on_board(4, 8)");
}

// TASK: 5
void test_a_win_scores_2_half_points(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, result_points(RESULT_WIN), "result_points(RESULT_WIN)");
}

void test_a_draw_scores_1_half_point(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, result_points(RESULT_DRAW), "result_points(RESULT_DRAW)");
}

void test_a_loss_scores_no_half_points(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, result_points(RESULT_LOSS), "result_points(RESULT_LOSS)");
}

// TASK: 6
void test_scoring_more_wins_the_match(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(RESULT_WIN, match_result(5, 3), "match_result(5, 3)");
}

void test_scoring_the_same_draws_the_match(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(RESULT_DRAW, match_result(4, 4), "match_result(4, 4)");
}

void test_scoring_less_loses_the_match(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(RESULT_LOSS, match_result(2, 6), "match_result(2, 6)");
}

void test_a_clean_sweep_wins_the_match(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(RESULT_WIN, match_result(8, 0), "match_result(8, 0)");
}

void test_losing_every_game_loses_the_match(void) {
    TEST_IGNORE();
    TEST_ASSERT_EQUAL_INT32_MESSAGE(RESULT_LOSS, match_result(0, 8), "match_result(0, 8)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_a_pawn_is_worth_1);
    RUN_TEST(test_a_bishop_is_worth_3);
    RUN_TEST(test_a_rook_is_worth_5);
    RUN_TEST(test_a_queen_is_worth_9);
    RUN_TEST(test_the_king_is_not_counted);
    RUN_TEST(test_promoting_with_all_pieces_on_the_board);
    RUN_TEST(test_promoting_with_only_a_pawn_left);
    RUN_TEST(test_promoting_in_the_endgame);
    RUN_TEST(test_promoting_a_second_queen);
    RUN_TEST(test_a_full_board_at_the_start);
    RUN_TEST(test_only_the_two_kings_are_left);
    RUN_TEST(test_an_empty_board);
    RUN_TEST(test_a_board_with_every_square_filled);
    RUN_TEST(test_the_first_corner_is_on_the_board);
    RUN_TEST(test_the_last_corner_is_on_the_board);
    RUN_TEST(test_a_rank_past_the_edge_is_off_the_board);
    RUN_TEST(test_a_negative_file_is_off_the_board);
    RUN_TEST(test_a_file_past_the_edge_is_off_the_board);
    RUN_TEST(test_a_win_scores_2_half_points);
    RUN_TEST(test_a_draw_scores_1_half_point);
    RUN_TEST(test_a_loss_scores_no_half_points);
    RUN_TEST(test_scoring_more_wins_the_match);
    RUN_TEST(test_scoring_the_same_draws_the_match);
    RUN_TEST(test_scoring_less_loses_the_match);
    RUN_TEST(test_a_clean_sweep_wins_the_match);
    RUN_TEST(test_losing_every_game_loses_the_match);
    return UNITY_END();
}
