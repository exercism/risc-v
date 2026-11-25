# Introduction

So far, every value in memory has been a 32-bit word.
Many values are small, and do not need 4 bytes each.
Using smaller elements means a big array takes less memory.

## Halfwords

A **halfword** is 16 bits, which is 2 bytes: half of a word.
C has two halfword types:

| C type     | Smallest value | Largest value |
| ---------- | -------------- | ------------- |
| `int16_t`  | -32768         | 32767         |
| `uint16_t` | 0              | 65535         |

An `int16_t` can be negative.
Like a 32-bit number, it uses its top bit, bit 15, as the **sign bit**.
A `uint16_t` is never negative, so it can use all 16 bits for bigger positive numbers.

The same 16 bits can mean two different numbers, depending on the type.
For example, the bits `0xFF38` mean -200 as an `int16_t`, but 65336 as a `uint16_t`.

Python's `array` module can store numbers in exactly these sizes.
Type code `'h'` is like `int16_t`, and `'H'` is like `uint16_t`:

```python
from array import array

temps = array('h', [-200, 150])    # each element takes 2 bytes
depths = array('H', [65336, 900])  # each element takes 2 bytes
```

## Arrays of halfwords

In an `int16_t` or `uint16_t` array, the element size is 2.
So element `i` starts `2 * i` bytes after element 0:

| Element     | Address        |
| ----------- | -------------- |
| `values[0]` | `values`       |
| `values[1]` | `values + 2`   |
| `values[2]` | `values + 4`   |
| `values[i]` | `values + 2*i` |

Multiplying by 2 is the same as shifting left by 1.
When you walk a pointer through a halfword array, add 2 to move to the next element, and work out the end pointer as `values + 2 * count`.

A halfword's address must be a multiple of 2.
This is its **alignment**.
The arrays the tests pass you are always aligned, and steps of 2 keep them aligned.

## Loading a halfword

A register always holds 32 bits.
When you load 16 bits into it, the processor must decide what to put in the top 16 bits.
There are two load instructions, and they decide differently:

| Instruction | Name                      | Fills bits 16 to 31 with | Use it for |
| ----------- | ------------------------- | ------------------------ | ---------- |
| `lh`        | load halfword             | copies of bit 15         | `int16_t`  |
| `lhu`       | load halfword, unsigned   | zeros                    | `uint16_t` |

Copying the sign bit into the upper bits is called **sign extension**.
You met it before, when the processor sign-extends a 12-bit immediate.
It keeps a negative number negative: -200 as 16 bits becomes -200 as 32 bits.

Filling the upper bits with zeros is called **zero extension**.
It treats the 16 bits as a number from 0 to 65535, so the result is never negative.

Here are both loads reading the same halfword, whose bits are `0xFF38`:

```asm
        lh      t0, 0(a0)       # t0 = 0xFFFFFF38, which is -200
        lhu     t1, 0(a0)       # t1 = 0x0000FF38, which is 65336
```

Choose the load that matches the type in the C declaration.
`lh` on a `uint16_t` turns every value from 32768 up into a negative number.
`lhu` on an `int16_t` turns every negative value into a large positive number.

As with `lw`, the address is `offset(register)`.
For a fixed index, the offset is 2 times the index:

```asm
        lh      t0, 6(a0)       # t0 = values[3], at address a0 + 6
```

## Storing a halfword

`sh` (store halfword) writes the **low 16 bits** of a register to memory.
It changes only those 2 bytes, and leaves the bytes next to them alone.
There is only one store, because storing does not need to know about signs.

```asm
        sh      t0, 2(a0)       # values[1] = the low 16 bits of t0
```

If the number in the register is too big for 16 bits, the upper bits are simply lost.
This is called **truncation**, and there is no error.
For example, 70000 is `0x11170`, so `sh` stores `0x1170`, which is 4464.

Here is a function that halves every element of an `int16_t` array in place:

```asm
# Halves every element of values, rounding down.
# values arrives in a0, count in a1.
halve_all:
        slli    t0, a1, 1       # t0 = 2 * count
        add     t0, a0, t0      # t0 = end = values + 2 * count
halve_all_loop:
        beq     a0, t0, halve_all_done  # if the pointer has reached the end, stop
        lh      t1, 0(a0)       # t1 = the current element, sign-extended
        srai    t1, t1, 1       # t1 = t1 / 2, rounded down
        sh      t1, 0(a0)       # store it back
        addi    a0, a0, 2       # move the pointer to the next element
        j       halve_all_loop  # go back to the test
halve_all_done:
        ret
```

```python
def halve_all(values):
    for i in range(len(values)):
        values[i] = values[i] >> 1
```

## Arithmetic happens in 32 bits

There are no special instructions for adding or comparing halfwords.
Once a halfword is loaded, it is an ordinary 32-bit number in a register, and `add`, `blt` and the rest work on all 32 bits.

So a result kept in a register can grow far beyond the halfword range.
For example, adding 30000 and 30000 in registers gives 60000, which is too big for an `int16_t` but fits easily in 32 bits.
Only when you store a value back with `sh` must it fit in 16 bits again.
