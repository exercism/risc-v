# Instructions

The drama club at a secondary school in Apia, Samoa, is holding a masquerade ball in the school hall.
A **masquerade** is a dance where the guests wear masks over their eyes.
The money from tickets and donations will pay for costumes for the club's next play.

At the door, the club's laptop keeps one entry for each guest, in arrays of `int32_t`.
Each mask has a colour, stored as a number:

| Code | Mask colour   |
| ---- | ------------- |
| 0    | no mask       |
| 1    | red           |
| 2    | blue          |
| 3    | gold          |
| 4    | green         |
| 5    | silver        |

Your job is to write the functions using vector instructions and masks, so that each instruction works on many guests at once.

Every function in this exercise must work for any number of guests it is allowed, and must not read or change any element past the last one it is given.

## 1. Count the masks of one colour

The prize for the best costume goes to a guest in a gold mask this year, and the judges want to know how many there are.

Implement the `count_colour` function, which takes the address of an array of mask colours, the number of guests and a colour.
It returns how many guests wear a mask of that colour.

```c
const int32_t colours[] = {3, 1, 3, 0, 3};
count_colour(colours, 5, 3);
// => 3
```

## 2. Find the first guest without a mask

Every guest should wear a mask, and the club has spare masks at the door.

Implement the `first_unmasked` function, which takes the address of an array of mask colours and the number of guests.
It returns the index of the first guest with no mask (colour 0).
If every guest has a mask, it returns -1.

```c
const int32_t colours[] = {3, 1, 0, 2, 0};
first_unmasked(colours, 5);
// => 2
```

## 3. Cap the donations

Some guests give a donation in tala, the money of Samoa.
For the raffle, every donation above a cap counts only as the cap.

Implement the `cap_donations` function, which takes the address of an array of donations, the number of guests and the cap.
It changes every donation that is more than the cap to the cap, and leaves the others as they are.

```c
int32_t donations[] = {20, 150, 50, 300, 100};
cap_donations(donations, 5, 100);
// donations => {20, 100, 50, 100, 100}
```

## 4. Discount the juniors

Guests younger than an age limit pay less for their ticket.

Implement the `discount_juniors` function, which takes the address of an array of ticket prices, the address of an array of ages, the number of guests, the age limit and the discount.
It takes the discount off the price of every guest younger than the age limit.
The other prices stay as they are.

```c
int32_t prices[] = {30, 30, 30, 30};
const int32_t ages[] = {10, 16, 12, 45};
discount_juniors(prices, ages, 4, 13, 10);
// prices => {20, 30, 20, 30}
```

## 5. Add up the juniors' donations

The club wants to thank the younger guests for their donations.

Implement the `junior_donations` function, which takes the address of an array of donations, the address of an array of ages, the number of guests and the age limit.
It returns the total of the donations from guests younger than the age limit.
If there are none, it returns 0.

```c
const int32_t donations[] = {10, 20, 30, 40, 50};
const int32_t ages[] = {12, 40, 8, 17, 11};
junior_donations(donations, ages, 5, 13);
// => 90
```

## 6. Count the lucky-door winners

Each ticket has a long number, kept in an array of `uint32_t`.
Some numbers are bigger than 2147483647, too big for an `int32_t`.
At the end of the night, the club calls out a low number and a high number.
Every ticket from the low number to the high number, including both, wins a lucky-door prize.

Implement the `count_winners` function, which takes the address of an array of ticket numbers, the number of tickets, the low number and the high number.
It returns how many tickets win.

```c
const uint32_t tickets[] = {2500000000, 1000, 2147483647, 3000000001, 2000000000};
count_winners(tickets, 5, 2000000000, 3000000000);
// => 3
```
