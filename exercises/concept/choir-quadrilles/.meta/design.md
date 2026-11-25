# Design

## Learning objectives

- Know that a permutation moves elements to new places, like NumPy's `a[::-1]`, `a[::2]`, `a[idx]`, `a[mask]` and `np.roll`.
- Load with a byte stride held in an ordinary register, using `vlse32.v`, and move the pointer on by `vl * stride`.
- Walk backwards through memory with a negative stride.
- Build an index vector with `vid.v` and vector arithmetic such as `vxor.vi`.
- Gather elements with `vrgather.vv`, and know that its destination must not overlap its sources and that an index past the register gives 0.
- Shift elements along with `vslide1up.vx`, and read any element into an ordinary register with `vslidedown.vx` and `vmv.x.s`.
- Pack the elements chosen by a mask with `vcompress.vm`, count them with `vcpop.m`, and store only those by setting `vl` with a second `vsetvli`.
- Plan permutations that cross strips: choose where each round reads and writes, carry values between rounds in ordinary registers, and keep pairs inside one strip by choosing an even `vl`.

## Out of scope

- Indexed loads and stores such as `vluxei32.v` and `vsoxei32.v` (described in `about.md` only).
- `vrgatherei16.vv`, `viota.m` and `vslide1down.vx` in the exemplar (`vslide1down` is in the introduction; the others are in `about.md` only).
- Strided stores `vsse32.v` in the exemplar (taught in the introduction).
- Byte vectors and fault-only-first loads (the `vector-strings` concept).
- LMUL other than `m1`, and element widths other than `e32`.
- The tail-undisturbed policy `tu`.
- Calling other functions, and the stack.
- Floating point, which this track does not cover.

## Concepts

- `vector-permutations`

## Prerequisites

- `vector-masks`: `vmsgt.vx` masks in `v0`, and `vcpop.m`.
- `vector-reductions` (through `vector-masks`): reading element 0 with `vmv.x.s`, and carrying a value across rounds in an ordinary register.
- `vectors` (through `vector-reductions`): `.option arch, +v`, `vsetvli` and `vl`, `vle32.v`, `vse32.v`, and the strip-mining loop.
- `arrays`, `shifts` and `bitwise-operations` (through `vectors`): pointer arithmetic with `slli`, `andi` with -2 to round down to an even number, and `xor`.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: moving a pointer on by a fixed number of bytes, such as 16, instead of a multiple of `vl`; this assumes VLEN.
- `essential`: in `zigzag`, using `vl` from `vsetvli` without making it even; the specification allows an odd `vl` for an even count, although QEMU never gives one.
- `essential`: using the same register as destination and source of `vrgather.vv`, `vslide1up.vx` or `vcompress.vm`.
- `essential`: in `keep_high_notes`, storing the whole strip after `vcompress.vm`; the elements past the kept notes overwrite the destination.
- `essential`: in `echo_previous`, putting the given first note into element 0 in every round instead of the last note of the round before.
- `actionable`: in `reverse_tune`, reversing each strip with `vrgather.vv` while also walking from the end; it is correct, but a negative stride does it in one load.
- `informative`: an element-by-element `lw` loop instead of vector instructions.
