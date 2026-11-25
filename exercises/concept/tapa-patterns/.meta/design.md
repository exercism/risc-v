# Design

## Learning objectives

- Make room for a local array in the stack frame with `addi sp, sp, -N`, rounding `N` up to a multiple of 16.
- Place a local array at the bottom of the frame, with saved registers above it.
- Address the elements of a local array from `sp`, with fixed offsets and with an index in a register.
- Know that a new local array holds garbage, and set it to zero with a loop.
- Pass the address of a local array to another function.
- Know that a local array disappears when its function returns, so its address must never be returned.
- Make a local array whose size is known only at run time: compute the size, round it up with `addi t0, t0, 15` and `andi t0, t0, -16`, and `sub` it from `sp`.
- Use `s0` as a frame pointer to restore `sp` after a variable-size array.
- Know that a local array that is too big causes a stack overflow.

## Out of scope

- Local scalar variables spilled to the stack (the same idea, but not practised here).
- Returning or keeping a pointer to a local array: taught only as a mistake to avoid.
- Arrays of bytes or halfwords on the stack: the `strings` and `halfwords` concepts.
- Arrays in the data section: the `data-section` concept.
- Recursion: the `recursion` concept.
- Floating point, which this track does not cover.

## Checking the calling convention

The tests make the rules matter, rather than trusting the student:

- Every test calls the student's function through `call_checked`, copied from `canoe-carpentry`.
  It loads known values into `s0`–`s11`, makes the call, and records in `clobbered_registers` any `s` register or `sp` that was not restored.
  Each test asserts that `clobbered_registers` is 0.
  This catches a variable-size array whose `sp` is not put back, and a frame pointer `s0` that is not saved.
- Before each call, `dirty_stack` fills the stack below the test with junk, so a tally that is not set to zero gives a wrong answer.
- `dye_needed`, written in C in the test file, writes junk into `t0`–`t6` and `a1`–`a7`, returns -1 unless it is passed 16, and returns -1 if `sp` is not a multiple of 16 when it is called.
  `dye_for_row` calls it with the address of its local tally.
- The `rotate_row` and `smooth_row` tests put a sentinel after the row, which must not change.
- `smooth_row` has a test where smoothing in place, without a copy, gives a different answer.

## Concepts

- `local-arrays`

## Prerequisites

- `stack`
- `arrays`

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: the tally in tasks 1–3 must live in the stack frame, not in the caller's row or below `sp`.
- `essential`: the size subtracted from `sp` in `rotate_row` and `smooth_row` must be rounded up to a multiple of 16.
- `essential`: after a variable-size array, `sp` should be restored from a saved register such as `s0`, and `s0` must be saved first.
- `actionable`: tasks 1–3 could share a tally-building helper that takes the address of the tally.
- `informative`: `rotate_row` can be done in place with three reversals, but the exercise is about a temporary buffer.
