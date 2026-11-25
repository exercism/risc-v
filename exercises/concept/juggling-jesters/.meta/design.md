# Design

## Learning objectives

- Know the words caller, callee, helper, return address and link register.
- Call a function with `call`, and know that `call` puts the return address in `ra`.
- Know that `ret` jumps to the address in `ra`, and that it is `jalr zero, 0(ra)`.
- Know that a function that calls another loses its own `ra`, and keep it in a temporary register the helper does not use.
- Pass arguments in `a0`, `a1`, … and read the result from `a0` after a call.
- Know that the calling convention lets a callee change `a0`–`a7` and `t0`–`t6`, and keep values needed after a call in registers the helper is known not to touch.
- Call a function inside a loop.
- Write a helper without `.global`.
- Call a leaf helper with a custom link register, `jal t5, helper`, and return from it with `jr t5`.
- Know that these are shortcuts for helpers you wrote yourself, and that the stack and saved registers are the standard way.

## Out of scope

- The stack, `sp`, and saving `ra` in memory: the next concept, `stack`.
- The saved registers `s0`–`s11`.
- Recursion.
- Calling functions written in C, or any function the student did not write.
- Memory, arrays and strings.
- Comparison instructions such as `slt`.
- Jump tables and calls through a register with `jalr` to a computed address.
- Floating point, which this track does not cover.

## Concepts

- `functions`

## Prerequisites

- `loops`

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: a function that uses `call` without keeping `ra` will loop for ever or crash; point to the section on keeping the return address.
- `essential`: `act_score`, `encore` and `show_total` should call `catch_count` rather than repeat the `mul`.
- `actionable`: a value needed after a `call` is kept in a register that the helper changes, such as `t0` or `a0`.
- `actionable`: the score-card helper in `crowd_rating` is marked `.global`; it is only used inside the file.
- `informative`: `show_total` could use the formula `rounds * acts * (acts + 1) / 2`, but the exercise is about calling a helper in a loop.
