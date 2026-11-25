# Instructions

A small café near the market in Nukuʻalofa is famous for its cuisine: ʻota ʻika, raw fish in coconut cream, and lū pulu, corned beef baked in taro leaves.
Every Thursday evening the café holds a quiz night.
Teams of friends sit at numbered tables, share plates of food, and answer questions about sport, music, history and the islands.

Losaline, who runs the café, shows the scores on a big screen.
The screen can only show text, so every number must first be turned into a string of characters.
She would like some small functions to do this, and to read the numbers back from the teams' answer sheets.

In every task that writes text, the function receives a buffer `buf` that is always big enough.
It writes the characters, then a zero byte, and returns the number of characters it wrote, not counting the zero byte.

## 1. Show a score

Implement the `format_score` function, which takes a buffer and a team's score, and writes the score in decimal.

```c
char buf[16];
format_score(buf, 42);
// => 2, and buf is now "42"
```

A score of 0 is shown as `"0"`.

## 2. Show the penalty points

Teams lose points if they shout out an answer or look one up on a phone.
So a team's total can be negative.

Implement the `format_signed` function, which takes a buffer and a total that may be negative.
A negative total is written with a `'-'` in front of its digits.
The length that is returned includes the `'-'`.

```c
char buf[16];
format_signed(buf, -15);
// => 3, and buf is now "-15"
```

The function must work for every `int32_t`, including -2147483648.

## 3. Number the tables

Each table has a card with its number on it.
Losaline likes all the numbers to be the same width, so table 7 is shown as `"007"`.

Implement the `format_padded` function, which takes a buffer, a number and a width.
It writes the number with enough leading zeros to make it `width` characters long.
If the number already has `width` digits or more, no zeros are added, and no digits are cut off.

```c
char buf[16];
format_padded(buf, 7, 3);
// => 3, and buf is now "007"
format_padded(buf, 1234, 3);
// => 4, and buf is now "1234"
```

## 4. Read an answer sheet

At the end of each round, the quizmaster types each team's score from its answer sheet.
Sometimes a word comes after the number, as in `"120 points"`.

Implement the `parse_score` function, which takes a string and returns the number made by the digits at its start.
It stops at the first character that is not a digit.
If the string does not start with a digit, the result is 0.

```c
parse_score("120 points");
// => 120
```

## 5. Read a penalty

Penalties are typed with a `'-'` in front, such as `"-15"`.

Implement the `parse_signed` function, which takes a string that may start with a `'-'`, and returns the number it shows.
After the optional `'-'`, it reads digits as in task 4.

```c
parse_signed("-15");
// => -15
parse_signed("27");
// => 27
```

The function must work for every `int32_t`, including -2147483648.
