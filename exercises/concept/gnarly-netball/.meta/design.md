# Design

## Learning objectives

- Use `mul` to multiply two registers.
- Use `div` and `rem` to find a quotient and a remainder.
- Know that `mul`, `div` and `rem` have no immediate forms, so a constant must be loaded with `li` first.
- Know that `div` truncates towards zero and that the sign of `rem` follows the dividend, unlike Python's `//` and `%`.
- Know that division by zero does not stop the program: `div` gives -1 and `rem` gives the dividend.
- Know that `mul` overflow wraps around.
- Round a quotient up with `(n + d - 1) / d`, without branches.

## Out of scope

- `divu` and `remu`, and unsigned numbers in general (mentioned only).
- `mulh`, `mulhu` and `mulhsu` (covered in `about.md` only).
- Replacing division by a constant with multiplication and shifts.
- Branches, comparisons and loops.
- Memory, the stack, and calling other functions.
- Floating point, which this track does not cover.

## Concepts

- `multiplication-and-division`

## Prerequisites

- `basics`

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `informative`: `ladder_points` doubling with repeated `add` works, but this exercise is practice for `mul`.
- `actionable`: `buses_needed` computing `div` first and then adding 1 gives the wrong answer when the people fill the buses exactly; suggest adding `seats - 1` before dividing.
- `informative`: `oranges_left` needs no special case for zero players, because `rem` by zero gives the dividend.
