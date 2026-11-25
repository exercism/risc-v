# Introduction

So far, every instruction has worked on one number at a time.
To add 1 to every element of an array, your loop loads one element, adds 1, stores it, and goes round again, once for each element.

Many RISC-V processors also have a **vector extension**, called **V**.
Its instructions work on many elements at once.
One vector `vadd` can add 4, 16 or even more pairs of numbers in a single step.
This idea is called **SIMD**: **s**ingle **i**nstruction, **m**ultiple **d**ata.

If you have used NumPy in Python, you have seen this idea already.
With a plain Python list, you write a loop, and it handles one element at a time:

```python
totals = []
for i in range(len(a)):
    totals.append(a[i] + b[i])
```

With NumPy arrays, you write one operation for the whole array:

```python
totals = a + b      # adds every pair of elements
bigger = a + 5      # adds 5 to every element
```

Vector instructions let you do the same in assembly.

## Turning the vector extension on

The vector extension is optional, so the assembler does not accept vector instructions unless you ask for them.
This line at the top of the `.S` file asks for them:

```asm
.option arch, +v        # allow vector instructions in this file
```

`.option arch` is a directive that changes which instructions the assembler accepts.
`+v` means "add the V extension".
The stub for this exercise already starts with this line, so leave it there.

## Vector registers

The vector extension has its own 32 registers, called `v0` to `v31`.
Each one holds a row of elements side by side.

```text
v8:  [ 12 | 40 |  7 | 23 ]      four 32-bit elements
```

The size of a vector register, in bits, is called **VLEN**.
The processor designer chooses VLEN, not you.
With VLEN = 128, a vector register holds four 32-bit elements.
With VLEN = 512, it holds sixteen.

The same program must run correctly on both, so **your code must never assume VLEN**.
The tests for this exercise run your code with two different values of VLEN.

All the vector registers are temporary, like `t0` to `t6`: your function may change any of them.
`v0` has a special job that you will meet in a later concept, so the examples here start at `v8`.

## Setting the vector length: `vsetvli`

Before using vector instructions, you tell the processor what kind of elements you have, and how many you still want to process.
The processor answers with how many it will process this time.

```asm
        vsetvli t0, a1, e32, m1, ta, ma     # t0 = vl, the number of elements for this round
```

Here is what each part means:

| Part   | Meaning                                                                                                |
| ------ | ------------------------------------------------------------------------------------------------------ |
| `t0`   | receives **vl**, the number of elements each vector instruction will work on                           |
| `a1`   | the number of elements you still want to process                                                       |
| `e32`  | the **element width**: each element is 32 bits, like `int32_t` (`e8` and `e16` give 8 and 16 bits)     |
| `m1`   | use one vector register at a time (always `m1` in this exercise)                                       |
| `ta`   | **tail agnostic**: we do not care what happens to the elements of a register past `vl`                 |
| `ma`   | **mask agnostic**: we do not use masks here, so we do not care about them either                       |

`vl` is never more than the number you asked for, and never more than fits in one register.
So with VLEN = 128 and `a1 = 10`, the first `vsetvli` gives `vl = 4`.
With `a1 = 3`, it gives `vl = 3`.

Every vector instruction after `vsetvli` works on the first `vl` elements only, until the next `vsetvli`.
Your code should use whatever `vl` comes back, without guessing it.

## Loading and storing

`vle32.v` loads `vl` 32-bit elements, one after another, from memory into a vector register.
`vse32.v` stores `vl` elements from a vector register into memory.

```asm
        vle32.v v8, (a0)        # v8 = the vl words starting at address a0
        vse32.v v8, (a1)        # the vl words starting at address a1 = v8
```

The address goes in brackets, with no offset in front.
As with `sw`, the store puts the value first and the address second.

A store writes exactly `vl` elements.
Memory after them is not touched.

## Vector arithmetic

Most vector instructions come in up to three forms, depending on what the second operand is:

| Ending | Second operand                   | Example                 | Meaning, for every element `i`  |
| ------ | -------------------------------- | ----------------------- | ------------------------------- |
| `.vv`  | another vector register          | `vadd.vv v8, v8, v9`    | `v8[i] = v8[i] + v9[i]`         |
| `.vx`  | an ordinary register             | `vadd.vx v8, v8, a2`    | `v8[i] = v8[i] + a2`            |
| `.vi`  | an immediate                     | `vadd.vi v8, v8, 1`     | `v8[i] = v8[i] + 1`             |

The `x` stands for the ordinary registers such as `a2` and `t0`, whose official names are `x0` to `x31`.
A `.vx` instruction uses the same number for every element, like `a + 5` in NumPy.
A `.vi` immediate is only 5 bits, so it must be between -16 and 15.

These are the instructions you need:

| Instruction            | Meaning, for every element `i` |
| ---------------------- | ------------------------------ |
| `vadd.vv vd, vs, vt`   | `vd[i] = vs[i] + vt[i]`        |
| `vadd.vx vd, vs, rs`   | `vd[i] = vs[i] + rs`           |
| `vsub.vv vd, vs, vt`   | `vd[i] = vs[i] - vt[i]`        |
| `vsub.vx vd, vs, rs`   | `vd[i] = vs[i] - rs`           |
| `vrsub.vx vd, vs, rs`  | `vd[i] = rs - vs[i]` (reverse subtract) |
| `vmul.vv vd, vs, vt`   | `vd[i] = vs[i] * vt[i]`        |
| `vmul.vx vd, vs, rs`   | `vd[i] = vs[i] * rs`           |
| `vmv.v.x vd, rs`       | `vd[i] = rs` (copy one number into every element) |

`vrsub` exists because `.vx` always puts the ordinary register second.
To work out `10 - x` for every element, use `vrsub.vx` with 10 in an ordinary register.

There is no `vsub.vi`, just as there is no `subi`: use `vadd.vi` with a negative immediate instead.
`vadd.vi` and `vrsub.vi` exist, but `vmul` has no `.vi` form.

As with `add` and `mul`, results that are too big for 32 bits wrap around.

## The strip-mining loop

An array may be longer than one vector register, so you still need a loop.
Each time round, `vsetvli` tells you how many elements to process, and you move on by that many.
This is called **strip-mining**: the array is cut into strips, one strip per round.

Here is a function that copies an array, with a Python version for comparison:

```asm
# Copies count words from src to dest.
# dest arrives in a0, src in a1, count in a2.
copy_words:
.Lcopy_words_loop:
        blez    a2, .Lcopy_words_done   # if no elements are left, stop
        vsetvli t0, a2, e32, m1, ta, ma # t0 = vl, the elements for this round
        vle32.v v8, (a1)                # load vl elements from src
        vse32.v v8, (a0)                # store them to dest
        slli    t1, t0, 2               # t1 = vl * 4, the bytes in this round
        add     a1, a1, t1              # move src on by vl elements
        add     a0, a0, t1              # move dest on by vl elements
        sub     a2, a2, t0              # count = count - vl
        j       .Lcopy_words_loop       # go round again
.Lcopy_words_done:
        ret
```

```python
def copy_words(dest, src, count):
    start = 0
    while count > 0:
        vl = min(count, 4)              # the hardware decides; 4 is just an example
        dest[start:start + vl] = src[start:start + vl]
        start = start + vl
        count = count - vl
```

With VLEN = 128 and 10 elements, the loop usually goes round three times, with `vl` = 4, then 4, then 2.
The last round is shorter, so nothing is written past the end of `dest`.
With VLEN = 512, the same code goes round once, with `vl = 10`.

The loop has the same shape as the array loops you already know: test at the top, so that an empty array does nothing.
The differences are:

- `vsetvli` at the start of each round, to get `vl`;
- each pointer moves on by `vl * 4` bytes, not by 4;
- the count goes down by `vl`, not by 1.

To change the elements, put a vector arithmetic instruction between the load and the store.
