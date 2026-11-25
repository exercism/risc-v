#include "vendor/unity.h"

#include <stddef.h>
#include <stdint.h>

struct item {
    int16_t aisle;
    int16_t shelf;
    int32_t price_cents;
    int32_t stock;
};

struct crate {
    int16_t items;
    int32_t weight_grams;
    int16_t aisle;
};

_Static_assert(offsetof(struct item, aisle) == 0, "item.aisle is at offset 0");
_Static_assert(offsetof(struct item, shelf) == 2, "item.shelf is at offset 2");
_Static_assert(offsetof(struct item, price_cents) == 4, "item.price_cents is at offset 4");
_Static_assert(offsetof(struct item, stock) == 8, "item.stock is at offset 8");
_Static_assert(sizeof(struct item) == 12, "struct item takes 12 bytes");
_Static_assert(offsetof(struct crate, items) == 0, "crate.items is at offset 0");
_Static_assert(offsetof(struct crate, weight_grams) == 4, "crate.weight_grams is at offset 4");
_Static_assert(offsetof(struct crate, aisle) == 8, "crate.aisle is at offset 8");
_Static_assert(sizeof(struct crate) == 12, "struct crate takes 12 bytes");

extern int32_t item_price(const struct item *it);
extern void restock(struct item *it, int32_t delivered);
extern int32_t stock_value(const struct item *items, int32_t count);
extern int32_t items_in_aisle(const struct item *items, int32_t count, int32_t aisle);
extern int32_t heaviest_crate(const struct crate *crates, int32_t count);
extern int32_t items_to_shelve(const struct crate *crates, int32_t count, int32_t aisle);

void setUp(void) {
}

void tearDown(void) {
}

// TASK: 1
void test_price_of_popcorn_kernels(void) {
    const struct item popcorn = {.aisle = 2, .shelf = 1, .price_cents = 450, .stock = 20};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(450, item_price(&popcorn), "item_price(&popcorn)");
}

void test_price_of_fishing_line(void) {
    TEST_IGNORE();
    const struct item line = {.aisle = 5, .shelf = 3, .price_cents = 1299, .stock = 4};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1299, item_price(&line), "item_price(&line)");
}

void test_price_of_an_item_out_of_stock(void) {
    TEST_IGNORE();
    const struct item batteries = {.aisle = 3, .shelf = 2, .price_cents = 899, .stock = 0};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(899, item_price(&batteries), "item_price(&batteries)");
}

void test_price_of_a_large_sack(void) {
    TEST_IGNORE();
    const struct item flour = {.aisle = 1, .shelf = 1, .price_cents = 125000, .stock = 2};
    TEST_ASSERT_EQUAL_INT32_MESSAGE(125000, item_price(&flour), "item_price(&flour)");
}

// TASK: 2
void test_restock_soap(void) {
    TEST_IGNORE();
    struct item soap = {.aisle = 4, .shelf = 2, .price_cents = 275, .stock = 6};
    restock(&soap, 18);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(24, soap.stock, "restock(&soap, 18) with soap.stock = 6");
}

void test_restock_an_empty_shelf(void) {
    TEST_IGNORE();
    struct item maize = {.aisle = 2, .shelf = 4, .price_cents = 320, .stock = 0};
    restock(&maize, 36);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(36, maize.stock, "restock(&maize, 36) with maize.stock = 0");
}

void test_take_items_off_the_shelf(void) {
    TEST_IGNORE();
    struct item fish = {.aisle = 3, .shelf = 1, .price_cents = 560, .stock = 15};
    restock(&fish, -4);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(11, fish.stock, "restock(&fish, -4) with fish.stock = 15");
}

void test_restock_leaves_the_other_fields_alone(void) {
    TEST_IGNORE();
    struct item popcorn = {.aisle = 2, .shelf = 1, .price_cents = 450, .stock = 20};
    restock(&popcorn, 10);
    TEST_ASSERT_EQUAL_INT16_MESSAGE(2, popcorn.aisle, "popcorn.aisle after restock(&popcorn, 10)");
    TEST_ASSERT_EQUAL_INT16_MESSAGE(1, popcorn.shelf, "popcorn.shelf after restock(&popcorn, 10)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(450, popcorn.price_cents, "popcorn.price_cents after restock(&popcorn, 10)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(30, popcorn.stock, "popcorn.stock after restock(&popcorn, 10)");
}

void test_restock_only_the_item_pointed_to(void) {
    TEST_IGNORE();
    struct item items[] = {
        {.aisle = 2, .shelf = 1, .price_cents = 450, .stock = 20},
        {.aisle = 4, .shelf = 2, .price_cents = 275, .stock = 6},
        {.aisle = 2, .shelf = 3, .price_cents = 390, .stock = 12},
    };
    restock(&items[1], 5);
    TEST_ASSERT_EQUAL_INT32_MESSAGE(20, items[0].stock, "items[0].stock after restock(&items[1], 5)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(11, items[1].stock, "items[1].stock after restock(&items[1], 5)");
    TEST_ASSERT_EQUAL_INT16_MESSAGE(2, items[2].aisle, "items[2].aisle after restock(&items[1], 5)");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(12, items[2].stock, "items[2].stock after restock(&items[1], 5)");
}

// TASK: 3
void test_value_of_two_items(void) {
    TEST_IGNORE();
    const struct item items[] = {
        {.aisle = 2, .shelf = 1, .price_cents = 450, .stock = 20},
        {.aisle = 4, .shelf = 2, .price_cents = 275, .stock = 6},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(10650, stock_value(items, 2), "stock_value(items, 2)");
}

void test_value_of_no_items(void) {
    TEST_IGNORE();
    const struct item items[] = {
        {.aisle = 2, .shelf = 1, .price_cents = 450, .stock = 20},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, stock_value(items, 0), "stock_value(items, 0)");
}

void test_value_of_one_item(void) {
    TEST_IGNORE();
    const struct item items[] = {
        {.aisle = 5, .shelf = 3, .price_cents = 1299, .stock = 4},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(5196, stock_value(items, 1), "stock_value(items, 1)");
}

void test_value_with_an_empty_shelf(void) {
    TEST_IGNORE();
    const struct item items[] = {
        {.aisle = 1, .shelf = 2, .price_cents = 610, .stock = 10},
        {.aisle = 3, .shelf = 2, .price_cents = 899, .stock = 0},
        {.aisle = 2, .shelf = 4, .price_cents = 320, .stock = 36},
        {.aisle = 3, .shelf = 1, .price_cents = 560, .stock = 15},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(26020, stock_value(items, 4), "stock_value(items, 4)");
}

void test_value_of_only_the_items_counted(void) {
    TEST_IGNORE();
    const struct item items[] = {
        {.aisle = 1, .shelf = 1, .price_cents = 200, .stock = 3},
        {.aisle = 1, .shelf = 2, .price_cents = 150, .stock = 2},
        {.aisle = 1, .shelf = 3, .price_cents = 999, .stock = 99},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(900, stock_value(items, 2), "stock_value(items, 2)");
}

// TASK: 4
void test_two_items_in_aisle_2(void) {
    TEST_IGNORE();
    const struct item items[] = {
        {.aisle = 2, .shelf = 1, .price_cents = 450, .stock = 20},
        {.aisle = 4, .shelf = 2, .price_cents = 275, .stock = 6},
        {.aisle = 2, .shelf = 3, .price_cents = 390, .stock = 12},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(2, items_in_aisle(items, 3, 2), "items_in_aisle(items, 3, 2)");
}

void test_no_items_in_an_empty_aisle(void) {
    TEST_IGNORE();
    const struct item items[] = {
        {.aisle = 2, .shelf = 1, .price_cents = 450, .stock = 20},
        {.aisle = 4, .shelf = 2, .price_cents = 275, .stock = 6},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, items_in_aisle(items, 2, 3), "items_in_aisle(items, 2, 3)");
}

void test_no_items_in_an_empty_store(void) {
    TEST_IGNORE();
    const struct item items[] = {
        {.aisle = 1, .shelf = 1, .price_cents = 450, .stock = 20},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, items_in_aisle(items, 0, 1), "items_in_aisle(items, 0, 1)");
}

void test_every_item_in_one_aisle(void) {
    TEST_IGNORE();
    const struct item items[] = {
        {.aisle = 5, .shelf = 1, .price_cents = 1299, .stock = 4},
        {.aisle = 5, .shelf = 2, .price_cents = 350, .stock = 9},
        {.aisle = 5, .shelf = 3, .price_cents = 80, .stock = 50},
        {.aisle = 5, .shelf = 4, .price_cents = 2400, .stock = 1},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(4, items_in_aisle(items, 4, 5), "items_in_aisle(items, 4, 5)");
}

void test_items_in_aisle_ignores_the_shelf_number(void) {
    TEST_IGNORE();
    const struct item items[] = {
        {.aisle = 1, .shelf = 1, .price_cents = 610, .stock = 10}, {.aisle = 3, .shelf = 1, .price_cents = 899, .stock = 0},
        {.aisle = 1, .shelf = 3, .price_cents = 320, .stock = 36}, {.aisle = 2, .shelf = 1, .price_cents = 560, .stock = 15},
        {.aisle = 1, .shelf = 2, .price_cents = 140, .stock = 7},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, items_in_aisle(items, 5, 1), "items_in_aisle(items, 5, 1)");
}

// TASK: 5
void test_heaviest_of_three_crates(void) {
    TEST_IGNORE();
    const struct crate crates[] = {
        {.items = 24, .weight_grams = 9600, .aisle = 2},
        {.items = 48, .weight_grams = 18200, .aisle = 1},
        {.items = 12, .weight_grams = 5400, .aisle = 4},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, heaviest_crate(crates, 3), "heaviest_crate(crates, 3)");
}

void test_heaviest_of_one_crate(void) {
    TEST_IGNORE();
    const struct crate crates[] = {
        {.items = 30, .weight_grams = 15000, .aisle = 3},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, heaviest_crate(crates, 1), "heaviest_crate(crates, 1)");
}

void test_heaviest_crate_is_the_last(void) {
    TEST_IGNORE();
    const struct crate crates[] = {
        {.items = 100, .weight_grams = 4000, .aisle = 5},
        {.items = 90, .weight_grams = 6500, .aisle = 5},
        {.items = 80, .weight_grams = 7200, .aisle = 3},
        {.items = 6, .weight_grams = 21000, .aisle = 1},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(3, heaviest_crate(crates, 4), "heaviest_crate(crates, 4)");
}

void test_heaviest_crate_is_not_the_one_with_most_items(void) {
    TEST_IGNORE();
    const struct crate crates[] = {
        {.items = 200, .weight_grams = 3000, .aisle = 4},
        {.items = 10, .weight_grams = 25000, .aisle = 1},
        {.items = 150, .weight_grams = 4500, .aisle = 4},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, heaviest_crate(crates, 3), "heaviest_crate(crates, 3)");
}

void test_heaviest_crate_tie_goes_to_the_first(void) {
    TEST_IGNORE();
    const struct crate crates[] = {
        {.items = 12, .weight_grams = 8000, .aisle = 2},
        {.items = 24, .weight_grams = 16000, .aisle = 2},
        {.items = 30, .weight_grams = 9000, .aisle = 3},
        {.items = 24, .weight_grams = 16000, .aisle = 1},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(1, heaviest_crate(crates, 4), "heaviest_crate(crates, 4)");
}

// TASK: 6
void test_items_to_shelve_in_aisle_2(void) {
    TEST_IGNORE();
    const struct crate crates[] = {
        {.items = 24, .weight_grams = 9600, .aisle = 2},
        {.items = 48, .weight_grams = 18200, .aisle = 1},
        {.items = 12, .weight_grams = 5400, .aisle = 2},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(36, items_to_shelve(crates, 3, 2), "items_to_shelve(crates, 3, 2)");
}

void test_nothing_to_shelve_with_no_crates(void) {
    TEST_IGNORE();
    const struct crate crates[] = {
        {.items = 24, .weight_grams = 9600, .aisle = 2},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, items_to_shelve(crates, 0, 2), "items_to_shelve(crates, 0, 2)");
}

void test_nothing_to_shelve_in_an_aisle_with_no_crates(void) {
    TEST_IGNORE();
    const struct crate crates[] = {
        {.items = 24, .weight_grams = 9600, .aisle = 2},
        {.items = 48, .weight_grams = 18200, .aisle = 1},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(0, items_to_shelve(crates, 2, 4), "items_to_shelve(crates, 2, 4)");
}

void test_items_to_shelve_from_many_crates(void) {
    TEST_IGNORE();
    const struct crate crates[] = {
        {.items = 6, .weight_grams = 21000, .aisle = 1}, {.items = 100, .weight_grams = 4000, .aisle = 5},
        {.items = 90, .weight_grams = 6500, .aisle = 5}, {.items = 80, .weight_grams = 7200, .aisle = 3},
        {.items = 25, .weight_grams = 2000, .aisle = 5},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(215, items_to_shelve(crates, 5, 5), "items_to_shelve(crates, 5, 5)");
}

void test_items_to_shelve_from_the_last_crate_only(void) {
    TEST_IGNORE();
    const struct crate crates[] = {
        {.items = 30, .weight_grams = 9000, .aisle = 1},
        {.items = 40, .weight_grams = 12000, .aisle = 2},
        {.items = 50, .weight_grams = 15000, .aisle = 4},
        {.items = 60, .weight_grams = 18000, .aisle = 3},
    };
    TEST_ASSERT_EQUAL_INT32_MESSAGE(60, items_to_shelve(crates, 4, 3), "items_to_shelve(crates, 4, 3)");
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_price_of_popcorn_kernels);
    RUN_TEST(test_price_of_fishing_line);
    RUN_TEST(test_price_of_an_item_out_of_stock);
    RUN_TEST(test_price_of_a_large_sack);
    RUN_TEST(test_restock_soap);
    RUN_TEST(test_restock_an_empty_shelf);
    RUN_TEST(test_take_items_off_the_shelf);
    RUN_TEST(test_restock_leaves_the_other_fields_alone);
    RUN_TEST(test_restock_only_the_item_pointed_to);
    RUN_TEST(test_value_of_two_items);
    RUN_TEST(test_value_of_no_items);
    RUN_TEST(test_value_of_one_item);
    RUN_TEST(test_value_with_an_empty_shelf);
    RUN_TEST(test_value_of_only_the_items_counted);
    RUN_TEST(test_two_items_in_aisle_2);
    RUN_TEST(test_no_items_in_an_empty_aisle);
    RUN_TEST(test_no_items_in_an_empty_store);
    RUN_TEST(test_every_item_in_one_aisle);
    RUN_TEST(test_items_in_aisle_ignores_the_shelf_number);
    RUN_TEST(test_heaviest_of_three_crates);
    RUN_TEST(test_heaviest_of_one_crate);
    RUN_TEST(test_heaviest_crate_is_the_last);
    RUN_TEST(test_heaviest_crate_is_not_the_one_with_most_items);
    RUN_TEST(test_heaviest_crate_tie_goes_to_the_first);
    RUN_TEST(test_items_to_shelve_in_aisle_2);
    RUN_TEST(test_nothing_to_shelve_with_no_crates);
    RUN_TEST(test_nothing_to_shelve_in_an_aisle_with_no_crates);
    RUN_TEST(test_items_to_shelve_from_many_crates);
    RUN_TEST(test_items_to_shelve_from_the_last_crate_only);
    return UNITY_END();
}
