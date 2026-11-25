# Design

## Learning objectives

- Know that a reduction combines all the elements of a vector into one value, like Python's `sum`, `min` and `max`.
- Use `vredsum.vs vd, vs2, vs1`, and know that the seed is element 0 of `vs1` and the result is element 0 of `vd`.
- Move a value into element 0 with `vmv.s.x`, and out of element 0 with `vmv.x.s`.
- Carry a running answer across the rounds of a strip-mining loop, by using each round's result as the next round's seed.
- Use `vredmin.vs` and `vredmax.vs`, and choose a seed that does not change the answer: the first element, or INT32_MIN / INT32_MAX.
- Know that a seed of 0 is wrong for a maximum when every element may be negative.
- Know that the same bits mean different numbers as `int32_t` and `uint32_t`, and use `vredmaxu.vs` for unsigned elements.
- Combine element-wise arithmetic (`vadd.vv`) with a reduction.
- Do two reductions on the same strip in one loop.
- Return the seed for an empty array.

## Out of scope

- The pattern of accumulating with `vadd.vv` under `tu` and reducing once at the end (described in `about.md` only).
- `vredand.vs`, `vredor.vs`, `vredxor.vs` and the widening reductions (described in `about.md` only).
- Masked reductions, comparisons and `vmerge` (the `vector-masks` concept).
- Scalar unsigned comparisons such as `bltu` and `sltu` (the `unsigned-integers` and `comparisons` concepts).
- LMUL other than `m1`, and element widths other than `e32`.
- Calling other functions, and the stack.
- Floating point, which this track does not cover.

## Concepts

- `vector-reductions`

## Prerequisites

- `vectors`: `.option arch, +v`, `vsetvli`, `vle32.v`, `vadd.vv`, and the strip-mining loop that moves pointers on by `vl * 4` and tests the count at the top.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: moving a pointer on by a fixed number of bytes, such as 16, instead of `vl * 4`; this assumes VLEN.
- `essential`: `vmv.s.x` before the first `vsetvli`, where `vl` may be 0 and nothing is written.
- `essential`: in `best_improvement`, a seed of 0, which gives the wrong answer when every change is negative.
- `essential`: in `longest_paddle`, `vredmax.vs` instead of `vredmaxu.vs`.
- `actionable`: in `lap_spread`, two separate loops over the array, or calling `fastest_lap`, instead of two reductions on each strip.
- `actionable`: reducing only the last round, by seeding every round with 0 and not carrying the answer forward.
- `informative`: an element-by-element `lw` loop instead of vector instructions.
