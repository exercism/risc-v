# Design

## Learning objectives

- Know the sections `.text`, `.section .rodata`, `.data` and `.bss`, and what belongs in each.
- Switch between sections, and return to `.text` before the code.
- Put a label on data, and know that it stands for the address of the data.
- Write strings with `.string` (or `.asciz`), and know that `.ascii` adds no zero terminator.
- Write tables with `.byte` and `.word`, and know of `.half`, `.zero` and `.space`.
- Write `.balign 4` before words that follow bytes.
- Load the address of a label with `la`, and know that it is a pseudo-instruction for `auipc` and `addi`.
- Return a `const char *` by returning the address of a string in `.rodata`.
- Read element `i` of a table in `.rodata`, with `lbu` for bytes and `lw` for words.
- Keep a variable in `.data` that lasts between calls, like a Python module-level variable.
- Know that a store into `.rodata` crashes the program with a segmentation fault.
- Know that `.global` can export a data label as well as a function.

## Out of scope

- Lookup-table design: tables indexed by characters, sentinel entries, range checks before indexing (the `lookup-tables` concept).
- Tables of addresses, such as `.word` tables of string labels (the `pointer-arrays` concept).
- Halfword loads `lh` and `lhu` (the `halfwords` concept); `.half` is only mentioned.
- `.equ` constants (the `constants` concept).
- Loading and storing straight through a label (`lw t0, label`), `%hi`/`%lo` and `%pcrel_hi`/`%pcrel_lo` (covered in `about.md` only).
- Exporting data to C with `.global`; the tests reach data only through functions.
- Calling other functions, and the stack.
- Vector instructions.
- Floating point, which this track does not cover.

## Concepts

- `data-section`

## Prerequisites

- `strings`: bytes, `lbu`, the zero terminator and `const char *`; through it, `arrays` (indexing with a shift), `memory` (`lw`/`sw`, alignment) and `conditionals` (the if-chain in `thank_you`).

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: the visitor count in `.section .rodata`; the store crashes the program.
- `essential`: building a string with `sb` into memory instead of keeping it in `.rodata` and returning its address.
- `essential`: `.ascii` without a following `.byte 0`, so the string has no terminator.
- `actionable`: a `.word` table after bytes with no `.balign 4` before it.
- `actionable`: `days_in_month` or `day_of_year` written as a chain of twelve branches instead of a table.
- `actionable`: forgetting to switch back to `.text` before the functions.
- `informative`: `days_in_month` can use offset `-1`, and `day_of_year` offset `-4`, instead of subtracting 1 from the month first.
- `informative`: the visitor count could equally be `.zero 4` in `.bss`.
