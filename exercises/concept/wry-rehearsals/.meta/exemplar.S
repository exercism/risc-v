.text
.global line_length
.global count_char
.global first_word_length
.global is_shouted
.global whisper
.global copy_cue

/* extern int32_t line_length(const char *line); */
line_length:
        mv      t0, a0                  # pointer to the current character
.Lline_length_loop:
        lbu     t1, 0(t0)
        beqz    t1, .Lline_length_done
        addi    t0, t0, 1
        j       .Lline_length_loop
.Lline_length_done:
        sub     a0, t0, a0              # length = end - start
        ret

/* extern int32_t count_char(const char *line, char ch); */
count_char:
        li      t0, 0                   # count
.Lcount_char_loop:
        lbu     t1, 0(a0)
        beqz    t1, .Lcount_char_done
        bne     t1, a1, .Lcount_char_next
        addi    t0, t0, 1
.Lcount_char_next:
        addi    a0, a0, 1
        j       .Lcount_char_loop
.Lcount_char_done:
        mv      a0, t0
        ret

/* extern int32_t first_word_length(const char *line); */
first_word_length:
        mv      t0, a0                  # pointer to the current character
        li      t2, ' '
.Lfirst_word_length_loop:
        lbu     t1, 0(t0)
        beqz    t1, .Lfirst_word_length_done
        beq     t1, t2, .Lfirst_word_length_done
        addi    t0, t0, 1
        j       .Lfirst_word_length_loop
.Lfirst_word_length_done:
        sub     a0, t0, a0
        ret

/* extern int32_t is_shouted(const char *line); */
is_shouted:
        li      t0, 0                   # 1 once an uppercase letter is seen
        li      t3, 26                  # the number of letters
.Lis_shouted_loop:
        lbu     t1, 0(a0)
        beqz    t1, .Lis_shouted_done
        addi    t2, t1, -'a'
        bltu    t2, t3, .Lis_shouted_no # a lowercase letter
        addi    t2, t1, -'A'
        bgeu    t2, t3, .Lis_shouted_next
        li      t0, 1                   # an uppercase letter
.Lis_shouted_next:
        addi    a0, a0, 1
        j       .Lis_shouted_loop
.Lis_shouted_done:
        mv      a0, t0
        ret
.Lis_shouted_no:
        li      a0, 0
        ret

/* extern void whisper(char *line); */
whisper:
        li      t2, 26                  # the number of letters
.Lwhisper_loop:
        lbu     t0, 0(a0)
        beqz    t0, .Lwhisper_done
        addi    t1, t0, -'A'
        bgeu    t1, t2, .Lwhisper_next  # not an uppercase letter
        addi    t0, t0, 'a' - 'A'
        sb      t0, 0(a0)
.Lwhisper_next:
        addi    a0, a0, 1
        j       .Lwhisper_loop
.Lwhisper_done:
        ret

/* extern int32_t copy_cue(char *dest, const char *src); */
copy_cue:
        mv      t0, a0                  # where the next character goes
.Lcopy_cue_loop:
        lbu     t1, 0(a1)
        sb      t1, 0(t0)               # copies the zero terminator too
        beqz    t1, .Lcopy_cue_done
        addi    t0, t0, 1
        addi    a1, a1, 1
        j       .Lcopy_cue_loop
.Lcopy_cue_done:
        sub     a0, t0, a0              # the number of characters copied
        ret
