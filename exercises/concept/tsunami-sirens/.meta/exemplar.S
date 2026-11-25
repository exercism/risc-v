.text
.global larger
.global tide_change
.global alert_level
.global should_evacuate
.global siren_minutes

/* extern int32_t larger(int32_t a, int32_t b); */
larger:
        bge     a0, a1, .Llarger_done
        mv      a0, a1
.Llarger_done:
        ret

/* extern int32_t tide_change(int32_t earlier, int32_t later); */
tide_change:
        sub     a0, a1, a0
        bgez    a0, .Ltide_change_done
        sub     a0, zero, a0
.Ltide_change_done:
        ret

/* extern int32_t alert_level(int32_t height_cm); */
alert_level:
        li      t0, 30
        blt     a0, t0, .Lalert_level_0
        li      t0, 100
        blt     a0, t0, .Lalert_level_1
        li      t0, 300
        blt     a0, t0, .Lalert_level_2
        li      a0, 3
        ret
.Lalert_level_0:
        li      a0, 0
        ret
.Lalert_level_1:
        li      a0, 1
        ret
.Lalert_level_2:
        li      a0, 2
        ret

/* extern int32_t should_evacuate(int32_t level, int32_t strong_quake); */
should_evacuate:
        bnez    a1, .Lshould_evacuate_yes
        li      t0, 2
        bge     a0, t0, .Lshould_evacuate_yes
        li      a0, 0
        ret
.Lshould_evacuate_yes:
        li      a0, 1
        ret

/* extern int32_t siren_minutes(int32_t level); */
siren_minutes:
        beqz    a0, .Lsiren_minutes_done
        li      t0, 1
        bne     a0, t0, .Lsiren_minutes_not_1
        li      a0, 2
        j       .Lsiren_minutes_done
.Lsiren_minutes_not_1:
        li      t0, 2
        bne     a0, t0, .Lsiren_minutes_not_2
        li      a0, 5
        j       .Lsiren_minutes_done
.Lsiren_minutes_not_2:
        li      a0, 10
.Lsiren_minutes_done:
        ret
