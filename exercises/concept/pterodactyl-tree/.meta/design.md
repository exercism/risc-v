# Design

## Learning objectives

- Know that a recursive function is one that calls itself, and that in assembly this is an ordinary `call` to its own label.
- Write a base case that returns without a call, and a recursive case that calls itself on a smaller input.
- Know that each active call has its own stack frame, holding its own saved `ra` and `s` registers.
- Keep values that are needed after a recursive call in `s` registers.
- Know that recursion depth uses stack space, and that runaway recursion ends in a stack overflow, the cousin of Python's `RecursionError`.
- Turn a loop into recursion: the exit test becomes the base case.
- Write a recursive case that makes two calls, keeping the first answer safe across the second.
- Walk a tree stored in memory: load fields at fixed offsets, and treat `NULL` (0) as the empty tree.

## Out of scope

- Structs as a topic (field alignment, arrays of structs): the `structs` concept.
  Here each node is three words at offsets 0, 4 and 8.
- Indexing arrays with shifts.
- Memoisation, and turning tree recursion into loops.
- Tail calls (covered in `about.md` only).
- Arguments passed on the stack.
- Floating point, which this track does not cover.

## Checking the calling convention

The tests make the convention matter, rather than trusting the student:

- Every test calls the student's function through `call_checked`, written in assembly in the test file.
  It loads known values into `s0`–`s11`, makes the call, and records in `clobbered_registers` any `s` register or `sp` that was not restored.
  Each test asserts that `clobbered_registers` is 0.
  Since each function's caller is usually itself, a function that forgets to restore an `s` register usually gives a wrong answer as well.
- `larger`, written in C in the test file, writes junk into `t0`–`t6` and `a1`–`a7`.
  `largest_wingspan` calls it, so a value kept in a `t` or `a` register across that call gives a wrong answer.

## Concepts

- `recursion`

## Prerequisites

- `stack`

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: every function should call itself, rather than use a loop.
- `essential`: a function that uses `call` must save `ra` on the stack, and restore every `s` register it uses.
- `actionable`: the base case needs no stack frame; return before the prologue.
- `actionable`: `colony_size` needs only one recursive call, because both chicks' families are the same size.
- `informative`: `climbing_routes` makes very many calls for large inputs; a loop would be much faster.
