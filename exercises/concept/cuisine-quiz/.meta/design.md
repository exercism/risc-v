# Design

## Learning objectives

- Turn a digit into its character by adding `'0'`, and a character into its digit by subtracting `'0'`.
- Get the decimal digits of a number, last digit first, with `remu` and `divu` by 10.
- Use a do-while loop so that 0 is written as `"0"`.
- Put the digits in order, either by reversing the buffer in place or by counting the digits first and writing backwards.
- Write a zero terminator and return the length of the text.
- Format a negative number as `'-'` and its magnitude, using `neg` and unsigned division so that -2147483648 works.
- Pad a number to a width with leading zeros, without cutting off digits.
- Parse decimal text with `n = n * 10 + (c - '0')`, stopping at the first non-digit with the one-compare range check.
- Parse an optional leading `'-'`.
- Relate formatting and parsing to Python's `str`, `int` and `f"{n:03}"`.

## Out of scope

- Other bases, such as hexadecimal (covered in `about.md` only).
- Padding with spaces (covered in the introduction and `about.md` only).
- Detecting overflow while parsing; the tests only use numbers that fit.
- Buffer sizes passed as arguments; the buffer is always big enough.
- Calling other functions and the stack, so each function formats its own digits.
- Strings in the data section and lookup tables of digit characters.
- Floating point, which this track does not cover.

## Concepts

- `number-formatting`

## Prerequisites

- `strings`: `lbu`, `sb`, character literals, the zero terminator, writing into a caller's buffer, and the one-compare range check for digits.
- `multiplication-and-division`: `mul`, and `divu`/`remu` alongside `div`/`rem`.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: using `div` or `rem` in `format_signed`; -2147483648 then gives wrong digits.
- `essential`: a loop with its test at the top in `format_score`, which writes nothing for 0.
- `essential`: not writing the zero terminator.
- `actionable`: the magic numbers 48 or 45 instead of `'0'` or `'-'`.
- `actionable`: `li t0, 10` inside the digit loop rather than before it.
- `informative`: in `format_padded`, writing exactly `length` digits from the end lets `remu` produce the padding zeros, with no separate padding loop.
- `informative`: `parse_signed` can turn the `'-'` check into 0 or 1 with `seqz` and add it to the pointer, instead of branching.
