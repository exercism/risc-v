# About

## What a reduction does

A **reduction** combines all the elements of a vector into one value.
In RVV, every single-width integer reduction has the same form:

```asm
        vredop.vs vd, vs2, vs1      # vd[0] = vs1[0] op vs2[0] op vs2[1] op ... op vs2[vl-1]
```

- `vs2` is the vector being reduced; only its first `vl` elements take part.
- Element 0 of `vs1` is the **seed** (the specification calls it the initial value).
- The result is written to element 0 of `vd`.
  The other elements of `vd` are tail elements, so with `ta` they may change.
- `vd` and `vs1` may be the same register, which is the usual way to write it.
- If `vl` is 0, nothing is written at all, and `vd` keeps its old value.

The integer reductions are:

| Instruction   | Operation                       | Identity (a seed that changes nothing) |
| ------------- | ------------------------------- | -------------------------------------- |
| `vredsum.vs`  | sum, wrapping on overflow       | 0                                      |
| `vredmax.vs`  | signed maximum                  | `0x80000000` (INT32_MIN)               |
| `vredmin.vs`  | signed minimum                  | `0x7FFFFFFF` (INT32_MAX)               |
| `vredmaxu.vs` | unsigned maximum                | 0                                      |
| `vredminu.vs` | unsigned minimum                | `0xFFFFFFFF` (UINT32_MAX)              |
| `vredand.vs`  | bitwise AND of every element    | `0xFFFFFFFF` (-1)                      |
| `vredor.vs`   | bitwise OR of every element     | 0                                      |
| `vredxor.vs`  | bitwise XOR of every element    | 0                                      |

The identities are for SEW = 32; for other element widths they shrink to match.
`vredor.vs` answers "is any bit set in any element?", and `vredand.vs` answers "is this bit set in every element?".

There is also a **widening** sum, `vwredsum.vs` (and `vwredsumu.vs` for unsigned), whose seed and result are twice as wide as the elements.
It is useful for adding up bytes or halfwords without overflow.

Reductions can be masked with `v0.t`, so that only some elements take part.
Masks are covered in the `vector-masks` concept.

## Moving single elements

| Instruction       | Meaning                                             |
| ----------------- | --------------------------------------------------- |
| `vmv.s.x vd, rs1` | `vd[0] = rs1`, if `vl` > 0; other elements are tail |
| `vmv.x.s rd, vs2` | `rd = vs2[0]`, whatever `vl` is, even 0             |

If SEW is narrower than XLEN, `vmv.x.s` sign-extends the element.
If SEW is wider than XLEN, `vmv.s.x` sign-extends `rs1` and `vmv.x.s` keeps the low bits.

## Pattern 1: reduce every round, seed in an ordinary register

The introduction keeps the answer so far in an ordinary register, and does `vmv.s.x`, the reduction and `vmv.x.s` in every round.
This is easy to read, works for every reduction, and returns the starting value when the array is empty.

A variation keeps the answer in element 0 of a vector register for the whole loop.
The seed has to be written before the first round, when `vl` has not yet been set, so set it first:

```asm
        vsetivli zero, 1, e32, m1, ta, ma   # vl = 1
        vmv.s.x v9, zero                    # v9[0] = 0, the seed
.Lloop:
        blez    a1, .Ldone
        vsetvli t0, a1, e32, m1, ta, ma
        vle32.v v8, (a0)
        vredsum.vs v9, v8, v9               # v9[0] = v9[0] + this round's sum
        ...
.Ldone:
        vmv.x.s a0, v9                      # a0 = v9[0], even when vl is 0
        ret
```

## Pattern 2: add element by element, reduce once at the end

A reduction is slower than an ordinary vector add, because it has to combine elements from across the whole register.
For a long array, it can be faster to keep a vector of partial sums, add each strip into it with `vadd.vv`, and reduce only once, after the loop:

```asm
        vsetvli t0, zero, e32, m1, ta, ma   # vl = VLMAX
        vmv.v.i v10, 0                      # every partial sum = 0
.Lloop:
        blez    a1, .Ldone
        vsetvli t0, a1, e32, m1, tu, ma     # tu: tail undisturbed
        vle32.v v8, (a0)
        vadd.vv v10, v10, v8                # partial sums += this strip
        slli    t1, t0, 2
        add     a0, a0, t1
        sub     a1, a1, t0
        j       .Lloop
.Ldone:
        vsetvli t0, zero, e32, m1, ta, ma   # vl = VLMAX again
        vmv.s.x v9, zero                    # seed = 0
        vredsum.vs v9, v10, v9              # add up every partial sum
        vmv.x.s a0, v9
        ret
```

The `tu` in the loop matters.
In the last round, `vl` may be smaller than VLMAX.
The `vadd.vv` then writes only the first `vl` partial sums, and the ones past `vl` are the tail.
They still hold sums from earlier rounds.
With `tu`, they keep those values.
With `ta`, the processor may fill them with all 1 bits, and the final reduction would add up garbage.

The final `vsetvli` sets `vl` back to VLMAX, so that the reduction includes every partial sum, not just the first `vl` of the last round.
Because every partial sum started at 0, the ones that were never used add nothing.

This pattern suits sums, and also minimums and maximums if the partial results start at the identity and use `vmin.vv` or `vmax.vv`.
The additions happen in a different order from pattern 1, but integer sums wrap the same way in any order, so the answer is the same.

## Ordered and unordered

The integer reductions give the same answer whatever order the hardware combines the elements in.
The floating-point extension has two sum reductions, an ordered `vfredosum.vs` and an unordered `vfredusum.vs`, because rounding makes the order matter there.

## Comparing with Python and NumPy

| Python / NumPy          | RVV                          |
| ----------------------- | ---------------------------- |
| `sum(a)`, `a.sum()`     | `vredsum.vs`, seed 0         |
| `sum(a, start)`         | `vredsum.vs`, seed `start`   |
| `min(a)`, `a.min()`     | `vredmin.vs` (`vredminu.vs` for `np.uint32`) |
| `max(a)`, `a.max()`     | `vredmax.vs` (`vredmaxu.vs` for `np.uint32`) |
| `np.bitwise_or.reduce(a)`  | `vredor.vs`, seed 0       |
| `np.bitwise_and.reduce(a)` | `vredand.vs`, seed -1     |
| `np.bitwise_xor.reduce(a)` | `vredxor.vs`, seed 0      |
| `(a + b).sum()`         | `vadd.vv`, then `vredsum.vs` |

Python's `min` and `max` raise an error for an empty list, unless you give a `default`.
In assembly, you decide what an empty array returns: usually the seed.
