.text
.global item_price
.global restock
.global stock_value
.global items_in_aisle
.global heaviest_crate
.global items_to_shelve

/* extern int32_t item_price(const struct item *it); */
item_price:
        lw      a0, 4(a0)               # it->price_cents
        ret

/* extern void restock(struct item *it, int32_t delivered); */
restock:
        lw      t0, 8(a0)               # it->stock
        add     t0, t0, a1
        sw      t0, 8(a0)
        ret

/* extern int32_t stock_value(const struct item *items, int32_t count); */
stock_value:
        li      t0, 12                  # sizeof(struct item)
        mul     t0, a1, t0
        add     t0, a0, t0              # end
        li      t1, 0                   # total
stock_value_loop:
        beq     a0, t0, stock_value_done
        lw      t2, 4(a0)               # price_cents
        lw      t3, 8(a0)               # stock
        mul     t2, t2, t3
        add     t1, t1, t2
        addi    a0, a0, 12
        j       stock_value_loop
stock_value_done:
        mv      a0, t1
        ret

/* extern int32_t items_in_aisle(const struct item *items, int32_t count, int32_t aisle); */
items_in_aisle:
        li      t0, 12                  # sizeof(struct item)
        mul     t0, a1, t0
        add     t0, a0, t0              # end
        li      t1, 0                   # matching items
items_in_aisle_loop:
        beq     a0, t0, items_in_aisle_done
        lh      t2, 0(a0)               # aisle
        bne     t2, a2, items_in_aisle_next
        addi    t1, t1, 1
items_in_aisle_next:
        addi    a0, a0, 12
        j       items_in_aisle_loop
items_in_aisle_done:
        mv      a0, t1
        ret

/* extern int32_t heaviest_crate(const struct crate *crates, int32_t count); */
heaviest_crate:
        lw      t0, 4(a0)               # crates[0].weight_grams, after 2 bytes of padding
        li      t1, 0                   # index of the heaviest so far
        li      t2, 1                   # index of the current crate
        addi    a0, a0, 12              # start at crate 1
heaviest_crate_loop:
        bge     t2, a1, heaviest_crate_done
        lw      t3, 4(a0)               # weight_grams
        ble     t3, t0, heaviest_crate_next
        mv      t0, t3
        mv      t1, t2
heaviest_crate_next:
        addi    a0, a0, 12
        addi    t2, t2, 1
        j       heaviest_crate_loop
heaviest_crate_done:
        mv      a0, t1
        ret

/* extern int32_t items_to_shelve(const struct crate *crates, int32_t count, int32_t aisle); */
items_to_shelve:
        li      t0, 12                  # sizeof(struct crate)
        mul     t0, a1, t0
        add     t0, a0, t0              # end
        li      t1, 0                   # total items
items_to_shelve_loop:
        beq     a0, t0, items_to_shelve_done
        lh      t2, 8(a0)               # aisle
        bne     t2, a2, items_to_shelve_next
        lh      t3, 0(a0)               # items
        add     t1, t1, t3
items_to_shelve_next:
        addi    a0, a0, 12
        j       items_to_shelve_loop
items_to_shelve_done:
        mv      a0, t1
        ret
