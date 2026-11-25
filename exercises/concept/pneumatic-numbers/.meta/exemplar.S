.section .rodata
symbols:                                # the symbol for each operator code
        .ascii  "+-*/"

        .balign 4
operations:                             # the function for each operator code
        .word   machine_add
        .word   machine_subtract
        .word   machine_multiply
        .word   machine_divide

program_steps:                          # the code for each step of a program
        .word   .Lrun_program_add_one
        .word   .Lrun_program_double
        .word   .Lrun_program_change_sign
        .word   .Lrun_program_clear

.text
.global operator_symbol
.global calculate
.global run_program
.global apply
.global operation

/* extern char operator_symbol(int32_t op); */
operator_symbol:
        li      t0, 4
        bgeu    a0, t0, .Loperator_symbol_unknown
        la      t1, symbols
        add     t1, t1, a0
        lbu     a0, 0(t1)
        ret
.Loperator_symbol_unknown:
        li      a0, '?'
        ret

/* extern int32_t calculate(int32_t op, int32_t a, int32_t b); */
calculate:
        li      t0, 4
        bgeu    a0, t0, .Lcalculate_unknown
        slli    t1, a0, 2
        la      t2, operations
        add     t2, t2, t1
        lw      t3, 0(t2)               # t3 = the function for op
        mv      a0, a1
        mv      a1, a2
        jr      t3                      # tail call: it returns to our caller
.Lcalculate_unknown:
        li      a0, 0
        ret

# The machine's four operations. Each takes a in a0 and b in a1.
machine_add:
        add     a0, a0, a1
        ret

machine_subtract:
        sub     a0, a0, a1
        ret

machine_multiply:
        mul     a0, a0, a1
        ret

machine_divide:
        beqz    a1, .Lmachine_divide_by_zero
        div     a0, a0, a1
        ret
.Lmachine_divide_by_zero:
        li      a0, 0
        ret

/* extern int32_t run_program(const int32_t *codes, int32_t count, int32_t start); */
run_program:
        slli    t0, a1, 2
        add     t0, a0, t0              # t0 = end of codes
        mv      t1, a2                  # t1 = the dial's value
        la      t2, program_steps
        li      t3, 4                   # the number of step codes
.Lrun_program_loop:
        beq     a0, t0, .Lrun_program_done
        lw      t4, 0(a0)               # t4 = the current code
        bgeu    t4, t3, .Lrun_program_next
        slli    t4, t4, 2
        add     t4, t2, t4
        lw      t4, 0(t4)               # t4 = the code address for this step
        jr      t4
.Lrun_program_add_one:
        addi    t1, t1, 1
        j       .Lrun_program_next
.Lrun_program_double:
        add     t1, t1, t1
        j       .Lrun_program_next
.Lrun_program_change_sign:
        neg     t1, t1
        j       .Lrun_program_next
.Lrun_program_clear:
        li      t1, 0
.Lrun_program_next:
        addi    a0, a0, 4
        j       .Lrun_program_loop
.Lrun_program_done:
        mv      a0, t1
        ret

/* extern int32_t apply(int32_t (*fn)(int32_t, int32_t), int32_t a, int32_t b); */
apply:
        mv      t0, a0
        mv      a0, a1
        mv      a1, a2
        jr      t0                      # tail call: fn returns to our caller

/* extern binary_op operation(int32_t op); */
operation:
        li      t0, 4
        bgeu    a0, t0, .Loperation_unknown
        slli    t1, a0, 2
        la      t2, operations
        add     t2, t2, t1
        lw      a0, 0(t2)
        ret
.Loperation_unknown:
        li      a0, 0
        ret
