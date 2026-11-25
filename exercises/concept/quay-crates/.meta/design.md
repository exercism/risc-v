# Design

## Learning objectives

- Know that a RISC-V processor computes with 32 registers, each holding a 32-bit number.
- Know the roles of `a0`–`a7`, `t0`–`t6` and `zero`.
- Write a function with `.text`, `.global`, a label and `ret`.
- Know that arguments arrive in `a0`, `a1`, `a2`, … and that the result is returned in `a0`.
- Use `li`, `mv`, `add`, `sub` and `addi`, including `addi` with a negative immediate.
- Write comments.

## Out of scope

- Multiplication and division.
- Branches, comparisons and loops.
- Memory, the stack, and calling other functions.
- `.equ` constants and the data section.
- How pseudo-instructions expand (covered in `about.md` only).
- Floating point, which this track does not cover.

## Concepts

- `basics`

## Prerequisites

There are no prerequisites.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: `checked_count` should copy `a1` with `mv a0, a1` rather than, for example, `add a0, a1, zero`.
- `actionable`: `space_left` computing `a0 - 120` gives the wrong sign; suggest loading 120 into a temporary first.
