.text
.global next_ticket
.global tickets_waiting
.global is_valid_counter
.global served_first
.global average_wait
.global seconds_part

/* extern uint32_t next_ticket(uint32_t last); */
next_ticket:
        ret

/* extern uint32_t tickets_waiting(uint32_t now_serving, uint32_t last_printed); */
tickets_waiting:
        ret

/* extern int32_t is_valid_counter(uint32_t counter); */
is_valid_counter:
        ret

/* extern int32_t served_first(uint32_t a_open, uint32_t b_open); */
served_first:
        ret

/* extern uint32_t average_wait(uint32_t total, uint32_t customers); */
average_wait:
        ret

/* extern uint32_t seconds_part(uint32_t time); */
seconds_part:
        ret
