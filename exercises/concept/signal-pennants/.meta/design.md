# Design

## Learning objectives

- Replace a chain of branches with a lookup table in `.rodata`, indexed by the input.
- Know the terms code-driven and data-driven, and compare them with Python's `if`/`elif` and a list or `dict`.
- Index a table by a character, by subtracting `'A'`.
- Range-check an index with one unsigned comparison before using it, and return a sentinel such as -1 when it fails.
- Choose the entry size: `.byte` with `lbu` for small numbers, `.word` with `lw` (and a shift by 2) for addresses.
- Build a table of string addresses with `.word label, ...`, and return an entry as a `const char *`.
- Write table entries as expressions, such as `1 + 2`, that the assembler works out.
- Use a biased base, `la t0, table - 'A'`, when the input is known to be valid.
- Use the value from one table as the index into another (a two-level lookup), here a 5-bit colour mask into a 32-entry count table.
- Use a table inside a loop, loading its address and bound once before the loop.

## Out of scope

- Halfword tables, `.half`, `lh` and `lhu` (the `halfwords` concept is not a prerequisite).
- 256-entry tables indexed directly by a byte, and offsets into one long string (covered in `about.md` only).
- Tables of `char *` passed in by a caller (the `pointer-arrays` concept).
- Jump tables of code addresses (the `jump-tables` concept).
- Bit-counting instructions such as `cpop` (the `bit-manipulation-extension` concept).
- `.equ` constants (the `constants` concept is not a prerequisite).
- Calling other functions, and the stack: `spell_length` repeats the lookup from `phonetic_length` inline.
- Vector instructions.
- Floating point, which this track does not cover.

## Concepts

- `lookup-tables`

## Prerequisites

- `data-section`: `.section .rodata`, `.byte`, `.word`, `.string`, `.balign` and `la`; through it, `strings` (`lbu`, character literals, walking to the zero byte), `unsigned-integers` (the one-compare range check), `arrays` (scaling an index with `slli`) and `bitwise-operations` (colour sets as bits).

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: any task written as a chain of branches on the letter instead of a table.
- `essential`: `phonetic_length` or `spell_length` indexing the table without a range check, so `'a'` or `'['` reads past the table.
- `actionable`: a range check written as two signed comparisons instead of one unsigned comparison.
- `actionable`: a `.word` table after strings or bytes with no `.balign 4` before it.
- `actionable`: `phonetic_word` copying the string with `sb` instead of returning the address from the table.
- `actionable`: `colour_count` counting bits with a loop instead of a second table.
- `informative`: `flag_colours` and `colour_count` can use `la t0, table - 'A'`, because the letter is always valid.
- `informative`: `spell_length` can load the table's address and the bound 26 once, before the loop.
