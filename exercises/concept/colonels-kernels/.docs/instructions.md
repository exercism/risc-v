# Instructions

Colonel Teina Marsters spent thirty years in the New Zealand Army, then came home to Atiu, in the Cook Islands, and took over the village store.
The store is famous across the island for its sacks of maize kernels and popcorn kernels, so everyone calls it "Colonel's Kernels".
It also sells almost everything else: rice, flour, tinned fish, coconut cream, soap, torch batteries and fishing line.
The goods sit on shelves along a few short aisles, and the supply ship brings new crates every few weeks.

The colonel runs the store like an army camp, and keeps a record for every item on an old computer.
You are helping them look after those records.
Each record is a C struct:

```c
struct item {
    int16_t aisle;       // the aisle number
    int16_t shelf;       // the shelf number in that aisle
    int32_t price_cents; // the price of one item, in cents
    int32_t stock;       // how many are on the shelf
};
```

When the ship comes in, each crate also gets a record:

```c
struct crate {
    int16_t items;        // how many items are packed in the crate
    int32_t weight_grams; // the weight of the crate, in grams
    int16_t aisle;        // the aisle the items go to
};
```

Before you start, work out the offset of every field, and the size of each struct.
Remember the padding.

Every pointer the tests pass you is aligned, and every array has the number of elements given by `count`.

## 1. Look up a price

Implement the `item_price` function.
It takes a pointer to one item, and returns its price in cents.

```c
struct item popcorn = {.aisle = 2, .shelf = 1, .price_cents = 450, .stock = 20};
item_price(&popcorn);
// => 450
```

## 2. Restock an item

Implement the `restock` function.
It takes a pointer to one item and the number of new items put on the shelf, and adds that number to the item's stock.
It changes the item in place, and returns nothing.
The number can be negative when items are taken off the shelf.

```c
struct item soap = {.aisle = 4, .shelf = 2, .price_cents = 275, .stock = 6};
restock(&soap, 18);
// soap.stock is now 24
```

## 3. Value the stock

The colonel wants to know how much all the goods on the shelves are worth.

Implement the `stock_value` function.
It takes an array of items and its length, and returns the sum of `price_cents * stock` over every item.
The array may be empty.

```c
struct item items[] = {
    {.aisle = 2, .shelf = 1, .price_cents = 450, .stock = 20}, // popcorn kernels
    {.aisle = 4, .shelf = 2, .price_cents = 275, .stock = 6},  // soap
};
stock_value(items, 2);
// => 10650
```

## 4. Count the items in an aisle

Implement the `items_in_aisle` function.
It takes an array of items, its length, and an aisle number, and returns how many of the items are kept in that aisle.

```c
struct item items[] = {
    {.aisle = 2, .shelf = 1, .price_cents = 450, .stock = 20}, // popcorn kernels
    {.aisle = 4, .shelf = 2, .price_cents = 275, .stock = 6},  // soap
    {.aisle = 2, .shelf = 3, .price_cents = 390, .stock = 12}, // maize kernels
};
items_in_aisle(items, 3, 2);
// => 2
```

## 5. Find the heaviest crate

The colonel's orders: the heaviest crate is unloaded first, with two people to lift it.

Implement the `heaviest_crate` function.
It takes an array of crates and its length, and returns the **index** of the heaviest crate.
There is always at least one crate.
If two crates share the greatest weight, return the index of the first one.

```c
struct crate crates[] = {
    {.items = 24, .weight_grams = 9600, .aisle = 2},
    {.items = 48, .weight_grams = 18200, .aisle = 1}, // bags of popcorn kernels
    {.items = 12, .weight_grams = 5400, .aisle = 4},
};
heaviest_crate(crates, 3);
// => 1
```

## 6. Count the items to shelve

Implement the `items_to_shelve` function.
It takes an array of crates, its length, and an aisle number.
It returns the total number of items, across all the crates for that aisle.

```c
struct crate crates[] = {
    {.items = 24, .weight_grams = 9600, .aisle = 2},
    {.items = 48, .weight_grams = 18200, .aisle = 1},
    {.items = 12, .weight_grams = 5400, .aisle = 2},
};
items_to_shelve(crates, 3, 2);
// => 36
```
