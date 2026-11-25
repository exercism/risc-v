.text
.global letters_in_first
.global letters_in_third
.global total_of_four
.global deliver
.global move_letters
.global swap_pigeonholes

/* extern int32_t letters_in_first(const int32_t *row); */
letters_in_first:
        lw      a0, 0(a0)
        ret

/* extern int32_t letters_in_third(const int32_t *row); */
letters_in_third:
        lw      a0, 8(a0)
        ret

/* extern int32_t total_of_four(const int32_t *row); */
total_of_four:
        lw      t0, 0(a0)
        lw      t1, 4(a0)
        lw      t2, 8(a0)
        lw      t3, 12(a0)
        add     a0, t0, t1
        add     a0, a0, t2
        add     a0, a0, t3
        ret

/* extern void deliver(int32_t *pigeonhole, int32_t count); */
deliver:
        lw      t0, 0(a0)
        add     t0, t0, a1
        sw      t0, 0(a0)
        ret

/* extern void move_letters(int32_t *from, int32_t *to); */
move_letters:
        lw      t0, 0(a0)
        lw      t1, 0(a1)
        add     t1, t1, t0
        sw      zero, 0(a0)
        sw      t1, 0(a1)
        ret

/* extern void swap_pigeonholes(int32_t *a, int32_t *b); */
swap_pigeonholes:
        lw      t0, 0(a0)
        lw      t1, 0(a1)
        sw      t1, 0(a0)
        sw      t0, 0(a1)
        ret
