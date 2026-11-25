.text
.global plank_length
.global hull_length
.global timber_needed
.global fleet_length
.global fleet_cost

/* Provided by the tests: extern int32_t trim_board(int32_t cm); */

/* extern int32_t plank_length(int32_t boards, int32_t board_cm); */
plank_length:
        mul     a0, a0, a1
        ret

/* extern int32_t hull_length(int32_t boards, int32_t board_cm); */
hull_length:
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)

        mv      s0, a0                  # boards
        mv      a0, a1
        call    trim_board              # a0 = trimmed board length
        mv      a1, a0
        mv      a0, s0
        call    plank_length            # a0 = length of the plank
        addi    a0, a0, 60              # 30 cm at the bow and 30 cm at the stern

        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret

/* extern int32_t timber_needed(int32_t hull_boards, int32_t float_boards, int32_t board_cm); */
timber_needed:
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)
        sw      s1, 4(sp)
        sw      s2, 0(sp)

        mv      s0, a0                  # hull boards
        mv      s1, a1                  # float boards
        mv      a0, a2
        call    trim_board
        mv      s2, a0                  # trimmed board length
        mv      a0, s0
        mv      a1, s2
        call    plank_length            # a0 = hull plank
        mv      s0, a0                  # the hull boards are no longer needed
        mv      a0, s1
        mv      a1, s2
        call    plank_length            # a0 = float plank
        add     a0, a0, s0

        lw      s2, 0(sp)
        lw      s1, 4(sp)
        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret

/* extern int32_t fleet_length(int32_t canoes, int32_t boards, int32_t board_cm); */
fleet_length:
        addi    sp, sp, -32
        sw      ra, 28(sp)
        sw      s0, 24(sp)
        sw      s1, 20(sp)
        sw      s2, 16(sp)
        sw      s3, 12(sp)

        mv      s0, a0                  # canoes still to measure
        mv      s1, a1                  # boards in this hull
        mv      s2, a2                  # board length
        li      s3, 0                   # total length
fleet_length_loop:
        blez    s0, fleet_length_done
        mv      a0, s1
        mv      a1, s2
        call    hull_length
        add     s3, s3, a0
        addi    s1, s1, 1               # the next hull has one more board
        addi    s0, s0, -1
        j       fleet_length_loop
fleet_length_done:
        mv      a0, s3

        lw      s3, 12(sp)
        lw      s2, 16(sp)
        lw      s1, 20(sp)
        lw      s0, 24(sp)
        lw      ra, 28(sp)
        addi    sp, sp, 32
        ret

/* extern int32_t fleet_cost(int32_t canoes, int32_t boards, int32_t board_cm, int32_t price_per_metre); */
fleet_cost:
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)

        mv      s0, a3                  # price per metre
        call    fleet_length            # a0 = total length in cm
        mul     a0, a0, s0
        li      t0, 100
        div     a0, a0, t0              # centimetres to metres, rounding down

        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret
