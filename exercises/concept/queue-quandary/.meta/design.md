# Design

## Learning objectives

- Know that the same 32 bits can be read as a signed (two's complement) or an unsigned number, and that `0xFFFFFFFF` is -1 or 4294967295.
- Read hexadecimal literals such as `0x80000000` as a way of writing numbers.
- Know that a register does not know which reading is meant: the instruction decides.
- Know that `add`, `addi` and `sub` wrap around, giving the same bits for signed and unsigned numbers, unlike Python's integers.
- Use subtraction to measure the distance between two wrapping counters.
- Use `bltu`, `bgeu`, `bgtu`, `bleu`, `sltu`, `sltiu` and `sgtu` to compare unsigned numbers.
- Use the one-compare range check: `addi` with `-low`, then one unsigned comparison with `high - low + 1`.
- Know that `seqz` is `sltiu rd, rs, 1`.
- Use `divu` and `remu` for unsigned numbers of `0x80000000` or more.
- Know that C's `uint32_t` is an unsigned 32-bit number.

## Out of scope

- Bitwise instructions and shifts.
- Detecting overflow, carries and 64-bit arithmetic (overflow detection is mentioned in `about.md` only).
- `mulh`, `mulhu` and `mulhsu`.
- Memory, the stack, and calling other functions.
- Floating point, which this track does not cover.

## Concepts

- `unsigned-integers`

## Prerequisites

- `comparisons`
- `loops`

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: `served_first` uses `slt`, `is_valid_counter` uses `slti`, or `average_wait` or `seconds_part` use `div` or `rem`; the signed instructions fail for values of `0x80000000` or more.
- `actionable`: `is_valid_counter` uses two comparisons; suggest subtracting 1 and doing one unsigned comparison with 12.
- `actionable`: `next_ticket` or `tickets_waiting` has a special case for the wrap-around; suggest relying on the wrapping of `addi` and `sub`.
- `informative`: `average_wait` without a zero check returns `0xFFFFFFFF` for 0 customers, because `divu` by zero gives the biggest unsigned number.
