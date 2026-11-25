.text
.global first_four
.global add_knocks
.global flip_knocks
.global remove_knocks
.global matching_knocks

/* extern uint32_t first_four(uint32_t code); */
first_four:
        andi    a0, a0, 0xF
        ret

/* extern uint32_t add_knocks(uint32_t code, uint32_t extra); */
add_knocks:
        or      a0, a0, a1
        ret

/* extern uint32_t flip_knocks(uint32_t code, uint32_t chosen); */
flip_knocks:
        xor     a0, a0, a1
        ret

/* extern uint32_t remove_knocks(uint32_t code, uint32_t marked); */
remove_knocks:
        not     t0, a1
        and     a0, a0, t0
        ret

/* extern uint32_t matching_knocks(uint32_t heard, uint32_t code); */
matching_knocks:
        xor     a0, a0, a1
        not     a0, a0
        andi    a0, a0, 0xFF
        ret
