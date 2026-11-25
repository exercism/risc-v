# Design

## Learning objectives

- Know that SIMD means one instruction working on many elements, and relate it to NumPy's whole-array operations.
- Enable the vector extension with `.option arch, +v`.
- Know that there are 32 vector registers, `v0`–`v31`, that VLEN is chosen by the hardware, and that code must not assume it.
- Use `vsetvli t0, n, e32, m1, ta, ma`, and know what each operand means and that it returns `vl`.
- Write a vector-length agnostic strip-mining loop: `vsetvli`, load, compute, store, advance each pointer by `vl * 4` with `slli`, subtract `vl` from the count, and repeat while the count is more than 0.
- Load and store with `vle32.v` and `vse32.v`.
- Use `vadd.vv`, `vadd.vx`, `vsub.vv`, `vmul.vx`, `vrsub.vx` and `vmv.v.x`, and know the `.vv`, `.vx` and `.vi` forms.
- Know that a vector store writes exactly `vl` elements, so nothing past the end of the array changes.

## Out of scope

- LMUL other than `m1`, fractional LMUL, and element widths other than `e32` (described in `about.md` only).
- Reductions such as `vredsum` and moving an element to a scalar register (the `vector-reductions` concept).
- Masks, comparisons and `vmerge` (the `vector-masks` concept).
- Fault-only-first loads and byte vectors (the `vector-strings` concept).
- Slides, gathers and `vid.v` (the `vector-permutations` concept).
- Strided and indexed loads and stores, widening and saturating arithmetic.
- Calling other functions, and the stack.
- Floating point, which this track does not cover.

## Concepts

- `vectors`

## Prerequisites

- `arrays`: an array arrives as a pointer and a count; elements are 4 bytes apart; `slli` by 2 turns an element count into bytes; test at the top so an empty array does nothing.
- `loops`: while loops with the test at the top, and `j` back to the label.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: moving a pointer on by a fixed number of bytes, such as 16, instead of `vl * 4`; this assumes VLEN.
- `essential`: moving a pointer on by `vl` instead of `vl * 4`; pointers count bytes.
- `essential`: missing `.option arch, +v`, so the file does not assemble.
- `essential`: in `fill_trays`, `add_extra` or `scale_recipe`, a do-while loop with no check before it, if it would store anything when the count is 0.
- `actionable`: in `room_on_trays`, broadcasting the capacity with `vmv.v.x` and then using `vsub.vv`; `vrsub.vx` does it in one instruction.
- `actionable`: using `e8`, `e16` or a load or store width that does not match `int32_t`.
- `informative`: an element-by-element `lw`/`sw` loop instead of vector instructions.
