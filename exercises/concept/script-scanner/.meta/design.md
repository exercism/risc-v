# Design

## Learning objectives

- Use `e8` byte elements, and know that one register then holds VLEN / 8 bytes (16 with VLEN = 128, 64 with VLEN = 512).
- Load and store bytes with `vle8.v` and `vse8.v`, and move a pointer on by `vl` bytes.
- Ask for as many elements as fit with `vsetvli t0, zero, ...`.
- Know why a string cannot be loaded with `vle8.v`: its length is unknown, and the bytes after the zero byte may be in a page the program cannot read.
- Load a string with the fault-only-first `vle8ff.v`, and read the possibly smaller `vl` back with `csrr t0, vl`.
- Write the strlen loop: `vle8ff.v`, `vmseq.vi` with 0, `vfirst.m`, and stop at the strip that holds the zero byte.
- Cut `vl` down with `vsetvli` to the index of the zero byte (or one more), so that later instructions ignore, and never write, the bytes after it.
- Know that `.vx` operands use the low 8 bits of the register, and that the signed compares see bytes from 128 to 255 as negative.
- Build a character-class mask with two unsigned compares and `vmand.mm`, or with a subtraction and one unsigned compare.
- Count characters with `vcpop.m`, and find one with `vfirst.m`, without counting or finding bytes after the zero byte.
- Change a string in place with a masked `vsub.vx` and `mu`, and copy a string including its zero byte.

## Out of scope

- `vmsbf.m`, `vmsif.m`, `vmsof.m` and masked stores as other ways to stop at the zero byte (described in `about.md` only).
- LMUL other than `m1`, and element widths other than `e8`.
- The CSRs `vtype` and `vlenb`, and `vstart`.
- `vle8ff.v` for arrays with a known length; an ordinary strip-mining loop is enough there.
- UTF-8 decoding: multi-byte characters are counted as bytes.
- Slides and gathers (the `vector-permutations` concept).
- Calling other functions, and the stack.
- Floating point, which this track does not cover.

## Concepts

- `vector-strings`

## Prerequisites

- `vector-masks`: `vmseq`, `vmsltu`, `vmsleu`, `vmsgtu`, `vmand.mm`, `vcpop.m`, `vfirst.m`, masked instructions with `v0.t`, and the `mu` mask policy.
- `strings`: the zero terminator, character literals, `lbu` versus `lb`, the one-compare range check, case conversion with `'a' - 'A'`, and writing a string into a caller's buffer.
- `vector-reductions` and `vectors` (through `vector-masks`): `.option arch, +v`, `vsetvli`, the tail policy, and the strip-mining loop.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: loading a line with `vle8.v` instead of `vle8ff.v`; it can fault when the line ends at the end of a page.
- `essential`: using the `t0` from `vsetvli` after `vle8ff.v`, without reading `vl` again with `csrr`.
- `essential`: in `shout` and `copy_line`, storing the strip that holds the zero byte without cutting `vl` down; bytes after the line are changed.
- `essential`: in `count_pauses`, `count_capitals` and `find_cue`, counting or finding bytes after the zero byte.
- `essential`: in `shout`, a masked `vsub` with `ma`; the tests run with `rvv_ma_all_1s=true`, so this fails at VLEN = 512.
- `essential`: signed compares (`vmslt`, `vmsle`, `vmsgt`) for a character class; bytes from 128 to 255 are negative.
- `essential`: moving a pointer on by a fixed number of bytes, such as 16, instead of `vl`; this assumes VLEN.
- `actionable`: a test at the top of the loop on a count that does not exist; the loop ends at the zero byte instead.
- `informative`: an element-by-element `lbu` loop instead of vector instructions.
