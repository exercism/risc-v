.text
.global split_clue
.global desk_neighbours
.global case_summary
.global update_tally
.global unscramble

/* extern void split_clue(int32_t first, int32_t second, int32_t *sum, int32_t *difference); */
split_clue:
        add     t0, a0, a1
        sub     t1, a0, a1
        sw      t0, 0(a2)
        sw      t1, 0(a3)
        ret

/* extern int32_t desk_neighbours(int32_t desk, int32_t *left); */
desk_neighbours:
        addi    t0, a0, -1
        sw      t0, 0(a1)
        addi    a0, a0, 1
        ret

/* extern void case_summary(const int32_t *clues, int32_t *total, int32_t *latest); */
case_summary:
        lw      t0, 0(a0)
        lw      t1, 4(a0)
        lw      t2, 8(a0)
        add     t3, t0, t1
        add     t3, t3, t2
        sw      t3, 0(a1)
        sw      t2, 0(a2)
        ret

/* extern int32_t update_tally(int32_t *tally, int32_t found); */
update_tally:
        lw      t0, 0(a0)
        add     t1, t0, a1
        sw      t1, 0(a0)
        mv      a0, t0
        ret

/* extern void unscramble(int32_t *first, int32_t *second); */
unscramble:
        lw      t0, 0(a0)
        lw      t1, 0(a1)
        add     t2, t0, t1
        sub     t3, t0, t1
        sw      t2, 0(a0)
        sw      t3, 0(a1)
        ret
