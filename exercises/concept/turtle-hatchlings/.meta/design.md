# Design

## Learning objectives

- Know that RISC-V has a base instruction set, I, and optional extensions such as M, C, V and Zbb, named in an arch string such as `rv32imc_zbb`.
- Know that `mul`, `div` and `rem` come from the M extension, not the base.
- Enable an extension in an assembly file with `.option arch, +zbb`.
- Count set bits with `cpop`, replacing the `x & (x - 1)` loop from the bit-sets concept.
- Find the lowest set bit with `ctz` and the highest with `31 - clz`, and know that both give 32 for 0.
- Use `andn` for set difference, replacing `not` and `and`, and know `orn` and `xnor`.
- Choose between signed `min`/`max` and unsigned `minu`/`maxu`, and clamp a value into a range without a branch.
- Rotate with `rol`, `ror` and `rori`, and know that there is no `roli`.
- Relate `cpop` and `clz` to Python's `int.bit_count()` and `int.bit_length()`.

## Out of scope

- `rev8`, `orc.b`, `sext.b`, `sext.h` and `zext.h` (covered in `about.md` only).
- The Zba, Zbs and Zbc extensions.
- RV64 forms such as `cpopw`, `clzw` and `rolw`.
- Calling other functions, and the stack.
- Vector instructions.
- Floating point, which this track does not cover.

## Concepts

- `bit-manipulation-extension`

## Prerequisites

- `bit-sets`: a `uint32_t` as a set of the numbers 0 to 31, set difference with `not` and `and`, counting members, and finding the smallest member.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: the `.option arch, +zbb` line removed from the solution, so the Zbb instructions do not assemble.
- `essential`: `first_hatched` returning the result of `ctz` for an empty nest, which gives 32 rather than -1.
- `essential`: `clamp_reading` using `maxu` or `minu`, which treat negative readings as large positive numbers.
- `actionable`: `hatched`, `first_hatched` or `last_hatched` using a loop instead of `cpop`, `ctz` or `clz`.
- `actionable`: `still_waiting` using `not` and `and` instead of `andn`, or subtracting two `cpop` counts, which counts crab holes.
- `actionable`: `rotate_watch` built from `sll`, `srl` and `or` instead of `rol`.
- `actionable`: `clamp_reading` using branches instead of `max` and `min`.
- `informative`: `last_hatched` needs no branch, because `31 - 32` is already -1 for an empty nest.
