# Instructions

You have a holiday job on the quay at Nukuʻalofa, helping load the inter-island cargo ship _Moana Star_.
Crates of taro, tinned fish and school books are swung aboard by crane, and the tally clerk keeps track of them all with a notebook and a pencil.

The clerk would like a few small functions to check the sums.

## 1. Know the size of the hold

The _Moana Star_'s cargo hold has room for exactly 120 crates.

Implement the `hold_capacity` function, which returns the number of crates the hold can carry.

```c
hold_capacity();
// => 120
```

## 2. Order some spares

The voyage between the islands can be rough, and a crate is sometimes damaged on the way, so every order includes 3 spare crates.

Implement the `crates_with_spares` function, which takes the number of crates a shop has ordered and returns the number to send, including the spares.

```c
crates_with_spares(10);
// => 13
```

## 3. Work out the space left

Implement the `space_left` function, which takes the number of crates already loaded and returns how many more will fit in the hold.

```c
space_left(45);
// => 75
```

## 4. Keep a running count

At each island the ship calls at, some crates are unloaded and others are loaded.

Implement the `crates_on_board` function, which takes three arguments: the number of crates on board when the ship arrives, the number unloaded, and the number loaded.
It returns the number of crates on board when the ship leaves.

```c
crates_on_board(80, 30, 25);
// => 75
```

## 5. Trust the crane

The clerk writes a count in the notebook, and the crane has its own counter.
When the two disagree, the clerk uses the crane's count, because it is checked against the ship's manifest.

Implement the `checked_count` function, which takes the clerk's count and the crane's count, and returns the crane's count.

```c
checked_count(41, 42);
// => 42
```
