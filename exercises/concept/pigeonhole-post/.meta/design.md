# Design

## Learning objectives

- Know that memory is a long row of bytes, each with a numbered address, and that an address is just a number in a register.
- Know that a C pointer argument arrives in `a0`–`a7` as an address.
- Know that an `int32_t` is a 4-byte word, so neighbouring values are 4 bytes apart, at offsets 0, 4, 8, 12, ….
- Use `lw rd, offset(rs1)` to load a word.
- Use `sw rs2, offset(rs1)` to store a word, and know that the value comes first.
- Know that the offset is a 12-bit signed immediate, and that word addresses should be aligned to a multiple of 4.
- Change a value in memory by loading it, changing it in a register, and storing it back.
- Store `zero` to clear a word.
- Swap two values in memory by loading both before storing either.
- Know that a `void` function returns nothing and does its work by changing memory.
- Relate pointers to Python passing a list to a function that changes it.

## Out of scope

- Indexing with a variable index, pointer arithmetic and loops over arrays (the `arrays` concept).
- Byte and halfword loads and stores: `lb`, `lbu`, `lh`, `lhu`, `sb`, `sh`.
- Byte order (covered in `about.md` only).
- The data section, `la` and global variables.
- Returning several results through pointers (the `output-parameters` concept).
- Branches, comparisons, multiplication and shifts.
- The stack, and calling other functions.
- Floating point, which this track does not cover.

## Concepts

- `memory`

## Prerequisites

- `basics`: registers, `li`, `mv`, `add`, `sub`, `addi`, `ret` and the calling convention.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: `letters_in_third` should use offset 8, not 2 or 3; offsets count bytes, not values.
- `essential`: in `deliver`, `sw a0, 0(t0)` has the operands the wrong way round; the value to store comes first, as in `sw t0, 0(a0)`.
- `actionable`: `move_letters` and `swap_pigeonholes` should load both values before storing either.
- `informative`: `move_letters` can clear the old count with `sw zero, 0(a0)` rather than loading 0 into a register first.
