# Hints

## General

- `mul`, `div` and `rem` work on registers only.
  To multiply or divide by a fixed number, load it into a temporary register with `li` first.

## 1. Count the ladder points

- Work out the points for wins and the points for draws separately, then add them.
- You can keep the two numbers 4 and 2 in two different temporary registers.

## 2. Find the goals per quarter

- `div` divides one register by another, and throws away any fraction.

## 3. Find the average margin

- `div` already rounds towards zero, even for negative numbers.
  You do not need to do anything special for them.

## 4. Share the oranges

- The oranges left over are the remainder after dividing by the number of players.
- Think about what `rem` gives when the divisor is zero.

## 5. Book the buses

- Plain division rounds down for positive numbers, so 41 people with 20 seats per bus gives 2, which is one bus too few.
- Adding one less than the number of seats before dividing makes any part-full bus count as a whole one.
  For example, (41 + 19) / 20 is 3, but (40 + 19) / 20 is 2.
