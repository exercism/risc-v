# About

## Mask registers

A **mask** is a vector register used as a row of bits, one for each element.
Bit *i* of the register (counting from bit 0 of byte 0) belongs to element *i*, whatever the element width.
So a mask for `vl` elements uses only the first `vl` bits of the register, and the rest of it is ignored.

Any vector register can hold a mask.
Only `v0` can **control** an instruction, as the `v0.t` operand of a masked instruction, or as the selector of `vmerge` and of the carry instructions such as `vadc`.

Mask results are always written as if the tail policy were agnostic: bits past `vl` may change.
Instructions that read a mask, such as `vcpop.m`, look only at the first `vl` bits.

## Compares

Every integer compare writes a mask.

| Instruction | `.vv` | `.vx` | `.vi` | Bit `i` is 1 when            |
| ----------- | ----- | ----- | ----- | ---------------------------- |
| `vmseq`     | yes   | yes   | yes   | `vs2[i] == op`               |
| `vmsne`     | yes   | yes   | yes   | `vs2[i] != op`               |
| `vmslt`     | yes   | yes   | no    | `vs2[i] < op`, signed        |
| `vmsltu`    | yes   | yes   | no    | `vs2[i] < op`, unsigned      |
| `vmsle`     | yes   | yes   | yes   | `vs2[i] <= op`, signed       |
| `vmsleu`    | yes   | yes   | yes   | `vs2[i] <= op`, unsigned     |
| `vmsgt`     | no    | yes   | yes   | `vs2[i] > op`, signed        |
| `vmsgtu`    | no    | yes   | yes   | `vs2[i] > op`, unsigned      |

The missing forms are not needed:

- `vmsgt.vv vd, va, vb` is `vmslt.vv vd, vb, va` with the operands swapped; the assembler accepts it as a pseudo-instruction.
- `vmslt.vi vd, va, k` is the same as `vmsle.vi vd, va, k-1`.
- "Greater than or equal" (`vmsge.vv`) is `vmsle.vv` with the operands swapped.
  `vmsge.vx` has no single instruction: use `vmslt.vx` followed by `vmnot.m`, or `vmsgt.vx` with `x - 1` if that cannot overflow.

The `.vi` immediate is 5 bits and signed, from -16 to 15, even for the unsigned compares.

## Reading masks

| Instruction         | Result                                                       |
| ------------------- | ------------------------------------------------------------ |
| `vcpop.m rd, vs2`   | the number of 1 bits among the first `vl` bits               |
| `vfirst.m rd, vs2`  | the index of the lowest 1 bit among the first `vl`, or -1   |

Both can themselves be masked with `v0.t`: then only bits that are also set in `v0` count.
`vcpop.m` was called `vpopc.m` in drafts of the specification before version 1.0.

Related mask instructions:

- `vmsbf.m`, `vmsif.m` and `vmsof.m` set the bits **b**efore, **i**ncluding or **o**nly at the first set bit.
- `viota.m` writes, for each element, how many mask bits before it are set.
- `vid.v` writes each element's own index, 0, 1, 2, …
- `vcompress.vm` packs the active elements together at the start of a register, like `a[mask]` in NumPy.

## Mask logic

| Instruction            | Bit `i`                    | NumPy      |
| ---------------------- | -------------------------- | ---------- |
| `vmand.mm vd, va, vb`  | `a & b`                    | `a & b`    |
| `vmor.mm vd, va, vb`   | `a \| b`                   | `a \| b`   |
| `vmxor.mm vd, va, vb`  | `a ^ b`                    | `a ^ b`    |
| `vmandn.mm vd, va, vb` | `a & ~b`                   | `a & ~b`   |
| `vmorn.mm vd, va, vb`  | `a \| ~b`                  | `a \| ~b`  |
| `vmnand.mm vd, va, vb` | `~(a & b)`                 |            |
| `vmnor.mm vd, va, vb`  | `~(a \| b)`                |            |
| `vmxnor.mm vd, va, vb` | `~(a ^ b)`                 |            |

The assembler also gives these pseudo-instructions:

| Pseudo-instruction | Becomes                 |
| ------------------ | ----------------------- |
| `vmnot.m vd, vs`   | `vmnand.mm vd, vs, vs`  |
| `vmmv.m vd, vs`    | `vmand.mm vd, vs, vs`   |
| `vmclr.m vd`       | `vmxor.mm vd, vd, vd`   |
| `vmset.m vd`       | `vmxnor.mm vd, vd, vd`  |

`vmandn.mm` saves an instruction: "at least `low` and at most `high`" is `vmsle(x, high)` and-not `vmslt(x, low)`.

## Masked instructions

Most vector instructions take an optional last operand `v0.t`.
Elements whose bit in `v0` is 1 are **active** and are computed as usual.
The others are **inactive**.

What happens to inactive elements of the destination depends on the **mask policy** set by `vsetvli`:

- `mu`, mask undisturbed: they keep their old values.
- `ma`, mask agnostic: they may keep their old values or be overwritten with all 1 bits.
  Hardware may choose differently each time.

Agnostic lets the hardware go faster, so choose `mu` only when you need the old values.
QEMU keeps the old values for both unless it is started with `rvv_ma_all_1s=true`, so a program that wrongly uses `ma` may still pass its tests under QEMU.

The destination of a masked instruction must not be `v0` itself (unless it is a compare writing a new mask, or a reduction).

### Masked loads and stores

Loads and stores can be masked too:

```asm
        vle32.v v8, (a0), v0.t      # load only the active elements
        vse32.v v8, (a0), v0.t      # store only the active elements; memory for inactive ones is not touched
```

A masked store is another way to update only some elements of an array.
Inactive elements of a masked load never touch memory, so they cannot cause a fault.

### Masked reductions

With `v0.t`, a reduction combines the seed with the active elements only.
If none is active, the result is the seed.
The result is written to element 0 whatever the mask, so the mask policy does not matter.

## `vmerge` and `vmv`

| Instruction                 | Element `i`                         |
| --------------------------- | ----------------------------------- |
| `vmerge.vvm vd, vs2, vs1, v0` | `v0[i] ? vs1[i] : vs2[i]`         |
| `vmerge.vxm vd, vs2, rs1, v0` | `v0[i] ? rs1 : vs2[i]`            |
| `vmerge.vim vd, vs2, imm, v0` | `v0[i] ? imm : vs2[i]`            |

`vmerge` always writes every element, so the mask policy does not apply.
`vmv.v.v`, `vmv.v.x` and `vmv.v.i` are encoded as the unmasked forms of `vmerge`.

For a simple cap or floor, `vmin.vx` and `vmax.vx` (and `vminu` and `vmaxu`) do in one instruction what a compare and a `vmerge` do in two.

## Finding the first match

A common pattern is "search with early exit":

```asm
        mv      t3, a0                  # remember where the array starts
.Lloop:
        blez    a1, .Lnone
        vsetvli t0, a1, e32, m1, ta, ma
        vle32.v v8, (a0)
        vmseq.vx v0, v8, a2
        vfirst.m t2, v0
        bgez    t2, .Lfound             # stop at the first strip with a match
        ...                             # move on as usual
.Lfound:
        sub     t1, a0, t3              # bytes before this strip
        srli    t1, t1, 2               # elements before this strip
        add     a0, t1, t2              # the index in the whole array
        ret
```

Instead of pointer arithmetic, you can keep a separate count of the elements already searched, adding `vl` each round.

## Comparing with Python and NumPy

| Python / NumPy                     | RVV                                          |
| ---------------------------------- | -------------------------------------------- |
| `m = a == x`                       | `vmseq.vx`                                   |
| `m = a < b`                        | `vmslt.vv`                                   |
| `m.sum()`, `np.count_nonzero(m)`   | `vcpop.m`                                    |
| `np.argmax(m)` (if `m.any()`)      | `vfirst.m`, which gives -1 when nothing matches |
| `m.any()`                          | `vfirst.m` and `bgez`, or `vcpop.m` and `bnez` |
| `np.where(m, x, a)`                | `vmerge.vxm`                                 |
| `a[m] += x`                        | `vadd.vx ..., v0.t` with `mu`                |
| `a[m].sum()`                       | `vredsum.vs ..., v0.t`                       |
| `m1 & m2`, `m1 \| m2`, `~m`        | `vmand.mm`, `vmor.mm`, `vmnot.m`             |
| `np.minimum(a, x)`                 | `vmin.vx`, or `vmsgt.vx` and `vmerge.vxm`    |
