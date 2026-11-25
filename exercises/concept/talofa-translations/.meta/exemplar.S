.section .rodata
talofa_lava:
        .string "Talofa lava!"
faafetai:
        .string "Faʻafetai"
malo:
        .string "Mālō"
tenkyu:
        .string "Tenkyu"
thank_you_english:
        .string "Thank you"

month_days:                             # days in each month, January first
        .byte   31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31

        .balign 4
days_before_month:                      # days in the year before each month starts
        .word   0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334

.data
        .balign 4
visitor_count:
        .word   0

.text
.global greeting
.global thank_you
.global days_in_month
.global day_of_year
.global visitors

/* extern const char *greeting(void); */
greeting:
        la      a0, talofa_lava
        ret

/* extern const char *thank_you(int32_t language); */
thank_you:
        beqz    a0, .Lthank_you_samoan
        li      t0, 1
        beq     a0, t0, .Lthank_you_tongan
        li      t0, 2
        beq     a0, t0, .Lthank_you_tok_pisin
        la      a0, thank_you_english
        ret
.Lthank_you_samoan:
        la      a0, faafetai
        ret
.Lthank_you_tongan:
        la      a0, malo
        ret
.Lthank_you_tok_pisin:
        la      a0, tenkyu
        ret

/* extern int32_t days_in_month(int32_t month); */
days_in_month:
        la      t0, month_days
        add     t0, t0, a0
        lbu     a0, -1(t0)              # month_days[month - 1]
        ret

/* extern int32_t day_of_year(int32_t month, int32_t day); */
day_of_year:
        la      t0, days_before_month
        slli    t1, a0, 2
        add     t0, t0, t1
        lw      t0, -4(t0)              # days_before_month[month - 1]
        add     a0, t0, a1
        ret

/* extern int32_t visitors(void); */
visitors:
        la      t0, visitor_count
        lw      a0, 0(t0)
        addi    a0, a0, 1
        sw      a0, 0(t0)
        ret
