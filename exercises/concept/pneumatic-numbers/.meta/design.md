# Design

## Learning objectives

- Know that code lives in memory, so every label in `.text` has an address, which `la` can load.
- Jump to an address held in a register with `jr`, and know that `ret` is `jalr zero, 0(ra)`.
- Know that `jalr rs` calls through a register, and why it needs a safe place for `ra`.
- Store code addresses in `.rodata` with `.word label`, to make a jump table.
- Build a switch statement: range-check with one `bgeu`, then `slli`, `add`, `lw` and `jr`.
- Use a jump table inside a loop, with each case ending in `j` back to the loop.
- Read the C type of a function pointer, `int32_t (*fn)(int32_t, int32_t)`.
- Tail-call a function pointer passed from C with `jr`, after moving the arguments into place.
- Return a function pointer from a table of function addresses.
- Relate jump tables and function pointers to a Python dictionary or list of functions.

## Out of scope

- Calling a function pointer with `jalr` and continuing afterwards: that needs the stack, which is not a prerequisite (covered in `about.md` only).
- The stack and the saved registers `s0`–`s11`.
- Sparse switch statements, binary search over cases, and control-flow integrity (covered in `about.md` only).
- Writable tables of function pointers in `.data`.
- Floating point, which this track does not cover.

## Concepts

- `jump-tables`

## Prerequisites

- `pointer-arrays`: a table of 4-byte addresses, read with `lw`.
- `functions`: `jal`, `jalr`, `ra`, leaf functions and tail calls.
- `data-section`: `.section .rodata`, `.word`, `.ascii`, `.balign` and `la`.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: a jump table used without a range check, or with a signed check (`bge`) that lets negative codes through.
- `essential`: `apply` moving `a1` into `a0` before saving `fn`, so that it jumps to `a` instead of `fn`.
- `essential`: `apply` using `jalr` without keeping `ra`, so that `fn` returns into `apply` and `apply`'s `ret` loops back to itself.
- `actionable`: `calculate` or `run_program` using a chain of `beq` instructions instead of a jump table.
- `actionable`: `calculate` dividing without checking for a zero divisor, which returns -1.
- `actionable`: a table of `.word` entries without a `.balign 4` after byte data.
- `informative`: `calculate` and `operation` can share one table of function addresses, with `calculate` tail-calling through it.
- `informative`: `run_program` can load the table's address and the range limit once, before the loop.
