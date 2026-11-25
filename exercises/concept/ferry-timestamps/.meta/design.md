# Design

## Learning objectives

- Know that on RV32 a `uint64_t` or `int64_t` is kept in two registers: a low half and a high half.
- Know that a 64-bit argument takes two consecutive argument registers, low half first, and that a 32-bit argument takes the next free one, so `f(uint64_t x, uint32_t n)` receives `x` in `a0`/`a1` and `n` in `a2`.
- Know that a 64-bit result is returned in `a0` (low) and `a1` (high).
- Add 64-bit numbers, finding the carry with `sltu`.
- Subtract 64-bit numbers, finding the borrow with `sltu` before overwriting a low half.
- Compare 64-bit numbers: high halves first, then low halves, always unsigned for the low halves.
- Multiply two 32-bit numbers to a full 64-bit product with `mul` and `mulhu` (and know of `mulh` and `mulhsu`).
- Know that Python's integers never overflow, and that `% 2**64` matches `uint64_t`.

## Out of scope

- Shifts and bitwise instructions, so no building 64-bit shifts.
- Multiplying two 64-bit numbers, and 64-bit division (covered in `about.md` only).
- Signed `int64_t` arithmetic in the tasks (signed comparison is shown in the introduction).
- Memory, the stack, and calling other functions.
- Floating point, which this track does not cover.

## Concepts

- `64-bit-integers`

## Prerequisites

- `unsigned-integers`
- `multiplication-and-division`

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: a function returning `uint64_t` that never writes `a1`; the high half is whatever the caller left there.
- `essential`: `minutes_to_us` or `arrival_time` uses `mulh` instead of `mulhu`; it gives the wrong high half for minutes of `0x80000000` or more.
- `essential`: `departs_before` compares the low halves with `slt` or `blt`; the low half carries no sign.
- `actionable`: `add_delay` or `arrival_time` uses a branch to detect the carry; suggest `sltu` on the low sum.
- `actionable`: `journey_time` computes the borrow after overwriting a low half; the comparison then reads the wrong value.
