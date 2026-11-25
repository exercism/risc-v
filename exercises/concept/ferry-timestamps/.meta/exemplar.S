.text
.global add_delay
.global journey_time
.global departs_before
.global minutes_to_us
.global arrival_time

/* extern uint64_t add_delay(uint64_t time_us, uint32_t delay_us); */
add_delay:
        add     a0, a0, a2              # low half of the new time
        sltu    t0, a0, a2              # carry: 1 if the low half wrapped around
        add     a1, a1, t0              # add the carry to the high half
        ret

/* extern uint64_t journey_time(uint64_t depart_us, uint64_t arrive_us); */
journey_time:
        sltu    t0, a2, a0              # borrow: 1 if arrive's low half < depart's
        sub     a0, a2, a0              # low half of arrive - depart
        sub     a1, a3, a1              # high half of arrive - depart
        sub     a1, a1, t0              # take away the borrow
        ret

/* extern int32_t departs_before(uint64_t first_us, uint64_t second_us); */
departs_before:
        bne     a1, a3, .Ldeparts_before_high   # different high halves decide
        sltu    a0, a0, a2              # equal high halves: compare the low halves
        ret
.Ldeparts_before_high:
        sltu    a0, a1, a3              # compare the high halves
        ret

/* extern uint64_t minutes_to_us(uint32_t minutes); */
minutes_to_us:
        li      t0, 60000000            # microseconds in a minute
        mulhu   a1, a0, t0              # high half of minutes * 60000000
        mul     a0, a0, t0              # low half of minutes * 60000000
        ret

/* extern uint64_t arrival_time(uint64_t depart_us, uint32_t minutes); */
arrival_time:
        li      t0, 60000000            # microseconds in a minute
        mulhu   t2, a2, t0              # high half of the journey in microseconds
        mul     t1, a2, t0              # low half of the journey in microseconds
        add     a0, a0, t1              # low half of the arrival time
        sltu    t3, a0, t1              # carry: 1 if the low half wrapped around
        add     a1, a1, t2              # high half of the arrival time
        add     a1, a1, t3              # add the carry
        ret
