.text
.global hold_capacity
.global crates_with_spares
.global space_left
.global crates_on_board
.global checked_count

/* extern int32_t hold_capacity(void); */
hold_capacity:
        li      a0, 120
        ret

/* extern int32_t crates_with_spares(int32_t ordered); */
crates_with_spares:
        addi    a0, a0, 3
        ret

/* extern int32_t space_left(int32_t loaded); */
space_left:
        li      t0, 120
        sub     a0, t0, a0
        ret

/* extern int32_t crates_on_board(int32_t arriving, int32_t unloaded, int32_t loaded); */
crates_on_board:
        sub     a0, a0, a1
        add     a0, a0, a2
        ret

/* extern int32_t checked_count(int32_t clerk, int32_t crane); */
checked_count:
        mv      a0, a1
        ret
