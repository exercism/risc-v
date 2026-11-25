# Instructions

Every Friday in the school holidays, a small café near the harbour in Apia runs a "gnocchi night".
Gnocchi are little dumplings made from potato and flour, and the café serves them with a choice of sauces.
All the money raised goes to the school sailing club, to pay for new sails.

The kitchen makes gnocchi in batches and lays the portions out on trays.
The café's laptop keeps the numbers in arrays of `int32_t`, and the volunteers want them worked out quickly, before the doors open.
Your job is to write the functions using vector instructions, so that each instruction works on many trays at once.

Every function in this exercise must work for any number of elements, including 0, and must not change any memory past the last element it is given.

## 1. Fill the trays

At the start of the night, every tray gets the same number of portions.

Implement the `fill_trays` function, which takes the address of an array of trays, the number of trays, and the number of portions.
It sets every tray to that number of portions.

```c
int32_t trays[] = {0, 0, 0, 0, 0};
fill_trays(trays, 5, 12);
// => trays is now {12, 12, 12, 12, 12}
```

## 2. Add extra portions

When a fresh batch comes out of the kitchen, the same number of extra portions goes onto every tray.
Sometimes the number is negative, when the cook takes a portion from every tray to taste.

Implement the `add_extra` function, which takes the address of an array of trays, the number of trays, and the number of extra portions.
It adds the extra portions to every tray, in place.

```c
int32_t trays[] = {10, 12, 9, 11};
add_extra(trays, 4, 3);
// => trays is now {13, 15, 12, 14}
```

## 3. Scale the recipe

The recipe lists the grams of each ingredient needed for one batch.
On a busy night, the kitchen makes several batches.

Implement the `scale_recipe` function, which takes the address of an array of grams, the number of ingredients, and the number of batches.
It multiplies every amount by the number of batches, in place.

```c
int32_t grams[] = {1000, 300, 50, 20, 5};
scale_recipe(grams, 5, 4);
// => grams is now {4000, 1200, 200, 80, 20}
```

## 4. Add up the portions

Two cooks make gnocchi side by side.
Each keeps an array with the number of portions they made for each sauce.

Implement the `add_portions` function, which takes the address of an array for the totals, the addresses of the two cooks' arrays, and the number of sauces.
It sets each total to the sum of the two cooks' portions for that sauce.

```c
const int32_t first[] = {14, 9, 20, 6};
const int32_t second[] = {11, 15, 0, 8};
int32_t totals[4];
add_portions(totals, first, second, 4);
// => totals is now {25, 24, 20, 14}
```

## 5. Count the leftovers

At the end of the night, the volunteers want to know how many portions of each sauce were left over.

Implement the `leftovers` function, which takes the address of an array for the leftovers, the address of an array of portions made, the address of an array of portions eaten, and the number of sauces.
It sets each leftover to the number made minus the number eaten.

```c
const int32_t made[] = {40, 25, 30, 18, 12};
const int32_t eaten[] = {37, 25, 21, 2, 12};
int32_t left[5];
leftovers(left, made, eaten, 5);
// => left is now {3, 0, 9, 16, 0}
```

## 6. Find the room on the trays

Every tray can hold the same number of portions, called its capacity.

Implement the `room_on_trays` function, which takes the address of an array for the room left, the address of an array with the portions on each tray, the number of trays, and the capacity.
It sets each room left to the capacity minus the portions on that tray.

```c
const int32_t on_tray[] = {12, 3, 20, 0};
int32_t room[4];
room_on_trays(room, on_tray, 4, 20);
// => room is now {8, 17, 0, 20}
```
