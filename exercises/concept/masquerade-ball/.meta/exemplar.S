.option arch, +v                        # allow vector instructions in this file

.text
.global count_colour
.global first_unmasked
.global cap_donations
.global discount_juniors
.global junior_donations
.global count_winners

/* extern int32_t count_colour(const int32_t *colours, int32_t count, int32_t colour); */
count_colour:
        li      t2, 0                   # guests counted so far
.Lcount_colour_loop:
        blez    a1, .Lcount_colour_done # no guests left
        vsetvli t0, a1, e32, m1, ta, ma # t0 = vl, the guests this round
        vle32.v v8, (a0)
        vmseq.vx v0, v8, a2             # mask: colours[i] == colour
        vcpop.m t3, v0                  # matches in this strip
        add     t2, t2, t3
        slli    t1, t0, 2               # vl * 4 bytes
        add     a0, a0, t1
        sub     a1, a1, t0
        j       .Lcount_colour_loop
.Lcount_colour_done:
        mv      a0, t2
        ret

/* extern int32_t first_unmasked(const int32_t *colours, int32_t count); */
first_unmasked:
        mv      t3, a0                  # where the array starts
.Lfirst_unmasked_loop:
        blez    a1, .Lfirst_unmasked_none
        vsetvli t0, a1, e32, m1, ta, ma
        vle32.v v8, (a0)
        vmseq.vi v0, v8, 0              # mask: colours[i] == 0, no mask worn
        vfirst.m t2, v0                 # index in this strip, or -1
        bgez    t2, .Lfirst_unmasked_found
        slli    t1, t0, 2
        add     a0, a0, t1
        sub     a1, a1, t0
        j       .Lfirst_unmasked_loop
.Lfirst_unmasked_found:
        sub     t1, a0, t3              # bytes before this strip
        srli    t1, t1, 2               # guests before this strip
        add     a0, t1, t2
        ret
.Lfirst_unmasked_none:
        li      a0, -1
        ret

/* extern void cap_donations(int32_t *donations, int32_t count, int32_t cap); */
cap_donations:
.Lcap_donations_loop:
        blez    a1, .Lcap_donations_done
        vsetvli t0, a1, e32, m1, ta, ma
        vle32.v v8, (a0)
        vmsgt.vx v0, v8, a2             # mask: donations[i] > cap
        vmerge.vxm v8, v8, a2, v0       # cap where the mask is set
        vse32.v v8, (a0)
        slli    t1, t0, 2
        add     a0, a0, t1
        sub     a1, a1, t0
        j       .Lcap_donations_loop
.Lcap_donations_done:
        ret

/* extern void discount_juniors(int32_t *prices, const int32_t *ages, int32_t count, int32_t age_limit, int32_t discount); */
discount_juniors:
.Ldiscount_juniors_loop:
        blez    a2, .Ldiscount_juniors_done
        vsetvli t0, a2, e32, m1, ta, mu # mu: inactive prices keep their values
        vle32.v v8, (a0)
        vle32.v v9, (a1)
        vmslt.vx v0, v9, a3             # mask: ages[i] < age_limit
        vsub.vx v8, v8, a4, v0.t        # discount only the juniors
        vse32.v v8, (a0)
        slli    t1, t0, 2
        add     a0, a0, t1
        add     a1, a1, t1
        sub     a2, a2, t0
        j       .Ldiscount_juniors_loop
.Ldiscount_juniors_done:
        ret

/* extern int32_t junior_donations(const int32_t *donations, const int32_t *ages, int32_t count, int32_t age_limit); */
junior_donations:
        li      t2, 0                   # total so far
.Ljunior_donations_loop:
        blez    a2, .Ljunior_donations_done
        vsetvli t0, a2, e32, m1, ta, ma
        vle32.v v8, (a0)
        vle32.v v9, (a1)
        vmslt.vx v0, v9, a3             # mask: ages[i] < age_limit
        vmv.s.x v10, t2                 # seed = total so far
        vredsum.vs v10, v8, v10, v0.t   # add only the juniors' donations
        vmv.x.s t2, v10
        slli    t1, t0, 2
        add     a0, a0, t1
        add     a1, a1, t1
        sub     a2, a2, t0
        j       .Ljunior_donations_loop
.Ljunior_donations_done:
        mv      a0, t2
        ret

/* extern int32_t count_winners(const uint32_t *tickets, int32_t count, uint32_t low, uint32_t high); */
count_winners:
        li      t2, 0                   # winners so far
.Lcount_winners_loop:
        blez    a1, .Lcount_winners_done
        vsetvli t0, a1, e32, m1, ta, ma
        vle32.v v8, (a0)
        vmsltu.vx v9, v8, a2            # tickets[i] < low
        vmnot.m v9, v9                  # tickets[i] >= low
        vmsleu.vx v10, v8, a3           # tickets[i] <= high
        vmand.mm v9, v9, v10            # both
        vcpop.m t3, v9
        add     t2, t2, t3
        slli    t1, t0, 2
        add     a0, a0, t1
        sub     a1, a1, t0
        j       .Lcount_winners_loop
.Lcount_winners_done:
        mv      a0, t2
        ret
