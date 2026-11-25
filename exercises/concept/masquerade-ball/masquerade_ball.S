.option arch, +v                        # allow vector instructions in this file

.text
.global count_colour
.global first_unmasked
.global cap_donations
.global discount_juniors
.global junior_donations
.global count_winners

/* extern int32_t count_colour(const int32_t *colours, int32_t count, int32_t colour); */
count_colour:
        ret

/* extern int32_t first_unmasked(const int32_t *colours, int32_t count); */
first_unmasked:
        ret

/* extern void cap_donations(int32_t *donations, int32_t count, int32_t cap); */
cap_donations:
        ret

/* extern void discount_juniors(int32_t *prices, const int32_t *ages, int32_t count, int32_t age_limit, int32_t discount); */
discount_juniors:
        ret

/* extern int32_t junior_donations(const int32_t *donations, const int32_t *ages, int32_t count, int32_t age_limit); */
junior_donations:
        ret

/* extern int32_t count_winners(const uint32_t *tickets, int32_t count, uint32_t low, uint32_t high); */
count_winners:
        ret
