# Design

## Learning objectives

- Know that a halfword is 16 bits, which is 2 bytes, and the ranges of `int16_t` (-32768 to 32767) and `uint16_t` (0 to 65535).
- Know that element `i` of a halfword array is at address `base + 2 * i`, and index it with `slli` by 1.
- Walk a pointer through a halfword array in steps of 2, stopping at `base + 2 * count`.
- Load `int16_t` values with `lh`, which sign-extends, and `uint16_t` values with `lhu`, which zero-extends.
- Know that the wrong load gives wrong answers only for negative `int16_t` values, or `uint16_t` values from 32768 up.
- Store halfwords with `sh`, which writes only the low 16 bits and leaves the neighbouring elements alone.
- Know that a value too big for 16 bits is truncated by `sh`, with no error.
- Know that arithmetic after loading happens in 32 bits, so a sum kept in a register does not overflow at 16 bits.
- Know that halfwords are aligned to 2 bytes.

## Out of scope

- Bytes and strings: `lb`, `lbu`, `sb` (the `strings` concept).
- Unsigned branches such as `bltu` and `bgeu`, and comparison instructions such as `slt`.
  After `lhu` every value is between 0 and 65535, so signed branches are enough.
- Byte order (covered in `about.md` only).
- Zbb's `sext.h` and `zext.h`.
- Structs with halfword fields (the `structs` concept).
- Calling other functions, and the stack.
- Vector instructions.
- Floating point, which this track does not cover.

## Concepts

- `halfwords`

## Prerequisites

- `arrays`: element addresses, indexing, walking a pointer to an end pointer, empty arrays.
  Through it, `memory`, `loops`, `shifts`, `bitwise-operations`, `conditionals`, `multiplication-and-division` and `basics`.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: using `lw` or `sw` on a halfword array; `sw` in `calibrate` overwrites the next reading.
- `essential`: shifting the index left by 2 instead of 1, or moving the pointer on by 4 instead of 2.
- `essential`: using `lh` in `deepest`, which makes depths from 32768 up look negative.
- `essential`: using `lhu` for `int16_t` readings, which makes negative readings look large.
- `actionable`: sign-extending by hand with `slli` and `srai` after `lhu`; suggest `lh`.
- `informative`: `coolest` and `deepest` can start with the first element and walk from the second, because the array is never empty.
