# Design

## Learning objectives

- Know that a C array argument arrives as the address of element 0 plus a separate length.
- Know that element `i` of an `int32_t` array is at address `base + 4 * i`, and that the element size is 4.
- Index an array with a register index: `slli`, `add`, then `lw` or `sw`.
- Walk a pointer through an array with `addi p, p, 4`, stopping at an end pointer `base + 4 * count`.
- Prefer pointer walking for loops that visit every element in order.
- Test at the top of the loop so that an empty array (count 0) reads and writes nothing.
- Start from element 0 and walk from element 1 when the array is known to be non-empty.
- Change every element of an array in place with a load, change and store in a loop.
- Turn a pointer back into an index with `sub` and `srli`, or keep a separate index.
- Relate pointer walking to Python's `for x in scores:` and indexing to `scores[i]`.

## Out of scope

- Byte and halfword arrays: `lb`, `lbu`, `lh`, `lhu`, `sb`, `sh` (the `strings` and `halfwords` concepts).
- Comparison instructions such as `slt`, and unsigned branches such as `bltu` and `bgeu`, including for comparing addresses.
- Arrays of pointers and arrays of structs.
- Arrays in the data section, and arrays on the stack.
- Returning several results through pointers.
- Calling other functions, and the stack.
- Vector instructions.
- Floating point, which this track does not cover.

## Concepts

- `arrays`

## Prerequisites

- `memory`: `lw`, `sw`, offsets, word size and alignment.
- `loops`: while loops with the test at the top, accumulators and counters.
- `shifts`: `slli` to multiply an index by 4, and `srli` to divide a byte offset by 4.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: in `runs_by`, adding the index to the address without shifting it left by 2; offsets count bytes, not elements.
- `essential`: a do-while loop in `total_runs`, `count_ducks`, `add_bonus` or `first_century` reads an element of an empty array.
- `actionable`: using `mul` by 4 instead of `slli` by 2 to scale an index.
- `actionable`: loading 100 with `li` inside the loop of `first_century`; suggest loading it once, before the loop.
- `informative`: an indexed loop that recomputes `base + 4 * i` every iteration could walk a pointer instead.
- `informative`: `highest_score` can start with the first score and walk from the second, because the array is never empty.
