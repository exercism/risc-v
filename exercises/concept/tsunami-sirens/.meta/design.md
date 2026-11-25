# Design

## Learning objectives

- Know that a branch compares two registers, jumps to a label when the comparison is true, and falls through otherwise.
- Use the branches `beq`, `bne`, `blt` and `bge`.
- Use the pseudo-instructions `bgt`, `ble`, `beqz`, `bnez`, `bltz`, `bgez`, `blez` and `bgtz`.
- Know that a branch cannot compare with an immediate, so a constant must be loaded with `li` first.
- Use `j` to jump without a condition.
- Use labels as branch targets, and name them as `.L` local labels.
- Write `if`, `if`-`else` and `if`-`elif`-`else` in assembly, branching on the opposite of the Python condition where needed.
- Write an `or` condition as two branches to the same label.

## Out of scope

- Comparison instructions such as `slt`, `sltu`, `seqz` and `snez`, which are covered by the `comparisons` concept.
- Unsigned branches `bltu` and `bgeu` (mentioned in `about.md` only).
- Loops and backward branches.
- Multiplication and division.
- Memory, the stack, and calling other functions.
- `.equ` constants and the data section.
- Numeric local labels such as `1:` with `1f` and `1b` (mentioned in `about.md` only).
- Floating point, which this track does not cover.

## Concepts

- `conditionals`

## Prerequisites

- `basics`

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: `should_evacuate` should return exactly 0 or 1, not the value of an argument.
- `actionable`: branch targets should be `.L` local labels rather than global-looking names such as `done:`.
- `informative`: `tide_change` can negate with `sub a0, zero, a0` (or `neg`) after a single `bgez`.
