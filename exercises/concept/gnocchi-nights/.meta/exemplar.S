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
.Lfill_trays_loop:
        blez    a1, .Lfill_trays_done   # no trays left
        vsetvli t0, a1, e32, m1, ta, ma # t0 = vl, the trays this round
        vmv.v.x v8, a2                  # every element = portions
        vse32.v v8, (a0)
        slli    t1, t0, 2               # vl * 4 bytes
        add     a0, a0, t1
        sub     a1, a1, t0
        j       .Lfill_trays_loop
.Lfill_trays_done:
        ret

/* extern void add_extra(int32_t *trays, int32_t count, int32_t extra); */
add_extra:
.Ladd_extra_loop:
        blez    a1, .Ladd_extra_done
        vsetvli t0, a1, e32, m1, ta, ma
        vle32.v v8, (a0)
        vadd.vx v8, v8, a2
        vse32.v v8, (a0)
        slli    t1, t0, 2
        add     a0, a0, t1
        sub     a1, a1, t0
        j       .Ladd_extra_loop
.Ladd_extra_done:
        ret

/* extern void scale_recipe(int32_t *grams, int32_t count, int32_t batches); */
scale_recipe:
.Lscale_recipe_loop:
        blez    a1, .Lscale_recipe_done
        vsetvli t0, a1, e32, m1, ta, ma
        vle32.v v8, (a0)
        vmul.vx v8, v8, a2
        vse32.v v8, (a0)
        slli    t1, t0, 2
        add     a0, a0, t1
        sub     a1, a1, t0
        j       .Lscale_recipe_loop
.Lscale_recipe_done:
        ret

/* extern void add_portions(int32_t *totals, const int32_t *first, const int32_t *second, int32_t count); */
add_portions:
.Ladd_portions_loop:
        blez    a3, .Ladd_portions_done
        vsetvli t0, a3, e32, m1, ta, ma
        vle32.v v8, (a1)
        vle32.v v9, (a2)
        vadd.vv v8, v8, v9
        vse32.v v8, (a0)
        slli    t1, t0, 2
        add     a0, a0, t1
        add     a1, a1, t1
        add     a2, a2, t1
        sub     a3, a3, t0
        j       .Ladd_portions_loop
.Ladd_portions_done:
        ret

/* extern void leftovers(int32_t *left, const int32_t *made, const int32_t *eaten, int32_t count); */
leftovers:
.Lleftovers_loop:
        blez    a3, .Lleftovers_done
        vsetvli t0, a3, e32, m1, ta, ma
        vle32.v v8, (a1)
        vle32.v v9, (a2)
        vsub.vv v8, v8, v9              # made - eaten
        vse32.v v8, (a0)
        slli    t1, t0, 2
        add     a0, a0, t1
        add     a1, a1, t1
        add     a2, a2, t1
        sub     a3, a3, t0
        j       .Lleftovers_loop
.Lleftovers_done:
        ret

/* extern void room_on_trays(int32_t *room, const int32_t *on_tray, int32_t count, int32_t capacity); */
room_on_trays:
.Lroom_on_trays_loop:
        blez    a2, .Lroom_on_trays_done
        vsetvli t0, a2, e32, m1, ta, ma
        vle32.v v8, (a1)
        vrsub.vx v8, v8, a3             # capacity - on_tray
        vse32.v v8, (a0)
        slli    t1, t0, 2
        add     a0, a0, t1
        add     a1, a1, t1
        sub     a2, a2, t0
        j       .Lroom_on_trays_loop
.Lroom_on_trays_done:
        ret
