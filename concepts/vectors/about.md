# About

## The vector extension

The RISC-V vector extension, version 1.0, is often called **RVV**.
It adds 32 vector registers, `v0` to `v31`, and several hundred instructions that work on every element of a vector register at once.
This is **SIMD**: **s**ingle **i**nstruction, **m**ultiple **d**ata.

Unlike many other SIMD instruction sets, RVV does not fix the size of a vector register.
Each processor chooses its own **VLEN**, the number of bits in one vector register.
VLEN is a power of two, at least 128 for the full V extension, and can be as large as 65536.
The same machine code runs on all of them, as long as it never assumes VLEN.
This is called **vector-length agnostic** code.

To use vector instructions, the assembler must know the V extension is available.
Either build with `-march=rv32gcv` (or similar), or write this at the top of the file:

```asm
.option arch, +v
```

The C compiler can also produce vector code itself: this is called **auto-vectorisation**.
A NumPy expression such as `a + b` usually ends up running code like the loops in this concept.

## The vector state

Three values control every vector instruction:

- **SEW**, the selected element width: 8, 16, 32 or 64 bits (`e8`, `e16`, `e32`, `e64`).
- **LMUL**, the register group multiplier (see below).
- **vl**, the number of elements to work on.

`vsetvli` sets all three at once.
The values stay until the next `vsetvli`, so you only need one per round of a loop, unless you change the element width in the middle.

## `vsetvli` in full

```asm
        vsetvli rd, rs1, e32, m1, ta, ma
```

- `rs1` is the **application vector length** (AVL): how many elements you still want to process.
- `rd` receives the new `vl`.
- **VLMAX** is the most elements a register group can hold: `VLEN / SEW * LMUL`.

The rules for `vl` are:

- if AVL ≤ VLMAX, then `vl = AVL`;
- if AVL ≥ 2 × VLMAX, then `vl = VLMAX`;
- in between, `vl` may be anything from about AVL / 2 up to VLMAX, as the processor chooses.

So the only safe thing to do is to use the `vl` the instruction returns.
Most processors (and QEMU) simply give `min(AVL, VLMAX)`.

Two special forms are useful:

- `vsetvli t0, zero, e32, m1, ta, ma` sets `vl` to VLMAX, so `t0` tells you how many 32-bit elements a register holds;
- `vsetvli zero, a1, e32, m1, ta, ma` sets `vl` but throws its value away.

`vsetivli` takes AVL as a 5-bit unsigned immediate (0 to 31) instead of a register.
`vsetvl` takes the element width and other settings from a register, which is rarely needed in hand-written code.

The `vlenb` control register holds VLEN in bytes: `csrr t0, vlenb`.

## Register groups: LMUL

With `m1`, each vector instruction uses one vector register.
With `m2`, `m4` or `m8`, it uses a **group** of 2, 4 or 8 registers together, as if they were one long register.
VLMAX grows by the same factor, so each round of a loop processes more elements, and the loop goes round fewer times.

In a group, the register number must be a multiple of LMUL: with `m4`, use `v0`, `v4`, `v8`, …, and `v8` then means `v8` to `v11`.
The price is that fewer separate groups are left for other values.

There are also fractional settings `mf2`, `mf4` and `mf8`, which use part of one register.
They are useful when a loop mixes element widths, such as bytes and words.

## Tail and mask policies

The **tail** is the elements of a destination register from `vl` up to the end of the register.

- `ta`, tail agnostic: the tail may keep its old values or be filled with all 1 bits.
- `tu`, tail undisturbed: the tail keeps its old values.

Masks let an instruction skip some elements.
`ma` and `mu` (mask agnostic and mask undisturbed) say what happens to the skipped ones.
Masks are covered in a later concept.

Agnostic is the usual choice, because it lets the processor go faster.
Choose undisturbed only when you need the old values in the tail.

The policies apply only to registers.
A vector store writes exactly `vl` elements and never touches memory beyond them.

## Loads and stores

The **unit-stride** loads and stores read or write consecutive elements:

| Load       | Store      | Element size |
| ---------- | ---------- | ------------ |
| `vle8.v`   | `vse8.v`   | 1 byte       |
| `vle16.v`  | `vse16.v`  | 2 bytes      |
| `vle32.v`  | `vse32.v`  | 4 bytes      |
| `vle64.v`  | `vse64.v`  | 8 bytes      |

The address is always a plain register in brackets, such as `(a0)`, with no offset.
The width in the instruction name usually matches the SEW set by `vsetvli`.
There are also strided loads (`vlse32.v`, every *k*-th element) and indexed loads (`vluxei32.v`, a gather from a table of offsets).

With `vl = 0`, a load or store touches no memory at all.
That is why the loop in the RISC-V specification's examples often tests at the bottom, with `bnez`.
Testing at the top, as in this concept, does the same job and matches the array loops you already know.

## Integer arithmetic

| Instruction | `.vv` | `.vx` | `.vi` |
| ----------- | ----- | ----- | ----- |
| `vadd`      | yes   | yes   | yes   |
| `vsub`      | yes   | yes   | no    |
| `vrsub`     | no    | yes   | yes   |
| `vmul`      | yes   | yes   | no    |
| `vmv.v`     | `vmv.v.v` copies a register | `vmv.v.x` broadcasts | `vmv.v.i` broadcasts an immediate |

Operands are written destination first, as for scalar instructions.
`vsub.vv v8, v9, v10` means `v8 = v9 - v10`, element by element.
`vrsub.vx v8, v9, a0` means `v8 = a0 - v9`, and `vrsub.vi v8, v8, 0` negates every element.

The `.vi` immediate is a 5-bit signed number, from -16 to 15.

Other integer instructions include `vand`, `vor`, `vxor`, `vsll`, `vsrl`, `vsra`, `vmin`, `vmax`, `vdiv`, `vrem` and `vmulh`.
There are also **widening** instructions such as `vwadd.vv`, whose results are twice as wide as their inputs, so that sums of bytes do not overflow.

Results wrap around on overflow, exactly as with `add` and `mul`.
The saturating instructions, such as `vsadd`, stop at the largest or smallest value instead.

## The calling convention

Under the standard calling convention, all vector registers, `vl` and the other vector settings are temporaries.
A function may change any of them, and a caller must not expect them to survive a call.

## Comparing with Python and NumPy

| Python / NumPy           | RVV                                         |
| ------------------------ | ------------------------------------------- |
| `c = a + b`              | `vle32.v` × 2, `vadd.vv`, `vse32.v`, in a strip-mining loop |
| `c = a + 5`              | `vadd.vx` (or `vadd.vi` for small numbers)  |
| `c = 100 - a`            | `vrsub.vx`                                  |
| `c = a * k`              | `vmul.vx`                                   |
| `np.full(n, x)`          | `vmv.v.x`, then `vse32.v` in a loop         |
| `np.int32` elements      | `e32` in `vsetvli`                          |
