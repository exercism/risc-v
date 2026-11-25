.equ PAWN, 1
.equ KNIGHT, 2
.equ BISHOP, 3
.equ ROOK, 4
.equ QUEEN, 5
.equ KING, 6

.equ PAWN_VALUE, 1
.equ KNIGHT_VALUE, 3
.equ BISHOP_VALUE, 3
.equ ROOK_VALUE, 5
.equ QUEEN_VALUE, 9
.equ KING_VALUE, 0

.equ BOARD_SIZE, 8
.equ BOARD_SQUARES, BOARD_SIZE * BOARD_SIZE
.equ LAST_INDEX, BOARD_SIZE - 1

.equ RESULT_WIN, 1
.equ RESULT_DRAW, 2
.equ RESULT_LOSS, 3

.equ WIN_HALF_POINTS, 2
.equ DRAW_HALF_POINTS, 1
.equ LOSS_HALF_POINTS, 0

.text
.global piece_value
.global promote_pawn
.global empty_squares
.global on_board
.global result_points
.global match_result

/* extern int32_t piece_value(int32_t piece); */
piece_value:
        li      t0, PAWN
        beq     a0, t0, .Lpiece_value_pawn
        li      t0, KNIGHT
        beq     a0, t0, .Lpiece_value_knight
        li      t0, BISHOP
        beq     a0, t0, .Lpiece_value_bishop
        li      t0, ROOK
        beq     a0, t0, .Lpiece_value_rook
        li      t0, QUEEN
        beq     a0, t0, .Lpiece_value_queen
        li      a0, KING_VALUE
        ret
.Lpiece_value_pawn:
        li      a0, PAWN_VALUE
        ret
.Lpiece_value_knight:
        li      a0, KNIGHT_VALUE
        ret
.Lpiece_value_bishop:
        li      a0, BISHOP_VALUE
        ret
.Lpiece_value_rook:
        li      a0, ROOK_VALUE
        ret
.Lpiece_value_queen:
        li      a0, QUEEN_VALUE
        ret

/* extern int32_t promote_pawn(int32_t material); */
promote_pawn:
        addi    a0, a0, QUEEN_VALUE - PAWN_VALUE
        ret

/* extern int32_t empty_squares(int32_t pieces); */
empty_squares:
        li      t0, BOARD_SQUARES
        sub     a0, t0, a0
        ret

/* extern int32_t on_board(int32_t rank, int32_t file); */
on_board:
        li      t0, LAST_INDEX
        bltz    a0, .Lon_board_no
        bgt     a0, t0, .Lon_board_no
        bltz    a1, .Lon_board_no
        bgt     a1, t0, .Lon_board_no
        li      a0, 1
        ret
.Lon_board_no:
        li      a0, 0
        ret

/* extern int32_t result_points(int32_t result); */
result_points:
        li      t0, RESULT_WIN
        beq     a0, t0, .Lresult_points_win
        li      t0, RESULT_DRAW
        beq     a0, t0, .Lresult_points_draw
        li      a0, LOSS_HALF_POINTS
        ret
.Lresult_points_win:
        li      a0, WIN_HALF_POINTS
        ret
.Lresult_points_draw:
        li      a0, DRAW_HALF_POINTS
        ret

/* extern int32_t match_result(int32_t our_points, int32_t their_points); */
match_result:
        bgt     a0, a1, .Lmatch_result_win
        beq     a0, a1, .Lmatch_result_draw
        li      a0, RESULT_LOSS
        ret
.Lmatch_result_win:
        li      a0, RESULT_WIN
        ret
.Lmatch_result_draw:
        li      a0, RESULT_DRAW
        ret
