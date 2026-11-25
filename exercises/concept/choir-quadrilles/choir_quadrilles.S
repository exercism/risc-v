.option arch, +v                        # allow vector instructions in this file

.text
.global every_other
.global reverse_tune
.global zigzag
.global keep_high_notes
.global echo_previous

/* extern int32_t every_other(int32_t *dest, const int32_t *src, int32_t count); */
every_other:
        ret

/* extern void reverse_tune(int32_t *dest, const int32_t *src, int32_t count); */
reverse_tune:
        ret

/* extern void zigzag(int32_t *dest, const int32_t *src, int32_t count); */
zigzag:
        ret

/* extern int32_t keep_high_notes(int32_t *dest, const int32_t *src, int32_t count, int32_t threshold); */
keep_high_notes:
        ret

/* extern void echo_previous(int32_t *dest, const int32_t *src, int32_t count, int32_t first); */
echo_previous:
        ret
