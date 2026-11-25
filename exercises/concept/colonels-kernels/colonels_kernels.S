.text
.global item_price
.global restock
.global stock_value
.global items_in_aisle
.global heaviest_crate
.global items_to_shelve

/* extern int32_t item_price(const struct item *it); */
item_price:
        ret

/* extern void restock(struct item *it, int32_t delivered); */
restock:
        ret

/* extern int32_t stock_value(const struct item *items, int32_t count); */
stock_value:
        ret

/* extern int32_t items_in_aisle(const struct item *items, int32_t count, int32_t aisle); */
items_in_aisle:
        ret

/* extern int32_t heaviest_crate(const struct crate *crates, int32_t count); */
heaviest_crate:
        ret

/* extern int32_t items_to_shelve(const struct crate *crates, int32_t count, int32_t aisle); */
items_to_shelve:
        ret
