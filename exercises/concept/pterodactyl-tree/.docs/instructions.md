# Instructions

The science club at a high school in Lae, in Papua New Guinea, is getting ready for the school open day.
This year their display is about pterodactyls: flying reptiles that lived at the same time as the dinosaurs, millions of years ago.

The club has made up a colony of pterodactyls living on a sea cliff, and has built a paper model of each one.
Now they want to draw the colony's family tree on a big poster, and they need some functions to check their numbers.

Each function in this exercise calls itself.
The tests check that each of your functions leaves `s0` to `s11` and `sp` exactly as it found them.

## 1. Grow the colony

In the club's story, every pterodactyl raises exactly two chicks.
A family of 0 generations is just one pterodactyl, the founder.
A family of more generations is the founder, plus the families of its two chicks, each with one generation fewer.

Implement the `colony_size` function, which takes the number of generations and returns the number of pterodactyls in the family.
Write it recursively: `colony_size` must call `colony_size`.

```c
colony_size(2);
// => 7
```

## 2. Check the leg bands

Each model wears a numbered band on its leg.
To catch copying mistakes, the club writes the total of the band's digits next to each number on the poster.

Implement the `band_total` function, which takes a band number, which is never negative, and returns the sum of its digits.
Write it recursively: the total is the last digit, plus the total of the other digits.

```c
band_total(4027);
// => 13
```

## 3. Count the climbing routes

Before their first flight, the chicks climb up the cliff, from ledge to ledge.
With each hop, a chick climbs either 1 ledge or 2 ledges.
The club wants to know how many different routes a chick could take to climb a number of ledges.
For example, there are 3 routes up 3 ledges: 1 + 1 + 1, 1 + 2, and 2 + 1.
There is just one route up 0 ledges: stay where you are.

Implement the `climbing_routes` function, which takes the number of ledges and returns the number of routes.

```c
climbing_routes(4);
// => 5
```

## 4. Count the family

The tests store the family tree in memory.
Each pterodactyl is a block of three words:

```c
struct pterodactyl {
    int32_t wingspan;                           // offset 0
    const struct pterodactyl *first_chick;      // offset 4
    const struct pterodactyl *second_chick;     // offset 8
};
```

The wingspan is the distance from one wing tip to the other, in centimetres.
Each chick is the address of another pterodactyl block, or `NULL` (0) if there is no chick.
A pterodactyl may have two chicks, one chick (either the first or the second), or none.

Implement the `count_family` function, which takes the address of a pterodactyl and returns the number of pterodactyls in its family: itself, its chicks, their chicks, and so on.
An empty family, `NULL`, has no pterodactyls.

```c
// Balus has chicks Kumul and Tarangau. Tarangau has one chick, Muruk.
count_family(&balus);
// => 4
```

## 5. Count the generations

The poster needs one row for each generation.

Implement the `count_generations` function, which takes the address of a pterodactyl and returns the number of generations in its family.
This is the number of pterodactyls in the longest line from the given pterodactyl down to one with no chicks.
An empty family has no generations.

```c
// Balus has chicks Kumul and Tarangau. Tarangau has one chick, Muruk.
count_generations(&balus);
// => 3
```

## 6. Find the widest wings

The poster is drawn to scale, so the club must know how wide to make the widest pair of wings.

Implement the `largest_wingspan` function, which takes the address of a pterodactyl and returns the largest wingspan in its family.
An empty family has a largest wingspan of 0.

The `larger` function is already written for you, in C, in the test file:

```c
extern int32_t larger(int32_t a, int32_t b);
```

It returns the larger of its two arguments.
Like any function you did not write, it may change every `a` and `t` register.
Use it to find the larger of two wingspans.

```c
// Balus (520) has chicks Kumul (480) and Tarangau (610). Tarangau has one chick, Muruk (450).
largest_wingspan(&balus);
// => 610
```
