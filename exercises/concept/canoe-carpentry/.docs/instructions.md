# Instructions

On a beach in Haʻapai, in Tonga, the Fifita family has a workshop under the coconut palms.
They build outrigger canoes, called _popao_, for families on the nearby islands.
Each canoe has a long hull, and a smaller float, the outrigger, fixed to one side to keep it steady.

Grandfather Sione measures every board twice.
His granddaughter Mele keeps the numbers in a notebook, and she would like some functions to do the sums.
Each step of the work builds on the steps before it, so each function may call the functions of earlier tasks.

All lengths are in centimetres.

Before a board is used, Uncle Tevita squares off its ends on his saw, which makes it a little shorter.
The `trim_board` function is already written for you, in C, in the test file:

```c
extern int32_t trim_board(int32_t cm);
```

It takes the length of a board and returns its length after trimming.
Like any function you did not write, it may change every `a` and `t` register.
The tests also check that each of your functions leaves `s0` to `s11` and `sp` exactly as it found them.

## 1. Measure a plank

The family joins boards end to end to make a long plank.
All the boards in a plank are the same length.

Implement the `plank_length` function, which takes the number of boards and the length of one board, and returns the length of the plank.

```c
plank_length(4, 150);
// => 600
```

## 2. Measure a hull

A hull is one plank, with a carved piece at each end: one at the bow (the front) and one at the stern (the back).
Each carved piece adds 30 centimetres.

Implement the `hull_length` function, which takes the number of boards and the length of one board, and returns the length of the hull.
Use `trim_board` to trim the board length, and then `plank_length` to measure the plank of trimmed boards.

```c
hull_length(4, 150);
// => 640
```

## 3. Order the timber

A canoe needs a plank for the hull and a shorter plank for the outrigger float.
Both are made from boards of the same length.

Implement the `timber_needed` function, which takes the number of boards in the hull, the number of boards in the float, and the length of one board.
It returns the total length of the two planks of trimmed boards, without the carved pieces.
Call `trim_board` once, and then `plank_length` once for the hull and once for the float.

```c
timber_needed(4, 2, 150);
// => 870
```

## 4. Line up the fleet

Before the school holidays, several families order canoes at once.
The family builds them in a row: the first canoe has a given number of boards in its hull, and each canoe after it has one more board than the one before.
All the boards are the same length.

Implement the `fleet_length` function, which takes the number of canoes, the number of boards in the first hull, and the length of one board.
It returns the total length of all the hulls.
Call `hull_length` once for each canoe.

```c
fleet_length(3, 4, 150);
// => 2355
```

## 5. Price the fleet

The varnish for the hulls is sold by the metre of hull, and a metre is 100 centimetres.
The shop rounds the price down to a whole number of paʻanga.

Implement the `fleet_cost` function, which takes the number of canoes, the number of boards in the first hull, the length of one board, and the price in paʻanga per metre.
It returns the cost of varnishing every hull in the fleet.
Use `fleet_length` to measure the fleet.

```c
fleet_cost(3, 4, 150, 20);
// => 471
```
