.text
.global add_delay
.global journey_time
.global departs_before
.global minutes_to_us
.global arrival_time

/* extern uint64_t add_delay(uint64_t time_us, uint32_t delay_us); */
add_delay:
        ret

/* extern uint64_t journey_time(uint64_t depart_us, uint64_t arrive_us); */
journey_time:
        ret

/* extern int32_t departs_before(uint64_t first_us, uint64_t second_us); */
departs_before:
        ret

/* extern uint64_t minutes_to_us(uint32_t minutes); */
minutes_to_us:
        ret

/* extern uint64_t arrival_time(uint64_t depart_us, uint32_t minutes); */
arrival_time:
        ret
