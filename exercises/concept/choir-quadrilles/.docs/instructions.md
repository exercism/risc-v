# Instructions

The choir at a secondary school in Nukuʻalofa, Tonga, is preparing a piece for the end-of-year concert that mixes singing with a quadrille.
A **quadrille** is an old dance for couples, first danced in France about 250 years ago.
The couples stand in a square, and the dance is made of short parts called figures, in which the couples cross the square, swap places with each other and turn, in set patterns.

For the concert, the singers stand in a line and sing a tune one note each: the first singer in the line sings the first note, the next singer sings the next note, and so on.
Every time the dancers move into a new figure, the singers move too, so the tune changes with the dance, while friends keep the beat on the school's lali, the wooden log drums.
The choir teacher numbers the notes, from 1 for the lowest upwards, and writes the line of singers as an array of `int32_t` note numbers, one for each singer.

Your job is to write the functions using vector instructions, so that each instruction moves many singers at once.

Every function in this exercise must work for any number of singers it is allowed, must not read any element past the last one it is given, and must not write past the last element it is asked to write.

## 1. Every other singer steps forward

In one figure, every other singer steps forward and sings alone, while the rest of the line dances.

Implement the `every_other` function, which takes the address of a destination array, the address of a tune and the number of notes in the tune.
It copies notes 0, 2, 4, … of the tune into the destination, and returns how many notes it copied.

```c
const int32_t tune[] = {1, 3, 5, 3, 1};
int32_t forward[3];
every_other(forward, tune, 5);
// => 3
// forward => {1, 5, 1}
```

## 2. Turn the line around

In another figure, the whole line turns around, so the last singer sings first and the tune is sung backwards.

Implement the `reverse_tune` function, which takes the address of a destination array, the address of a tune and the number of notes.
It writes the notes of the tune into the destination in reverse order.

```c
const int32_t tune[] = {1, 2, 3, 5, 8};
int32_t backwards[5];
reverse_tune(backwards, tune, 5);
// backwards => {8, 5, 3, 2, 1}
```

## 3. Partners swap places

The singers stand in couples: the first two singers are partners, then the next two, and so on.
In the zigzag figure, the partners in each couple swap places, just like the dancers do.

Implement the `zigzag` function, which takes the address of a destination array, the address of a tune and the number of notes.
The number of notes is always even.
It writes notes 1, 0, 3, 2, 5, 4, … of the tune into the destination.

```c
const int32_t tune[] = {1, 2, 3, 4};
int32_t swapped[4];
zigzag(swapped, tune, 4);
// swapped => {2, 1, 4, 3}
```

## 4. Keep the high voices

During one figure, only the singers with high notes keep singing, while everyone else dances.

Implement the `keep_high_notes` function, which takes the address of a destination array, the address of a tune, the number of notes and a threshold.
It copies every note that is higher than the threshold into the destination, keeping their order, and returns how many notes it copied.
It must not write anything else into the destination.

```c
const int32_t tune[] = {3, 11, 7, 14, 9};
int32_t high[5];
keep_high_notes(high, tune, 5, 8);
// => 3
// high => {11, 14, 9, ...}, the rest unchanged
```

## 5. Echo the previous note

In the last figure, a second line of singers faces the first line and echoes it, singing the same tune one note behind.
The second line starts with a note of its own.

Implement the `echo_previous` function, which takes the address of a destination array, the address of a tune, the number of notes and the first note of the echo.
The destination's first note is the given first note, and each of its other notes is the note before it in the tune.
So the destination has as many notes as the tune, and the tune's last note is not used.

```c
const int32_t tune[] = {3, 5, 7, 9, 11};
int32_t echo[5];
echo_previous(echo, tune, 5, 1);
// echo => {1, 3, 5, 7, 9}
```
