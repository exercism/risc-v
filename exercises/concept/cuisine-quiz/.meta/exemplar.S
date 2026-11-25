.text
.global format_score
.global format_signed
.global format_padded
.global parse_score
.global parse_signed

/* extern int32_t format_score(char *buf, uint32_t score); */
format_score:
        mv      t0, a0                  # pointer to the next free byte
        li      t3, 10
.Lformat_score_digit:
        remu    t1, a1, t3              # the last digit
        addi    t1, t1, '0'
        sb      t1, 0(t0)
        addi    t0, t0, 1
        divu    a1, a1, t3              # the other digits
        bnez    a1, .Lformat_score_digit
        sb      zero, 0(t0)             # end the string
        sub     t4, t0, a0              # the length
        addi    t0, t0, -1              # the last digit written
.Lformat_score_reverse:
        bgeu    a0, t0, .Lformat_score_done
        lbu     t1, 0(a0)
        lbu     t2, 0(t0)
        sb      t2, 0(a0)
        sb      t1, 0(t0)
        addi    a0, a0, 1
        addi    t0, t0, -1
        j       .Lformat_score_reverse
.Lformat_score_done:
        mv      a0, t4
        ret

/* extern int32_t format_signed(char *buf, int32_t points); */
format_signed:
        mv      t5, a0                  # the start of the text, sign included
        bgez    a1, .Lformat_signed_digits
        li      t1, '-'
        sb      t1, 0(a0)
        addi    a0, a0, 1               # the digits start after the sign
        neg     a1, a1                  # the magnitude, read as unsigned
.Lformat_signed_digits:
        mv      t0, a0                  # pointer to the next free byte
        li      t3, 10
.Lformat_signed_digit:
        remu    t1, a1, t3
        addi    t1, t1, '0'
        sb      t1, 0(t0)
        addi    t0, t0, 1
        divu    a1, a1, t3
        bnez    a1, .Lformat_signed_digit
        sb      zero, 0(t0)
        sub     t4, t0, t5              # the length, sign included
        addi    t0, t0, -1
.Lformat_signed_reverse:
        bgeu    a0, t0, .Lformat_signed_done
        lbu     t1, 0(a0)
        lbu     t2, 0(t0)
        sb      t2, 0(a0)
        sb      t1, 0(t0)
        addi    a0, a0, 1
        addi    t0, t0, -1
        j       .Lformat_signed_reverse
.Lformat_signed_done:
        mv      a0, t4
        ret

/* extern int32_t format_padded(char *buf, uint32_t n, int32_t width); */
format_padded:
        li      t3, 10
        mv      t1, a1                  # a copy of n, to count its digits
        li      t0, 0                   # the number of digits
.Lformat_padded_count:
        addi    t0, t0, 1
        divu    t1, t1, t3
        bnez    t1, .Lformat_padded_count
        bge     t0, a2, .Lformat_padded_write
        mv      t0, a2                  # pad up to the width
.Lformat_padded_write:
        add     t2, a0, t0              # the end of the text
        sb      zero, 0(t2)
.Lformat_padded_digit:
        addi    t2, t2, -1
        remu    t1, a1, t3              # 0 once n runs out of digits
        addi    t1, t1, '0'
        sb      t1, 0(t2)
        divu    a1, a1, t3
        bne     t2, a0, .Lformat_padded_digit
        mv      a0, t0
        ret

/* extern uint32_t parse_score(const char *text); */
parse_score:
        mv      t2, a0                  # pointer to the current character
        li      a0, 0                   # the number so far
        li      t0, 10
.Lparse_score_loop:
        lbu     t1, 0(t2)
        addi    t1, t1, -'0'
        bgeu    t1, t0, .Lparse_score_done  # not a digit
        mul     a0, a0, t0
        add     a0, a0, t1
        addi    t2, t2, 1
        j       .Lparse_score_loop
.Lparse_score_done:
        ret

/* extern int32_t parse_signed(const char *text); */
parse_signed:
        mv      t2, a0                  # pointer to the current character
        li      a0, 0                   # the magnitude so far
        li      t0, 10
        lbu     t1, 0(t2)
        addi    t3, t1, -'-'
        seqz    t3, t3                  # 1 if the text starts with '-'
        add     t2, t2, t3              # skip the '-'
.Lparse_signed_loop:
        lbu     t1, 0(t2)
        addi    t1, t1, -'0'
        bgeu    t1, t0, .Lparse_signed_done
        mul     a0, a0, t0
        add     a0, a0, t1
        addi    t2, t2, 1
        j       .Lparse_signed_loop
.Lparse_signed_done:
        beqz    t3, .Lparse_signed_positive
        neg     a0, a0
.Lparse_signed_positive:
        ret
