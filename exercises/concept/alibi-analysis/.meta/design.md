# Design

## Learning objectives

- Know that a boolean is held in a register as 1 for true and 0 for false.
- Use `slt` and `slti` to compare signed numbers.
- Know that `sltu` and `sltiu` compare as unsigned numbers, and that `seqz` and `snez` are built from them.
- Use the pseudo-instructions `seqz`, `snez`, `sltz`, `sgtz` and `sgt`.
- Test for equality by subtracting and then using `seqz`.
- Build `>=` and `<=` by flipping a "less than" or "greater than" with `xori ..., 1`.
- Combine 0/1 booleans with `and` and `or`.
- Return exactly 0 or 1, converting a count to a boolean with `snez` when needed.

## Out of scope

- Unsigned numbers and unsigned comparison in depth.
- Bitwise operations on values other than 0 and 1.
- Loops, multiplication and division.
- Memory, the stack, and calling other functions.
- Floating point, which this track does not cover.

## Concepts

- `comparisons`

## Prerequisites

- `conditionals`

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `actionable`: `is_cleared` using `or a0, a0, a1` directly returns the witness count rather than 1; suggest `snez` first.
- `actionable`: using `not` to flip a boolean gives -2 or -1; suggest `xori ..., 1`.
- `informative`: a solution that uses branches is correct, but a comparison instruction is shorter.
