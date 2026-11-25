.text
.global sight
.global has_passed
.global missing
.global boats_seen
.global first_boat
.global log_from_flags

/* extern uint32_t sight(uint32_t log, int32_t boat); */
sight:
        li      t0, 1
        sll     t0, t0, a1              # t0 = 1 << boat
        or      a0, a0, t0              # add the boat to the log
        ret

/* extern int32_t has_passed(uint32_t log, int32_t boat); */
has_passed:
        srl     a0, a0, a1              # move the boat's bit down to bit 0
        andi    a0, a0, 1               # keep only that bit
        ret

/* extern uint32_t missing(uint32_t expected, uint32_t seen); */
missing:
        not     t0, a1                  # t0 = every boat not seen
        and     a0, a0, t0              # expected boats that were not seen
        ret

/* extern int32_t boats_seen(uint32_t log); */
boats_seen:
        li      t0, 0                   # count
.Lboats_seen_loop:
        beqz    a0, .Lboats_seen_done
        addi    t1, a0, -1
        and     a0, a0, t1              # remove the smallest boat
        addi    t0, t0, 1
        j       .Lboats_seen_loop
.Lboats_seen_done:
        mv      a0, t0
        ret

/* extern int32_t first_boat(uint32_t log); */
first_boat:
        mv      t0, a0                  # the boats not yet checked
        li      a0, -1
        beqz    t0, .Lfirst_boat_done   # an empty log has no first boat
        li      a0, 0                   # the boat number to check
.Lfirst_boat_loop:
        andi    t1, t0, 1
        bnez    t1, .Lfirst_boat_done   # boat a0 is in the log
        srli    t0, t0, 1               # move the next boat down to bit 0
        addi    a0, a0, 1
        j       .Lfirst_boat_loop
.Lfirst_boat_done:
        ret

/* extern uint32_t log_from_flags(const char *flags); */
log_from_flags:
        li      t0, 0                   # the log
        li      t3, 1
.Llog_from_flags_loop:
        lbu     t1, 0(a0)
        beqz    t1, .Llog_from_flags_done
        addi    t1, t1, -'a'            # 0 to 25 for a lowercase letter
        sltiu   t2, t1, 26
        beqz    t2, .Llog_from_flags_next
        sll     t2, t3, t1              # t2 = 1 << boat
        or      t0, t0, t2
.Llog_from_flags_next:
        addi    a0, a0, 1
        j       .Llog_from_flags_loop
.Llog_from_flags_done:
        mv      a0, t0
        ret
