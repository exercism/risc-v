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
        ret

/* extern int32_t first_hatched(uint32_t nest); */
first_hatched:
        ret

/* extern int32_t last_hatched(uint32_t nest); */
last_hatched:
        ret

/* extern int32_t still_waiting(uint32_t eggs, uint32_t nest); */
still_waiting:
        ret

/* extern int32_t clamp_reading(int32_t reading, int32_t low, int32_t high); */
clamp_reading:
        ret

/* extern uint32_t rotate_watch(uint32_t rota, int32_t nights); */
rotate_watch:
        ret
