# Design

## Learning objectives

- Know that a function returns only one value in `a0`, so further results go back through pointers.
- Know that an output parameter is a non-`const` pointer argument, and that the caller owns the memory it points to.
- Store results through output parameters with `sw`.
- Write a `void` function whose only results are output parameters.
- Write a function that returns a value in `a0` and also stores a result through a pointer, using the argument in `a0` before overwriting it.
- Use an in/out parameter: load the old value, store a new one, and return the old one.
- Load every input before storing any output.
- Relate output parameters to Python returning a tuple, or filling in a list the caller passes.

## Out of scope

- Returning 64-bit values or small structs in `a0` and `a1` (mentioned only).
- Aliasing between inputs and outputs (covered in `about.md` only; the tests never pass aliased pointers).
- Structs, arrays of variable length and loops.
- Branches, comparisons, multiplication, division, shifts and bitwise operations.
- The stack, and calling other functions.
- Floating point, which this track does not cover.

## Concepts

- `output-parameters`

## Prerequisites

- `memory`: pointers as addresses, `lw`, `sw`, word offsets, and `void` functions.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: in `desk_neighbours`, overwriting `a0` with the return value before working out the left desk gives a left desk of `desk`, not `desk - 1`.
- `essential`: `update_tally` must return the old tally, not the new one or the pointer.
- `actionable`: `unscramble` should load both numbers before storing either; storing the sum first and then subtracting the second number from it gives the first number back, not the difference.
- `informative`: `case_summary` can store the third clue straight from the register it was loaded into.
