# Design

## Learning objectives

- Know that a `uint32_t` can hold a set of numbers from 0 to 31, with bit `i` set exactly when `i` is a member.
- Make a single-bit mask with `li t0, 1` and `sll t0, t0, a1`.
- Add a member with `or`, remove one with `not` and `and`, and toggle one with `xor`.
- Test membership with `srl` then `andi 1`, or with `and` then `snez`, giving a 0 or 1 boolean.
- Combine sets: union with `or`, intersection with `and`, and difference with `not` and `and`.
- Count members by repeatedly clearing the lowest set bit with `x & (x - 1)`.
- Find the smallest member with a shifting loop, handling the empty set separately.
- Build a set of letters from a string, mapping `'a'` to `'z'` onto bits 0 to 25 with the one-compare range check.
- Know why bit 31 needs tests that ignore the sign (`snez`, `beqz`, `bnez`) rather than signed ones (`sgtz`, `bltz`).
- Relate bit sets to Python's `set` operations and `int` bit tricks.

## Out of scope

- The Zbb extension: `cpop`, `ctz`, `clz` and `andn` (the `bit-manipulation-extension` concept).
- Sets larger than 32 members, stored as arrays of words.
- Case folding: capital letters stand for no boat.
- Calling other functions, and the stack.
- Vector instructions.
- Floating point, which this track does not cover.

## Concepts

- `bit-sets`

## Prerequisites

- `shifts`: `sll` and `srl` with a register shift amount, and logical versus arithmetic right shifts.
- `strings`: walking a zero-terminated string with `lbu`, character literals, and the one-compare range check for letters.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: `has_passed` using a signed test such as `sgtz` or `bltz` on the masked bit, which fails for boat 31.
- `essential`: `first_boat` looping without first checking for the empty log, which never ends.
- `essential`: `log_from_flags` using `lb` instead of `lbu` to load a character.
- `actionable`: `boats_seen` testing all 32 bits one at a time; suggest clearing the lowest set bit with `x & (x - 1)`.
- `actionable`: `missing` using `xor` instead of `and` with `not`; `xor` wrongly includes boats seen that did not go out.
- `actionable`: two signed comparisons for the letter check in `log_from_flags`; suggest the one-compare unsigned range check.
- `informative`: `log_from_flags` can load the 1 for the mask once, before the loop.
- `informative`: the Zbb extension, covered later, offers `cpop`, `ctz` and `andn` for tasks 3 to 5.
