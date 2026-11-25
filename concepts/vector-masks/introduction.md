# Introduction

So far, every vector instruction has done the same thing to every element.
Often you want to ask a question about each element first, and then treat the elements differently depending on the answer.

NumPy does this with **boolean arrays**:

```python
colours = np.array([3, 1, 3, 0])
is_gold = colours == 3          # array([ True, False,  True, False])
print(is_gold.sum())            # 2: True counts as 1
print(np.where(is_gold, 9, colours))   # array([9, 1, 9, 0])
```

The vector extension has the same idea, called a **mask**.

## Masks

A **mask** holds one bit for each element: 1 for true and 0 for false.
A mask lives in an ordinary vector register.
Bit 0 of the register belongs to element 0, bit 1 to element 1, and so on.

```text
v8:  [  3 |  1 |  3 |  0 ]      the elements
v0:  [  1 |  0 |  1 |  0 ]      a mask: "is the element 3?", one bit each
```

This is the same as the booleans of the comparisons concept, which are 0 or 1, but there is one for every element.

## Comparing vectors

A vector compare instruction compares every element and writes a mask.
All their names start with `vms`, for "**v**ector **m**ask **s**et".

| Instruction | Mask bit `i` is 1 when |
| ----------- | ---------------------- |
| `vmseq`     | `vs[i] == x` (equal)     |
| `vmsne`     | `vs[i] != x` (not equal) |
| `vmslt`     | `vs[i] < x`  (less than) |
| `vmsle`     | `vs[i] <= x` (less than or equal) |
| `vmsgt`     | `vs[i] > x`  (greater than) |
| `vmsltu`, `vmsleu`, `vmsgtu` | the same, but treating the elements as unsigned |

As with `vadd`, the second operand may be another vector (`.vv`), an ordinary register (`.vx`) or a small immediate (`.vi`).
`vmsgt` and `vmsgtu` have only the `.vx` and `.vi` forms.

```asm
        vmslt.vx v0, v8, zero   # bit i of v0 = 1 if v8[i] < 0
        vmseq.vi v9, v8, 7      # bit i of v9 = 1 if v8[i] == 7
        vmsle.vv v10, v8, v11   # bit i of v10 = 1 if v8[i] <= v11[i]
```

There is no "greater than or equal" compare.
Ask the opposite question, "less than", and flip the mask with `vmnot.m` (see below).

As with `vredmax` and `vredmaxu`, the `u` versions are for `uint32_t` elements.
`vmslt` thinks `0xFFFFFFFF` is -1, smaller than 5, while `vmsltu` thinks it is 4294967295, larger than 5.

Like other results, a mask only has meaning for the first `vl` elements.

## Counting and finding

Two instructions turn a mask into an ordinary number:

```asm
        vcpop.m t3, v0          # t3 = how many of the first vl bits of v0 are 1
        vfirst.m t2, v0         # t2 = the index of the first 1 bit of v0, or -1 if there is none
```

`vcpop` means "**c**ount **pop**ulation", the number of 1 bits.
It is like `mask.sum()` in NumPy.

`vfirst.m` is like `np.argmax(mask)`, but it gives -1, not 0, when no element is true.

In a strip-mining loop, each round only sees one strip.
`vcpop.m` counts the matches in this strip, so add each round's count to a total kept in an ordinary register.
`vfirst.m` gives an index **within this strip**.
To turn it into an index in the whole array, add the number of elements in earlier strips.
You can work that out from the pointers: the pointer has moved on 4 bytes for each earlier element.
If you only want the first match, you can leave the loop as soon as you find one.

## Choosing per element: `vmerge`

`vmerge` chooses, for each element, between two values, like `np.where`:

```asm
        vmerge.vim v8, v8, 0, v0    # v8[i] = 0 if bit i of v0 is 1, else v8[i]
```

```python
v8 = np.where(v0, 0, v8)
```

The operands are, in order: the destination, the value for a 0 bit, the value for a 1 bit, and the mask.
The value for a 1 bit can be a vector (`vmerge.vvm`), an ordinary register (`vmerge.vxm`) or an immediate (`vmerge.vim`).
The mask must be in `v0`.

## Masked instructions: `v0.t`

Most vector instructions can be **masked**: add `, v0.t` at the end.
The instruction then works only on the **active** elements, the ones whose bit in `v0` is 1.
The other elements are **inactive**, and are left alone.

```asm
        vadd.vi v8, v8, 1, v0.t     # v8[i] = v8[i] + 1, but only where bit i of v0 is 1
```

```python
v8[v0] += 1
```

`v0.t` means "true in `v0`".
Only `v0` can control an instruction, so a mask that controls `v0.t` or `vmerge` must be in `v0`.
Masks that you only count, find or combine can be in any vector register.

## The mask policy: `mu` and `ma`

The last part of `vsetvli` is the **mask policy**: it says what happens to the inactive elements of a masked instruction.

| Policy | Name              | Inactive elements                                           |
| ------ | ----------------- | ----------------------------------------------------------- |
| `ma`   | mask agnostic     | may keep their old values, or may be filled with all 1 bits |
| `mu`   | mask undisturbed  | always keep their old values                                |

Up to now, `ma` was fine, because nothing was masked.
If you mask an instruction and then store the **whole** register, the inactive elements are stored too.
With `ma`, they might be stored as -1 (all 1 bits) instead of their old values.
So choose `mu` when you need them:

```asm
        vsetvli t0, a1, e32, m1, ta, mu     # mu: inactive elements keep their values
```

Another way is to work out a whole new vector without a mask, and then use `vmerge` to choose between the old and new values.

## Masked reductions

A reduction can be masked too.
Then only the active elements are combined with the seed:

```asm
        vredsum.vs v10, v8, v10, v0.t   # v10[0] = v10[0] + the sum of the active elements of v8
```

```python
total = total + v8[v0].sum()
```

If no element is active, the result is just the seed.
A reduction only writes element 0, so the mask policy does not matter here.

## Combining masks

These instructions combine masks bit by bit, like `&`, `|` and `~` on NumPy boolean arrays:

```asm
        vmand.mm v9, v9, v10    # bit i = 1 if bit i of v9 and bit i of v10 are both 1
        vmor.mm v9, v9, v10     # bit i = 1 if either is 1
        vmnot.m v9, v9          # bit i = 1 if bit i of v9 was 0
```

```python
v9 = v9 & v10
v9 = v9 | v10
v9 = ~v9
```

The ending `.mm` means "**m**ask and **m**ask".
