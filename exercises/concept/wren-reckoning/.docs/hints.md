# Hints

## General

- A loop is a label, some instructions, and a branch or `j` back to the label.
- Keep loop counters and running totals in temporary registers, `t0` to `t6`.
- Give each loop's labels names that start with the function's name, so no two labels in the file are the same.
- If the tests never finish, one of your loops probably never ends.
  Check that something changes on every iteration, and that it moves towards the exit condition.

## 1. Add up the fuia

- This is like Python's `for day in range(1, days + 1): total += day`.
- Start a total at 0 and a day counter at 1.
- Test at the top of the loop, so that 0 days gives a total of 0.

## 2. Train more spotters

- Start with 1 spotter and 0 weeks.
- While there are fewer spotters than needed, double the spotters and add 1 to the weeks.
- Adding a register to itself doubles it.
- If the club needs only 1 spotter, the answer is 0 weeks, so the body may run zero times.

## 3. Size the tally boxes

- Dividing by 10 removes the last digit of a number.
- Count how many times you divide before the number reaches 0.
- Even 0 needs one digit, so a loop with the test at the bottom fits well.
- `div` needs 10 in a register. Load it once, before the loop.

## 4. Check a band number

- `rem` by 10 gives the last digit of a number.
- Add the last digit to a running total, then remove it with `div`.
- Stop when the number reaches 0.

## 5. Read the band the right way round

- Build the answer one digit at a time, in an accumulator that starts at 0.
- Each iteration, multiply the accumulator by 10, add the last digit of the number, then remove that digit from the number.
