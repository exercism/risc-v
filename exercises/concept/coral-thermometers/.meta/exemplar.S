.text
.global reading_at
.global coolest
.global total_change
.global deepest
.global calibrate
.global count_bleaching_risk

/* extern int32_t reading_at(const int16_t *log, int32_t index); */
reading_at:
        slli    t0, a1, 1               # offset = 2 * index
        add     t0, a0, t0              # address of log[index]
        lh      a0, 0(t0)
        ret

/* extern int32_t coolest(const int16_t *log, int32_t count); */
coolest:
        slli    t0, a1, 1
        add     t0, a0, t0              # end
        lh      t1, 0(a0)               # coolest so far
        addi    a0, a0, 2
coolest_loop:
        beq     a0, t0, coolest_done
        lh      t2, 0(a0)
        bge     t2, t1, coolest_next
        mv      t1, t2
coolest_next:
        addi    a0, a0, 2
        j       coolest_loop
coolest_done:
        mv      a0, t1
        ret

/* extern int32_t total_change(const int16_t *changes, int32_t count); */
total_change:
        slli    t0, a1, 1
        add     t0, a0, t0              # end
        li      t1, 0                   # total
total_change_loop:
        beq     a0, t0, total_change_done
        lh      t2, 0(a0)
        add     t1, t1, t2
        addi    a0, a0, 2
        j       total_change_loop
total_change_done:
        mv      a0, t1
        ret

/* extern int32_t deepest(const uint16_t *depths, int32_t count); */
deepest:
        slli    t0, a1, 1
        add     t0, a0, t0              # end
        lhu     t1, 0(a0)               # deepest so far
        addi    a0, a0, 2
deepest_loop:
        beq     a0, t0, deepest_done
        lhu     t2, 0(a0)
        ble     t2, t1, deepest_next
        mv      t1, t2
deepest_next:
        addi    a0, a0, 2
        j       deepest_loop
deepest_done:
        mv      a0, t1
        ret

/* extern void calibrate(int16_t *log, int32_t count, int32_t offset); */
calibrate:
        slli    t0, a1, 1
        add     t0, a0, t0              # end
calibrate_loop:
        beq     a0, t0, calibrate_done
        lh      t1, 0(a0)
        add     t1, t1, a2
        sh      t1, 0(a0)
        addi    a0, a0, 2
        j       calibrate_loop
calibrate_done:
        ret

/* extern int32_t count_bleaching_risk(const int16_t *log, int32_t count, int32_t threshold); */
count_bleaching_risk:
        slli    t0, a1, 1
        add     t0, a0, t0              # end
        li      t1, 0                   # risky readings
count_bleaching_risk_loop:
        beq     a0, t0, count_bleaching_risk_done
        lh      t2, 0(a0)
        blt     t2, a2, count_bleaching_risk_next
        addi    t1, t1, 1
count_bleaching_risk_next:
        addi    a0, a0, 2
        j       count_bleaching_risk_loop
count_bleaching_risk_done:
        mv      a0, t1
        ret
