.option arch, +v                        # allow vector instructions in this file

.text
.global fill_trays
.global add_extra
.global scale_recipe
.global add_portions
.global leftovers
.global room_on_trays

/* extern void fill_trays(int32_t *trays, int32_t count, int32_t portions); */
fill_trays:
        ret

/* extern void add_extra(int32_t *trays, int32_t count, int32_t extra); */
add_extra:
        ret

/* extern void scale_recipe(int32_t *grams, int32_t count, int32_t batches); */
scale_recipe:
        ret

/* extern void add_portions(int32_t *totals, const int32_t *first, const int32_t *second, int32_t count); */
add_portions:
        ret

/* extern void leftovers(int32_t *left, const int32_t *made, const int32_t *eaten, int32_t count); */
leftovers:
        ret

/* extern void room_on_trays(int32_t *room, const int32_t *on_tray, int32_t count, int32_t capacity); */
room_on_trays:
        ret
