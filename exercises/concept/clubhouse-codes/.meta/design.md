# Design

## Learning objectives

- Read and write numbers in binary (`0b...`) and hexadecimal (`0x...`).
- Know that bits are numbered from 0 at the right.
- Use `and`/`andi` with a mask to keep some bits and clear the rest.
- Use `or`/`ori` to set bits, and `xor`/`xori` to flip bits.
- Use the `not` pseudo-instruction, and know that it is `xori` with -1.
- Clear bits by combining `not` and `and`.
- Know that the immediates of `andi`, `ori` and `xori` are 12 bits and sign-extended.
- Tell `not` apart from `neg`.

## Out of scope

- Shifts.
- Branches, comparisons and loops.
- Multiplication and division.
- Memory, the stack, and calling other functions.
- The Zbb extension (`andn`, `orn`, `xnor`, `cpop` and others).
- Floating point, which this track does not cover.

## Concepts

- `bitwise-operations`

## Prerequisites

- `basics`

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: `first_four` and `matching_knocks` should use `andi` with the mask, rather than loading it with `li` first.
- `actionable`: `remove_knocks` using `xor` with the marked beats is wrong when a marked beat is already a pause; suggest `not` then `and`.
- `informative`: `matching_knocks` can apply `not` after the `xor`, then mask with `0xFF`.
