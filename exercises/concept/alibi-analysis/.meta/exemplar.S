.text
.global is_earlier
.global alibis_agree
.global was_present
.global is_suspect
.global is_cleared

/* extern int32_t is_earlier(int32_t a, int32_t b); */
is_earlier:
        slt     a0, a0, a1              # a0 = (a < b)
        ret

/* extern int32_t alibis_agree(int32_t a, int32_t b); */
alibis_agree:
        sub     a0, a0, a1              # a0 = a - b, zero only when a == b
        seqz    a0, a0                  # a0 = (a == b)
        ret

/* extern int32_t was_present(int32_t arrive, int32_t leave, int32_t t); */
was_present:
        sgt     t0, a0, a2              # t0 = (arrive > t)
        xori    t0, t0, 1               # t0 = (arrive <= t)
        slt     t1, a2, a1              # t1 = (t < leave)
        and     a0, t0, t1              # both must hold
        ret

/* extern int32_t is_suspect(int32_t present, int32_t has_motive); */
is_suspect:
        and     a0, a0, a1              # present and has_motive
        ret

/* extern int32_t is_cleared(int32_t away, int32_t witnesses); */
is_cleared:
        snez    t0, a1                  # t0 = (witnesses != 0)
        or      a0, a0, t0              # away or has a witness
        ret
