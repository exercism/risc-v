.text
.global runs_by
.global total_runs
.global highest_score
.global count_ducks
.global add_bonus
.global first_century

/* extern int32_t runs_by(const int32_t *scores, int32_t index); */
runs_by:
        slli    t0, a1, 2               # offset = 4 * index
        add     t0, a0, t0              # address of scores[index]
        lw      a0, 0(t0)
        ret

/* extern int32_t total_runs(const int32_t *scores, int32_t count); */
total_runs:
        slli    t0, a1, 2
        add     t0, a0, t0              # end
        li      t1, 0                   # total
total_runs_loop:
        beq     a0, t0, total_runs_done
        lw      t2, 0(a0)
        add     t1, t1, t2
        addi    a0, a0, 4
        j       total_runs_loop
total_runs_done:
        mv      a0, t1
        ret

/* extern int32_t highest_score(const int32_t *scores, int32_t count); */
highest_score:
        slli    t0, a1, 2
        add     t0, a0, t0              # end
        lw      t1, 0(a0)               # highest so far
        addi    a0, a0, 4
highest_score_loop:
        beq     a0, t0, highest_score_done
        lw      t2, 0(a0)
        ble     t2, t1, highest_score_next
        mv      t1, t2
highest_score_next:
        addi    a0, a0, 4
        j       highest_score_loop
highest_score_done:
        mv      a0, t1
        ret

/* extern int32_t count_ducks(const int32_t *scores, int32_t count); */
count_ducks:
        slli    t0, a1, 2
        add     t0, a0, t0              # end
        li      t1, 0                   # ducks
count_ducks_loop:
        beq     a0, t0, count_ducks_done
        lw      t2, 0(a0)
        bnez    t2, count_ducks_next
        addi    t1, t1, 1
count_ducks_next:
        addi    a0, a0, 4
        j       count_ducks_loop
count_ducks_done:
        mv      a0, t1
        ret

/* extern void add_bonus(int32_t *overs, int32_t count, int32_t bonus); */
add_bonus:
        slli    t0, a1, 2
        add     t0, a0, t0              # end
add_bonus_loop:
        beq     a0, t0, add_bonus_done
        lw      t1, 0(a0)
        add     t1, t1, a2
        sw      t1, 0(a0)
        addi    a0, a0, 4
        j       add_bonus_loop
add_bonus_done:
        ret

/* extern int32_t first_century(const int32_t *scores, int32_t count); */
first_century:
        slli    t0, a1, 2
        add     t0, a0, t0              # end
        mv      t1, a0                  # pointer to the current score
        li      t2, 100
first_century_loop:
        beq     t1, t0, first_century_none
        lw      t3, 0(t1)
        bge     t3, t2, first_century_found
        addi    t1, t1, 4
        j       first_century_loop
first_century_found:
        sub     a0, t1, a0              # offset in bytes
        srli    a0, a0, 2               # index
        ret
first_century_none:
        li      a0, -1
        ret
