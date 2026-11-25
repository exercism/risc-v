.text
.global ladder_points
.global goals_per_quarter
.global average_margin
.global oranges_left
.global buses_needed

/* extern int32_t ladder_points(int32_t wins, int32_t draws); */
ladder_points:
        li      t0, 4
        mul     a0, a0, t0
        li      t1, 2
        mul     a1, a1, t1
        add     a0, a0, a1
        ret

/* extern int32_t goals_per_quarter(int32_t goals); */
goals_per_quarter:
        li      t0, 4
        div     a0, a0, t0
        ret

/* extern int32_t average_margin(int32_t total_margin, int32_t games); */
average_margin:
        div     a0, a0, a1
        ret

/* extern int32_t oranges_left(int32_t oranges, int32_t players); */
oranges_left:
        rem     a0, a0, a1
        ret

/* extern int32_t buses_needed(int32_t people, int32_t seats); */
buses_needed:
        add     a0, a0, a1
        addi    a0, a0, -1
        div     a0, a0, a1
        ret
