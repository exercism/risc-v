# Design

## Learning objectives

- Know that a shift moves every bit of a register left or right, discarding bits that fall off the end.
- Use `sll` and `slli` to multiply by a power of two.
- Use `srl` and `srli` to divide an unsigned value by a power of two, filling with zeros.
- Use `sra` and `srai` to divide a signed value by a power of two, copying the sign bit, and know that it rounds towards minus infinity rather than towards zero.
- Know when to use the register form and when to use the immediate form.
- Know that a register shift amount uses only its lowest 5 bits.
- Extract a bit field with a right shift followed by `andi`.
- Pack bit fields with left shifts followed by `or`.
- Relate the shifts to Python's `<<` and `>>`, and know that Python's `>>` is arithmetic.

## Out of scope

- Multiplication and division instructions.
- Branches, comparisons and loops.
- Sign-extending a field from the middle of a word (covered in `about.md` only).
- Bit-manipulation extension instructions such as rotates.
- Memory, the stack, and calling other functions.
- Floating point, which this track does not cover.

## Concepts

- `shifts`

## Prerequisites

- `bitwise-operations`: `and`, `andi`, `or`, masks, and hexadecimal and binary literals.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: `octave_down` should use `srl`, not `sra`; frequencies are unsigned.
- `essential`: `offset_after_turns` should use `sra`, not `srl`; offsets can be negative.
- `actionable`: `note_octave` and `note_duration` without an `andi` after the shift return the higher fields as well.
- `informative`: `note_octave` and `note_duration` could mask first and shift second, but shifting first keeps the mask small enough for `andi`.
