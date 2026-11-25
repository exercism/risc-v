.option arch, +v                        # allow vector instructions in this file

.text
.global every_other
.global reverse_tune
.global zigzag
.global keep_high_notes
.global echo_previous

/* extern int32_t every_other(int32_t *dest, const int32_t *src, int32_t count); */
every_other:
        addi    a2, a2, 1
        srli    a2, a2, 1               # (count + 1) / 2 notes to copy
        mv      t3, a2                  # the answer
        li      t2, 8                   # stride: every second int32_t
.Levery_other_loop:
        blez    a2, .Levery_other_done
        vsetvli t0, a2, e32, m1, ta, ma
        vlse32.v v8, (a1), t2           # v8[i] = src[2 * i]
        vse32.v v8, (a0)
        slli    t1, t0, 3               # vl * 8 bytes read
        add     a1, a1, t1
        slli    t1, t0, 2               # vl * 4 bytes written
        add     a0, a0, t1
        sub     a2, a2, t0
        j       .Levery_other_loop
.Levery_other_done:
        mv      a0, t3
        ret

/* extern void reverse_tune(int32_t *dest, const int32_t *src, int32_t count); */
reverse_tune:
        slli    t1, a2, 2
        add     a1, a1, t1
        addi    a1, a1, -4              # a1 = &src[count - 1], the last note
        li      t2, -4                  # stride: one int32_t backwards
.Lreverse_tune_loop:
        blez    a2, .Lreverse_tune_done
        vsetvli t0, a2, e32, m1, ta, ma
        vlse32.v v8, (a1), t2           # v8[i] = the note i places before a1
        vse32.v v8, (a0)
        slli    t1, t0, 2               # vl * 4 bytes
        add     a0, a0, t1              # dest moves forwards
        sub     a1, a1, t1              # src moves backwards
        sub     a2, a2, t0
        j       .Lreverse_tune_loop
.Lreverse_tune_done:
        ret

/* extern void zigzag(int32_t *dest, const int32_t *src, int32_t count); */
zigzag:
.Lzigzag_loop:
        blez    a2, .Lzigzag_done
        vsetvli t0, a2, e32, m1, ta, ma
        andi    t0, t0, -2              # an even number of notes, so no pair is split
        vsetvli t0, t0, e32, m1, ta, ma # this fits, so vl = t0 exactly
        vle32.v v8, (a1)
        vid.v   v9                      # v9 = [0, 1, 2, 3, ...]
        vxor.vi v9, v9, 1               # v9 = [1, 0, 3, 2, ...]
        vrgather.vv v10, v8, v9         # v10[i] = v8[i ^ 1]
        vse32.v v10, (a0)
        slli    t1, t0, 2
        add     a0, a0, t1
        add     a1, a1, t1
        sub     a2, a2, t0
        j       .Lzigzag_loop
.Lzigzag_done:
        ret

/* extern int32_t keep_high_notes(int32_t *dest, const int32_t *src, int32_t count, int32_t threshold); */
keep_high_notes:
        li      t2, 0                   # notes kept so far
.Lkeep_high_notes_loop:
        blez    a2, .Lkeep_high_notes_done
        vsetvli t0, a2, e32, m1, ta, ma
        vle32.v v8, (a1)
        vmsgt.vx v0, v8, a3             # mask: src[i] > threshold
        vcompress.vm v10, v8, v0        # the high notes, packed at the front
        vcpop.m t3, v0                  # how many there are
        vsetvli zero, t3, e32, m1, ta, ma # store only those
        vse32.v v10, (a0)
        add     t2, t2, t3
        slli    t1, t3, 2               # dest moves on by the notes kept
        add     a0, a0, t1
        slli    t1, t0, 2               # src moves on by the whole strip
        add     a1, a1, t1
        sub     a2, a2, t0
        j       .Lkeep_high_notes_loop
.Lkeep_high_notes_done:
        mv      a0, t2
        ret

/* extern void echo_previous(int32_t *dest, const int32_t *src, int32_t count, int32_t first); */
echo_previous:
.Lecho_previous_loop:
        blez    a2, .Lecho_previous_done
        vsetvli t0, a2, e32, m1, ta, ma
        vle32.v v8, (a1)
        vslide1up.vx v9, v8, a3         # v9[0] = a3, v9[i] = v8[i - 1]
        vse32.v v9, (a0)
        addi    t1, t0, -1
        vslidedown.vx v10, v8, t1       # v10[0] = v8[vl - 1]
        vmv.x.s a3, v10                 # the last note goes first next round
        slli    t1, t0, 2
        add     a0, a0, t1
        add     a1, a1, t1
        sub     a2, a2, t0
        j       .Lecho_previous_loop
.Lecho_previous_done:
        ret
