.option arch, +zbb                      # allow Zbb instructions in this file

.text
.global hatched
.global first_hatched
.global last_hatched
.global still_waiting
.global clamp_reading
.global rotate_watch

/* extern int32_t hatched(uint32_t nest); */
hatched:
        cpop    a0, a0                  # count the set bits
        ret

/* extern int32_t first_hatched(uint32_t nest); */
first_hatched:
        bnez    a0, .Lfirst_hatched_found
        li      a0, -1                  # no egg has hatched
        ret
.Lfirst_hatched_found:
        ctz     a0, a0                  # position of the lowest set bit
        ret

/* extern int32_t last_hatched(uint32_t nest); */
last_hatched:
        clz     a0, a0                  # 32 for an empty nest
        li      t0, 31
        sub     a0, t0, a0              # position of the highest set bit, or -1
        ret

/* extern int32_t still_waiting(uint32_t eggs, uint32_t nest); */
still_waiting:
        andn    a0, a0, a1              # places with an egg that has not hatched
        cpop    a0, a0
        ret

/* extern int32_t clamp_reading(int32_t reading, int32_t low, int32_t high); */
clamp_reading:
        max     a0, a0, a1              # no lower than low
        min     a0, a0, a2              # no higher than high
        ret

/* extern uint32_t rotate_watch(uint32_t rota, int32_t nights); */
rotate_watch:
        rol     a0, a0, a1              # bit 31 comes round to bit 0
        ret
