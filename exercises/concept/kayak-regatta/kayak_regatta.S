.option arch, +v                        # allow vector instructions in this file

.text
.global total_time
.global fastest_lap
.global best_improvement
.global longest_paddle
.global team_total
.global lap_spread

/* extern int32_t total_time(const int32_t *laps, int32_t count); */
total_time:
        ret

/* extern int32_t fastest_lap(const int32_t *laps, int32_t count); */
fastest_lap:
        ret

/* extern int32_t best_improvement(const int32_t *changes, int32_t count); */
best_improvement:
        ret

/* extern uint32_t longest_paddle(const uint32_t *millimetres, int32_t count); */
longest_paddle:
        ret

/* extern int32_t team_total(const int32_t *first, const int32_t *second, int32_t count); */
team_total:
        ret

/* extern int32_t lap_spread(const int32_t *laps, int32_t count); */
lap_spread:
        ret
