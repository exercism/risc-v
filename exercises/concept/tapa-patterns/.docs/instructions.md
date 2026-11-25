# Instructions

In a village on Savaiʻi, in Samoa, the Leota family makes _siapo_, the bark cloth of Samoa.
In Tonga the same kind of cloth is called _ngatu_, and across the Pacific it is often called _tapa_.
The cloth is made from the inner bark of the paper mulberry tree, beaten thin and joined into long sheets.
Then it is decorated with brown and black dyes.

Aunty Losa paints the designs.
Each design is made of small pictures, called **motifs**, in a row along the edge of the cloth: a star, a starfish, a pandanus leaf, a fishing net, and so on.
Her nephew Ioane has given each motif a number from 0 to 15, where 0 means plain cloth with no paint.
He writes each row of a design as a list of motif numbers, and would like some functions to help plan the patterns.

Every row has at most 64 motifs, and every motif number is between 0 and 15.

Aunty Losa knows how much dye each motif takes.
Her `dye_needed` function is already written for you, in C, in the test file:

```c
extern int32_t dye_needed(const int32_t *tally, int32_t kinds);
```

It takes a **tally**, an array of 16 words where `tally[m]` is how many times motif `m` appears, and `kinds`, the number of words in the tally, which must be 16.
It returns the number of spoonfuls of dye needed.
It returns -1 instead if `kinds` is not 16, or if `sp` is not a multiple of 16 when it is called.
Like any function you did not write, it may change every `a` and `t` register.

The tests check that each of your functions leaves `s0` to `s11` and `sp` exactly as it found them.
They also fill the stack with junk before each call, so a local array that you do not set to zero will be full of junk.

## 1. Count the different motifs

Implement the `count_distinct_motifs` function, which takes a row and the number of motifs in it, and returns how many different motifs the row uses.
Plain cloth, motif 0, counts as a motif too.

Use a local array of 16 words to keep track of which motifs you have seen.

```c
const int32_t row[] = {1, 2, 1, 3, 2, 1};
count_distinct_motifs(row, 6);
// => 3
```

## 2. Find the favourite motif

Implement the `most_common_motif` function, which takes a row and the number of motifs in it, and returns the motif that appears most often.
If several motifs appear equally often, return the smallest of them.
Every row passed to this function has at least one motif.

```c
const int32_t row[] = {4, 9, 4, 2, 9, 4};
most_common_motif(row, 6);
// => 4
```

## 3. Mix the dye

Implement the `dye_for_row` function, which takes a row and the number of motifs in it, and returns how many spoonfuls of dye the row needs.

Build a tally of the row in a local array of 16 words, and pass its address to `dye_needed`.

```c
const int32_t row[] = {1, 2, 1, 3, 2, 1};
dye_for_row(row, 6);
// => 13
```

## 4. Rotate the border

Around the edge of a cloth, a border pattern may start at any point.
To try a new starting point, Ioane rotates the row: every motif moves `k` places to the right, and the motifs that fall off the right-hand end come back in at the left.

Implement the `rotate_row` function, which takes a row, the number of motifs `n`, and the number of places `k`, and rotates the row in place.
`k` is never negative, but it may be bigger than `n`.
It returns nothing.

Copy the motifs into a local array of `n` words, at their new places, and then copy them back.
The size of this array depends on `n`, so it is known only at run time.

```c
int32_t row[] = {1, 2, 3, 4, 5};
rotate_row(row, 5, 2);
// row is now {4, 5, 1, 2, 3}
```

## 5. Smooth the pattern

A single motif between two copies of another motif often looks like a mistake.
So Aunty Losa smooths a row: wherever the two neighbours of a motif are the same motif as each other, that motif is changed to match them.
The first and last motifs of the row have only one neighbour, so they never change.

The neighbours that count are always the ones in the row as it was **before** smoothing began.

Implement the `smooth_row` function, which takes a row and the number of motifs in it, and smooths the row in place.
It returns nothing.
Keep a copy of the original row in a local array of `n` words.

```c
int32_t row[] = {2, 9, 2, 9, 2, 9, 2, 9};
smooth_row(row, 8);
// row is now {2, 2, 9, 2, 9, 2, 9, 9}
```
