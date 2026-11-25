.option arch, +v                        # allow vector instructions in this file

.text
.global script_length
.global count_pauses
.global count_capitals
.global shout
.global find_cue
.global copy_line

/* extern int32_t script_length(const char *line); */
script_length:
        mv      t3, a0                  # where the line starts
.Lscript_length_loop:
        vsetvli t0, zero, e8, m1, ta, ma # as many bytes as fit
        vle8ff.v v8, (a0)               # load the bytes that can be read
        csrr    t0, vl                  # t0 = how many were loaded
        vmseq.vi v9, v8, 0              # mask: the zero byte
        vfirst.m t2, v9                 # its index in this strip, or -1
        bgez    t2, .Lscript_length_found
        add     a0, a0, t0              # move on by vl bytes
        j       .Lscript_length_loop
.Lscript_length_found:
        add     a0, a0, t2              # the address of the zero byte
        sub     a0, a0, t3              # minus the start is the length
        ret

/* extern int32_t count_pauses(const char *line); */
count_pauses:
        li      t3, 0                   # pauses counted so far
        li      t4, '/'
.Lcount_pauses_loop:
        vsetvli t0, zero, e8, m1, ta, ma
        vle8ff.v v8, (a0)
        csrr    t0, vl
        vmseq.vi v9, v8, 0
        vfirst.m t2, v9
        bltz    t2, .Lcount_pauses_count
        vsetvli t0, t2, e8, m1, ta, ma  # only the bytes before the zero byte
.Lcount_pauses_count:
        vmseq.vx v10, v8, t4            # mask: '/'
        vcpop.m t1, v10
        add     t3, t3, t1
        bgez    t2, .Lcount_pauses_done # the zero byte was in this strip
        add     a0, a0, t0
        j       .Lcount_pauses_loop
.Lcount_pauses_done:
        mv      a0, t3
        ret

/* extern int32_t count_capitals(const char *line); */
count_capitals:
        li      t3, 0                   # capitals counted so far
        li      t4, 'A' - 1
        li      t5, 'Z'
.Lcount_capitals_loop:
        vsetvli t0, zero, e8, m1, ta, ma
        vle8ff.v v8, (a0)
        csrr    t0, vl
        vmseq.vi v9, v8, 0
        vfirst.m t2, v9
        bltz    t2, .Lcount_capitals_count
        vsetvli t0, t2, e8, m1, ta, ma
.Lcount_capitals_count:
        vmsgtu.vx v10, v8, t4           # line[i] >= 'A'
        vmsleu.vx v11, v8, t5           # line[i] <= 'Z'
        vmand.mm v10, v10, v11          # both
        vcpop.m t1, v10
        add     t3, t3, t1
        bgez    t2, .Lcount_capitals_done
        add     a0, a0, t0
        j       .Lcount_capitals_loop
.Lcount_capitals_done:
        mv      a0, t3
        ret

/* extern void shout(char *line); */
shout:
        li      t4, 'a'
        li      t5, 'z' - 'a' + 1       # 26 letters
        li      t6, 'a' - 'A'
.Lshout_loop:
        vsetvli t0, zero, e8, m1, ta, mu # mu: the other bytes keep their values
        vle8ff.v v8, (a0)
        csrr    t0, vl
        vmseq.vi v9, v8, 0
        vfirst.m t2, v9
        bltz    t2, .Lshout_change
        vsetvli t0, t2, e8, m1, ta, mu  # stop before the zero byte
.Lshout_change:
        vsub.vx v10, v8, t4             # 0 to 25 for a lowercase letter
        vmsltu.vx v0, v10, t5           # mask: lowercase letters
        vsub.vx v8, v8, t6, v0.t        # make only those uppercase
        vse8.v  v8, (a0)
        bgez    t2, .Lshout_done
        add     a0, a0, t0
        j       .Lshout_loop
.Lshout_done:
        ret

/* extern int32_t find_cue(const char *line, char marker); */
find_cue:
        mv      t3, a0                  # where the line starts
.Lfind_cue_loop:
        vsetvli t0, zero, e8, m1, ta, ma
        vle8ff.v v8, (a0)
        csrr    t0, vl
        vmseq.vi v9, v8, 0
        vfirst.m t2, v9
        bltz    t2, .Lfind_cue_search
        vsetvli t0, t2, e8, m1, ta, ma
.Lfind_cue_search:
        vmseq.vx v10, v8, a1            # mask: the marker
        vfirst.m t1, v10
        bgez    t1, .Lfind_cue_found
        bgez    t2, .Lfind_cue_none     # reached the zero byte
        add     a0, a0, t0
        j       .Lfind_cue_loop
.Lfind_cue_found:
        sub     a0, a0, t3              # bytes before this strip
        add     a0, a0, t1
        ret
.Lfind_cue_none:
        li      a0, -1
        ret

/* extern void copy_line(char *dest, const char *src); */
copy_line:
.Lcopy_line_loop:
        vsetvli t0, zero, e8, m1, ta, ma
        vle8ff.v v8, (a1)
        csrr    t0, vl
        vmseq.vi v9, v8, 0
        vfirst.m t2, v9
        bltz    t2, .Lcopy_line_store
        addi    t1, t2, 1               # the bytes up to and including the zero byte
        vsetvli t0, t1, e8, m1, ta, ma
.Lcopy_line_store:
        vse8.v  v8, (a0)
        bgez    t2, .Lcopy_line_done
        add     a0, a0, t0
        add     a1, a1, t0
        j       .Lcopy_line_loop
.Lcopy_line_done:
        ret
