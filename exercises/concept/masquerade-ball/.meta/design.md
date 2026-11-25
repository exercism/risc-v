# Design

## Learning objectives

- Know that a mask holds one bit per element in a vector register, like a NumPy boolean array.
- Write masks with the compares `vmseq`, `vmsne`, `vmslt`, `vmsle` and `vmsgt`, in their `.vx` and `.vi` forms, and with the unsigned `vmsltu` and `vmsleu`.
- Count the set bits of a mask with `vcpop.m`, and add the counts across the rounds of a strip-mining loop.
- Find the first set bit with `vfirst.m`, leave the loop early, and turn the index in a strip into an index in the whole array with pointer arithmetic.
- Choose per element with `vmerge.vxm`, like `np.where`.
- Know that only `v0` can control an instruction, through `v0.t` or `vmerge`.
- Mask an arithmetic instruction with `v0.t`, and know why `mu` is needed when the whole register is stored afterwards.
- Mask a reduction with `v0.t`, so that only the active elements are added to the seed.
- Combine masks with `vmnot.m` and `vmand.mm`, and know that there is no `vmsge.vx`.

## Out of scope

- `vmandn.mm`, `vmxor.mm` and the other mask logic instructions (described in `about.md` only).
- Masked loads and stores, `vcompress`, `viota`, `vid` and `vmsbf`/`vmsif`/`vmsof` (described in `about.md` only).
- `vmin` and `vmax`, which do a cap in one instruction (described in `about.md` only).
- Byte vectors and fault-only-first loads (the `vector-strings` concept).
- Slides and gathers (the `vector-permutations` concept).
- Scalar unsigned comparisons such as `bltu` and `sltu` (the `unsigned-integers` concept).
- LMUL other than `m1`, and element widths other than `e32`.
- Calling other functions, and the stack.
- Floating point, which this track does not cover.

## Concepts

- `vector-masks`

## Prerequisites

- `vector-reductions`: `vredsum.vs`, seeding with `vmv.s.x` and reading the result with `vmv.x.s`, carrying the answer across rounds, and signed versus unsigned elements.
- `comparisons`: booleans as 0 and 1, and combining them with `and`, `or` and "not".
- `vectors` (through `vector-reductions`): `.option arch, +v`, `vsetvli` with its tail and mask policies, `vle32.v`, `vse32.v`, `vsub.vx`, and the strip-mining loop.
- `arrays` and `shifts` (through `vectors`): pointers move by `vl * 4`, and a byte difference shifted right by 2 is an element count.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: moving a pointer on by a fixed number of bytes, such as 16, instead of `vl * 4`; this assumes VLEN.
- `essential`: in `discount_juniors`, a masked `vsub` with `ma` followed by an unmasked store of the whole strip; the inactive prices may be lost.
  `bin/verify-exercises` catches this at VLEN 512, where it starts QEMU with `rvv_ma_all_1s=true`, but a default QEMU (as the test runner uses) keeps inactive elements under `ma`, so a student's wrong `ma` can still pass there.
- `essential`: in `first_unmasked`, returning the index from `vfirst.m` without adding the guests in earlier strips.
- `essential`: in `count_winners`, using the signed compares `vmslt` and `vmsle`.
- `essential`: using `v0.t` or `vmerge` with a mask that is not in `v0`.
- `actionable`: in `first_unmasked`, carrying on through the rest of the array after the first match is found.
- `actionable`: in `cap_donations`, using `vmin.vx`; it is correct, but this exercise is meant to practise `vmerge`.
- `informative`: an element-by-element `lw` loop instead of vector instructions.
