.text
.global operator_symbol
.global calculate
.global run_program
.global apply
.global operation

/* extern char operator_symbol(int32_t op); */
operator_symbol:
        ret

/* extern int32_t calculate(int32_t op, int32_t a, int32_t b); */
calculate:
        ret

/* extern int32_t run_program(const int32_t *codes, int32_t count, int32_t start); */
run_program:
        ret

/* extern int32_t apply(int32_t (*fn)(int32_t, int32_t), int32_t a, int32_t b); */
apply:
        ret

/* extern binary_op operation(int32_t op); */
operation:
        ret
