# Design

## Learning objectives

- Know that a character is a byte holding its ASCII code, and that `'A'` is 65 and `'a'` is 97.
- Write character literals such as `'a'`, `' '` and `'a' - 'A'` as immediates.
- Know that this assembler reads `'\0'` as `'0'`, and write the zero byte as `zero` or `0` instead.
- Load characters with `lbu`, and know why `lb` gives the wrong value for bytes from 128 to 255.
- Store characters with `sb`, and walk a string pointer by 1 byte at a time.
- Know that a C string has no stored length and ends with a zero byte.
- Loop until the zero byte, testing at the top so that the empty string works.
- Find a length by subtracting the start address from a pointer to the terminator.
- Check whether a character is a letter with the one-compare unsigned range check.
- Convert between uppercase and lowercase by adding or subtracting `'a' - 'A'` (32).
- Change a string in place, and write a string into a caller-provided buffer, ending it with a zero byte.
- Relate a string walk to Python's `for ch in s`, `ord` and `chr`.

## Out of scope

- Strings in the data section, `.string`, `.byte` and `la` (the `data-section` concept).
- Converting numbers to and from decimal digits (the `number-formatting` concept).
- Halfwords: `lh`, `lhu`, `sh`.
- UTF-8 decoding: multi-byte characters are counted as bytes.
- Buffer sizes passed as arguments; the buffer is always big enough.
- Calling other functions, and the stack.
- Vector instructions.
- Floating point, which this track does not cover.

## Concepts

- `strings`

## Prerequisites

- `arrays`: walking a pointer through memory, and testing at the top so that an empty input touches nothing.
- `unsigned-integers`: `bltu`, `bgeu`, `sltiu`, and the one-compare range check.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: using `lb` instead of `lbu` to load a character; bytes from 128 to 255 become negative.
- `essential`: `copy_cue` not writing the zero terminator into `dest`.
- `essential`: a do-while loop that reads past the terminator of an empty string.
- `actionable`: writing `'\0'`, which this assembler reads as 48; use `zero` or `beqz`.
- `actionable`: numeric codes such as `65`, `97` or `32` instead of `'A'`, `'a'` or `'a' - 'A'`.
- `actionable`: two signed comparisons for a letter check; suggest the one-compare unsigned range check.
- `actionable`: in `whisper`, storing every byte back, rather than only the changed letters, is harmless but unnecessary.
- `informative`: `is_shouted` can return 0 as soon as it finds a lowercase letter.
- `informative`: `copy_cue` can store each byte before testing it, so that the terminator is copied by the loop.
