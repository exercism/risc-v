# Introduction

In the vectors concept, every result was a whole vector: one answer for each element.
Often you want just **one** answer for a whole array instead, such as its total, its smallest element or its largest element.
Turning many elements into one value is called a **reduction**.

Python has reductions built in:

```python
total = sum(laps)
fastest = min(laps)
slowest = max(laps)
```

With NumPy, you write `laps.sum()`, `laps.min()` and `laps.max()`.

The vector extension has reduction instructions that do the same job for the elements of a vector register.

## `vredsum.vs`: add up a vector

```asm
        vredsum.vs v9, v8, v9   # v9[0] = v9[0] + v8[0] + v8[1] + ... + v8[vl-1]
```

A reduction instruction takes three vector registers:

| Operand | In the example | Meaning                                                   |
| ------- | -------------- | --------------------------------------------------------- |
| `vd`    | `v9`           | the result goes into element 0 of this register           |
| `vs2`   | `v8`           | the vector to reduce: its first `vl` elements             |
| `vs1`   | `v9`           | element 0 of this register is the **seed**, the starting value |

So `vredsum.vs vd, vs2, vs1` works out `vd[0] = vs1[0] + vs2[0] + vs2[1] + ... + vs2[vl-1]`.
Only element 0 of `vs1` is used, and only element 0 of `vd` is written.
The ending `.vs` means "a **v**ector and a **s**ingle element".

The seed is like the starting value of an accumulator in a loop.
It is also the second argument of Python's `sum`: `sum(values, 100)` starts counting from 100.

As with `vadd`, a sum that is too big for 32 bits wraps around.

## Moving one element: `vmv.s.x` and `vmv.x.s`

The seed must be in element 0 of a vector register, but your starting value is usually in an ordinary register.
The result is in element 0 of a vector register, but a function returns its result in `a0`.
Two instructions move a number between element 0 and an ordinary register:

```asm
        vmv.s.x v9, t2          # v9[0] = t2 (the other elements are not used)
        vmv.x.s t2, v9          # t2 = v9[0]
```

Read the names from left to right as "destination, source": `s.x` copies an ordinary (**x**) register into a **s**ingle element, and `x.s` copies a single element into an ordinary register.

`vmv.s.x` only writes when `vl` is at least 1, so use it after a `vsetvli`, inside the loop.

## Reducing in a strip-mining loop

A long array does not fit in one vector register, so you reduce it one strip at a time, in a strip-mining loop.
Keep the answer so far in an ordinary register.
In each round, use it as the seed, and read the new answer back:

```asm
# Returns the sum of the squares of the count words at values.
# values arrives in a0, count in a1.
sum_of_squares:
        li      t2, 0                   # t2 = the total so far
.Lsum_of_squares_loop:
        blez    a1, .Lsum_of_squares_done   # if no elements are left, stop
        vsetvli t0, a1, e32, m1, ta, ma # t0 = vl, the elements for this round
        vle32.v v8, (a0)                # load vl elements
        vmul.vv v8, v8, v8              # square each one
        vmv.s.x v9, t2                  # v9[0] = the total so far, as the seed
        vredsum.vs v9, v8, v9           # v9[0] = seed + the sum of this round's squares
        vmv.x.s t2, v9                  # t2 = the new total so far
        slli    t1, t0, 2               # t1 = vl * 4, the bytes in this round
        add     a0, a0, t1              # move on by vl elements
        sub     a1, a1, t0              # count = count - vl
        j       .Lsum_of_squares_loop   # go round again
.Lsum_of_squares_done:
        mv      a0, t2                  # return the total
        ret
```

```python
def sum_of_squares(values, count):
    total = 0
    start = 0
    while count > 0:
        vl = min(count, 4)              # the hardware decides; 4 is just an example
        total = sum([v * v for v in values[start:start + vl]], total)
        start = start + vl
        count = count - vl
    return total
```

The result of each round is the seed of the next round, so the answer carries from one strip to the next.
If the array is empty, the loop never runs, and the function returns the starting value.

## Smallest and largest

These reductions work in the same way as `vredsum.vs`:

| Instruction              | Result in `vd[0]`                                       |
| ------------------------ | ------------------------------------------------------- |
| `vredsum.vs vd, vs2, vs1`  | `vs1[0]` plus the sum of the elements of `vs2`          |
| `vredmin.vs vd, vs2, vs1`  | the smallest of `vs1[0]` and the elements of `vs2`      |
| `vredmax.vs vd, vs2, vs1`  | the largest of `vs1[0]` and the elements of `vs2`       |
| `vredminu.vs vd, vs2, vs1` | the smallest, treating every element as unsigned        |
| `vredmaxu.vs vd, vs2, vs1` | the largest, treating every element as unsigned         |

For a minimum or maximum, the seed must not change the answer.
There are two common choices:

- Use the first element of the array as the seed.
  This only works when the array has at least one element.
- Use a number that every element beats.
  For the largest `int32_t`, start from the smallest possible one, -2147483648 (`0x80000000`).
  For the smallest, start from the largest possible one, 2147483647 (`0x7FFFFFFF`).

Do not start a maximum from 0: if every element is negative, 0 would win.

## Signed and unsigned

So far, every word has been **signed**: when the top bit is 1, the number is negative.
A C `uint32_t` is **unsigned**: it is never negative, and holds numbers from 0 to 4294967295.
With unsigned numbers, a word whose top bit is 1 is a **big** number, not a negative one.

The same 32 bits can mean two different numbers:

| Bits         | As `int32_t` | As `uint32_t` |
| ------------ | ------------ | ------------- |
| `0x00000005` | 5            | 5             |
| `0x7FFFFFFF` | 2147483647   | 2147483647    |
| `0x80000000` | -2147483648  | 2147483648    |
| `0xFFFFFFFF` | -1           | 4294967295    |

Adding works the same way for both, so there is only one `vredsum.vs`.
Comparing does not: `vredmax.vs` thinks `0xFFFFFFFF` is -1, smaller than 5, while `vredmaxu.vs` thinks it is 4294967295, larger than 5.
The `u` at the end of `vredminu` and `vredmaxu` means unsigned.
Use them for arrays of `uint32_t`.

For unsigned numbers, the smallest possible value is 0, and the largest is `0xFFFFFFFF`.
