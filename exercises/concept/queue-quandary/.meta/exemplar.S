.text
.global next_ticket
.global tickets_waiting
.global is_valid_counter
.global served_first
.global average_wait
.global seconds_part

/* extern uint32_t next_ticket(uint32_t last); */
next_ticket:
        addi    a0, a0, 1               # wraps from 0xFFFFFFFF to 0
        ret

/* extern uint32_t tickets_waiting(uint32_t now_serving, uint32_t last_printed); */
tickets_waiting:
        sub     a0, a1, a0              # correct even across the wrap
        ret

/* extern int32_t is_valid_counter(uint32_t counter); */
is_valid_counter:
        addi    a0, a0, -1              # 1..12 become 0..11, 0 becomes huge
        sltiu   a0, a0, 12              # a0 = (counter - 1 < 12), unsigned
        ret

/* extern int32_t served_first(uint32_t a_open, uint32_t b_open); */
served_first:
        sltu    a0, a1, a0              # a0 = (b_open < a_open)
        ret

/* extern uint32_t average_wait(uint32_t total, uint32_t customers); */
average_wait:
        beqz    a1, .Laverage_wait_none
        divu    a0, a0, a1
        ret
.Laverage_wait_none:
        li      a0, 0
        ret

/* extern uint32_t seconds_part(uint32_t time); */
seconds_part:
        li      t0, 1000000
        divu    a0, a0, t0              # whole seconds
        li      t0, 60
        remu    a0, a0, t0              # seconds after the whole minutes
        ret
