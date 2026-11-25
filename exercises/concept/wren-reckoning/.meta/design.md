# Design

## Learning objectives

- Know that a loop is a label plus a branch or `j` that goes backward to it.
- Write a while loop, with the test at the top, that may run zero times.
- Write a do-while loop, with the test at the bottom, that always runs at least once.
- Write a counting loop, like Python's `for i in range(n)`, with the counter in a temporary register.
- Use an accumulator for a sum, a count or a number built up digit by digit.
- Combine loops with `div`, `rem` and `mul` to work through the decimal digits of a number.
- Give loop labels unique names.
- Recognise and avoid infinite loops.

## Out of scope

- Comparison instructions such as `slt` and `sltu`.
- Unsigned branches such as `bltu` and `bgeu`.
- Memory, arrays and strings: these loops work on numbers in registers only.
- Shifts: doubling is done with `add`.
- Calling other functions, and the stack.
- Numeric local labels such as `1:` and `1b`, and `.L` labels (covered in `about.md` only).
- Overflow in loops (mentioned in `about.md` only).
- Floating point, which this track does not cover.

## Concepts

- `loops`

## Prerequisites

- `conditionals`
- `multiplication-and-division`

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `actionable`: loading 10 with `li` inside a loop body; suggest loading it once, before the loop.
- `informative`: `digit_count` suits a do-while loop, because 0 still has one digit; a while loop then needs a special case for 0.
- `informative`: `total_sightings` could use the formula `n * (n + 1) / 2`, but the exercise is about writing a loop.
