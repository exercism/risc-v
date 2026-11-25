# Hints

## General

- Work out the layout of each struct before you write any code, and write it in a comment at the top of your file.
- Give each field an offset that is a multiple of its alignment: 2 for `int16_t`, 4 for `int32_t`.
- Round each struct's size up to a multiple of its largest alignment.
- Use `lh` and `sh` for `int16_t` fields, and `lw` and `sw` for `int32_t` fields.
- In an array of structs, the step from one element to the next is the size of the struct, including its padding.

## 1. Look up a price

- `aisle` and `shelf` take 2 bytes each, so `price_cents` starts at offset 4.
- One `lw` from `a0` plus that offset is enough.

## 2. Restock an item

- `stock` starts straight after `price_cents`, at offset 8.
- Load the stock, add `a1`, and store it back to the same offset.
- Nothing needs to go in `a0` at the end.

## 3. Value the stock

- `struct item` takes 12 bytes, so the end pointer is `items + 12 * count`.
- 12 is not a power of 2, so use `li` and `mul` to work out `12 * count`.
- Move the pointer on by 12 after each item.
- Keep the running total in a temporary register, starting at 0.

## 4. Count the items in an aisle

- `aisle` is an `int16_t` at offset 0, so load it with `lh`.
- `lw` at offset 0 would read the shelf number as well, in the upper 16 bits.
- `bne` can skip the `addi` when the aisle does not match.

## 5. Find the heaviest crate

- `items` takes 2 bytes, but `weight_grams` needs an offset that is a multiple of 4.
  So there are 2 bytes of padding, and `weight_grams` is at offset 4.
- `aisle` is at offset 8, and the size is rounded up from 10 to 12.
- There is always at least one crate, so you can start with crate 0 as the heaviest so far, and walk from crate 1.
- Keep the index of the current crate in its own register, and copy it when you find a heavier crate.
- Use `ble` or `bgt`, so that a crate of equal weight does not replace the first one.

## 6. Count the items to shelve

- You need two fields of each crate: `aisle` at offset 8, and `items` at offset 0.
- The struct takes 12 bytes, not 10, because of the padding at the end.
