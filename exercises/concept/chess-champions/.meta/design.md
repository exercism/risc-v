# Design

## Learning objectives

- Know that a magic number is a bare number whose meaning is not clear, and replace it with a named constant.
- Define constants with `.equ NAME, value` (and know that `.set` is a synonym, from `about.md`).
- Name constants in UPPER_SNAKE_CASE and define them before the code that uses them.
- Use constants as immediates in `li` and `addi`.
- Define constants from expressions of other constants, such as `BOARD_SIZE * BOARD_SIZE` and `BOARD_SIZE - 1`, and know that the assembler works these out.
- Write an expression such as `QUEEN_VALUE - PAWN_VALUE` directly in an immediate.
- Represent an enumeration of codes as a group of constants with a shared name prefix.
- Know that constants take no memory, have no address and are not registers.
- Keep assembly constants in step with C `#define` and `enum` values in another file.

## Out of scope

- Comparison instructions such as `slt` and `seqz`.
- Loops, multiplication and division instructions, bitwise operations and shifts.
- Memory, the data section and `la`.
- Sharing one header between C and a `.S` file through the C preprocessor (mentioned in `about.md` only).
- Exporting constants with `.global`.
- Floating point, which this track does not cover.

## Concepts

- `constants`

## Prerequisites

- `conditionals`

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: the solution should define constants with `.equ` or `.set` rather than writing bare numbers such as `9` or `64` in instructions.
- `actionable`: `promote_pawn` can be one `addi` with the immediate `QUEEN_VALUE - PAWN_VALUE`.
- `actionable`: `BOARD_SQUARES` and `LAST_INDEX` should be defined from `BOARD_SIZE`, not as separate literal numbers.
- `informative`: `match_result` should return the result-code constants, not half points.
