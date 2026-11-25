# Design

## Learning objectives

- Know that `sp` holds the address of the top of the stack, and that the stack grows downward.
- Make a stack frame with `addi sp, sp, -N`, and give it back with `addi sp, sp, N`.
- Save registers in the frame with `sw reg, offset(sp)` and restore them with `lw reg, offset(sp)`.
- Know that `sp` must stay a multiple of 16, and round frame sizes up to a multiple of 16.
- Write the prologue and epilogue of a non-leaf function, saving and restoring `ra`.
- Know the calling convention: `a0`–`a7` and `t0`–`t6` are caller-saved, `s0`–`s11`, `ra` and `sp` are callee-saved.
- Keep values that must survive a call, including loop state, in `s` registers.
- Know that following the convention lets any function call any other, including the student's own earlier functions, which call functions in turn.
- Restore everything before `ret`, and match every push with a pop.

## Out of scope

- Local variables and arrays in the stack frame: the `local-arrays` concept.
- Recursion: the `recursion` concept.
- The frame pointer `fp` (covered in `about.md` only).
- Spilling caller-saved registers to the frame around a call.
- Arguments passed on the stack (more than 8 arguments).
- Floating point, which this track does not cover.

## Checking the calling convention

The tests make the convention matter, rather than trusting the student:

- `trim_board`, written in C in the test file, writes junk into `t0`–`t6` and `a1`–`a7`.
  `hull_length` and `timber_needed` call it, and `fleet_length` and `fleet_cost` reach it through `hull_length`.
  A value kept in a `t` or `a` register across a call, or `ra` kept in `t6`, gives a wrong answer or a crash.
- Every test calls the student's function through `call_checked`, written in assembly in the test file.
  It loads known values into `s0`–`s11`, makes the call, and records in `clobbered_registers` any `s` register or `sp` that was not restored.
  Each test asserts that `clobbered_registers` is 0.

## Concepts

- `stack`

## Prerequisites

- `functions`
- `memory`

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: a function that uses `call` must save `ra` on the stack, not in a temporary register.
- `essential`: every `s` register a function writes must be saved in the prologue and restored in the epilogue.
- `essential`: the frame size must be a multiple of 16, and the epilogue must add back exactly what the prologue subtracted.
- `essential`: `hull_length`, `timber_needed`, `fleet_length` and `fleet_cost` should call the earlier functions rather than repeat their work.
- `actionable`: `plank_length` is a leaf function and needs no stack frame.
- `informative`: `fleet_length` could use a formula, but the exercise is about calling a function in a loop.
