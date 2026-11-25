# Instructions

In the beach village of Lalomanu, on the island of Upolu in Samoa, a group of school friends runs a detective club.
They meet in an old boat shed near the sand, where they write up their cases and plan their next stake-out.

To get into the boat shed, you must knock the secret code on the door.
A code is 8 beats long, and on each beat you either knock or pause.
The club records each code as a number, one bit per beat: 1 means knock, and 0 means pause.
Beat 0 is bit 0, the right-hand bit.

For example, `0b00010011` means: knock on beats 0, 1 and 4, and pause on the others.

The club treasurer would like some functions to work with the codes.

## 1. Listen to the first four beats

The youngest members find a whole code hard to remember, so they practise with only its first four beats, beats 0 to 3.

Implement the `first_four` function, which takes a code and returns only its first four beats.
Every other beat becomes a pause.

```c
first_four(0b10110110);
// => 0b0110
```

## 2. Add some knocks

When a new member joins, the club adds a few knocks to the code.

Implement the `add_knocks` function, which takes a code and a pattern of extra knocks.
It returns the code with a knock on every beat that has a knock in either of them.

```c
add_knocks(0b00010011, 0b01000010);
// => 0b01010011
```

## 3. Flip some beats

Every Monday the club changes its code.
On the beats chosen for the week, a knock becomes a pause and a pause becomes a knock.

Implement the `flip_knocks` function, which takes a code and a pattern marking the chosen beats.
It returns the new code.

```c
flip_knocks(0b00010011, 0b00000110);
// => 0b00010101
```

## 4. Remove some knocks

When it rains, the boat shed's wooden door swells and some knocks cannot be heard.

Implement the `remove_knocks` function, which takes a code and a pattern marking the beats to remove.
It returns the code with a pause on each marked beat.
Every other beat stays as it was.

```c
remove_knocks(0b00010011, 0b00010010);
// => 0b00000001
```

## 5. Check a knock at the door

The member on guard listens to a knock at the door and compares it with the code.

Implement the `matching_knocks` function, which takes the 8 beats that were heard and the 8 beats of the code.
It returns an 8-beat pattern with a 1 on each beat where what was heard matches the code, whether that is a knock or a pause, and a 0 on each beat where they differ.
Beats 8 to 31 of the result must all be 0.

```c
matching_knocks(0b00010011, 0b00010110);
// => 0b11111010
```
