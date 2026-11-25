.option arch, +v                        # allow vector instructions in this file

.text
.global total_time
.global fastest_lap
.global best_improvement
.global longest_paddle
.global team_total
.global lap_spread

/* extern int32_t total_time(const int32_t *laps, int32_t count); */
total_time:
        li      t2, 0                   # total so far
.Ltotal_time_loop:
        blez    a1, .Ltotal_time_done   # no laps left
        vsetvli t0, a1, e32, m1, ta, ma # t0 = vl, the laps this round
        vle32.v v8, (a0)
        vmv.s.x v9, t2                  # seed = total so far
        vredsum.vs v9, v8, v9
        vmv.x.s t2, v9
        slli    t1, t0, 2               # vl * 4 bytes
        add     a0, a0, t1
        sub     a1, a1, t0
        j       .Ltotal_time_loop
.Ltotal_time_done:
        mv      a0, t2
        ret

/* extern int32_t fastest_lap(const int32_t *laps, int32_t count); */
fastest_lap:
        lw      t2, 0(a0)               # seed = the first lap
.Lfastest_lap_loop:
        blez    a1, .Lfastest_lap_done
        vsetvli t0, a1, e32, m1, ta, ma
        vle32.v v8, (a0)
        vmv.s.x v9, t2
        vredmin.vs v9, v8, v9
        vmv.x.s t2, v9
        slli    t1, t0, 2
        add     a0, a0, t1
        sub     a1, a1, t0
        j       .Lfastest_lap_loop
.Lfastest_lap_done:
        mv      a0, t2
        ret

/* extern int32_t best_improvement(const int32_t *changes, int32_t count); */
best_improvement:
        lw      t2, 0(a0)               # seed = the first change, never 0
.Lbest_improvement_loop:
        blez    a1, .Lbest_improvement_done
        vsetvli t0, a1, e32, m1, ta, ma
        vle32.v v8, (a0)
        vmv.s.x v9, t2
        vredmax.vs v9, v8, v9
        vmv.x.s t2, v9
        slli    t1, t0, 2
        add     a0, a0, t1
        sub     a1, a1, t0
        j       .Lbest_improvement_loop
.Lbest_improvement_done:
        mv      a0, t2
        ret

/* extern uint32_t longest_paddle(const uint32_t *millimetres, int32_t count); */
longest_paddle:
        li      t2, 0                   # seed = 0, the smallest unsigned number
.Llongest_paddle_loop:
        blez    a1, .Llongest_paddle_done
        vsetvli t0, a1, e32, m1, ta, ma
        vle32.v v8, (a0)
        vmv.s.x v9, t2
        vredmaxu.vs v9, v8, v9          # unsigned maximum
        vmv.x.s t2, v9
        slli    t1, t0, 2
        add     a0, a0, t1
        sub     a1, a1, t0
        j       .Llongest_paddle_loop
.Llongest_paddle_done:
        mv      a0, t2
        ret

/* extern int32_t team_total(const int32_t *first, const int32_t *second, int32_t count); */
team_total:
        li      t2, 0
.Lteam_total_loop:
        blez    a2, .Lteam_total_done
        vsetvli t0, a2, e32, m1, ta, ma
        vle32.v v8, (a0)
        vle32.v v10, (a1)
        vadd.vv v8, v8, v10             # first[i] + second[i]
        vmv.s.x v9, t2
        vredsum.vs v9, v8, v9
        vmv.x.s t2, v9
        slli    t1, t0, 2
        add     a0, a0, t1
        add     a1, a1, t1
        sub     a2, a2, t0
        j       .Lteam_total_loop
.Lteam_total_done:
        mv      a0, t2
        ret

/* extern int32_t lap_spread(const int32_t *laps, int32_t count); */
lap_spread:
        lw      t2, 0(a0)               # fastest so far
        mv      t3, t2                  # slowest so far
.Llap_spread_loop:
        blez    a1, .Llap_spread_done
        vsetvli t0, a1, e32, m1, ta, ma
        vle32.v v8, (a0)
        vmv.s.x v9, t2
        vredmin.vs v9, v8, v9
        vmv.x.s t2, v9
        vmv.s.x v10, t3
        vredmax.vs v10, v8, v10
        vmv.x.s t3, v10
        slli    t1, t0, 2
        add     a0, a0, t1
        sub     a1, a1, t0
        j       .Llap_spread_loop
.Llap_spread_done:
        sub     a0, t3, t2              # slowest - fastest
        ret
