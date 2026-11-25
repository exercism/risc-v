# Introduction

A **shift** moves every bit of a number to the left or to the right by some number of places.
Bits that move past the end of the register are lost, and the empty places are filled in.

RISC-V has three kinds of shift.
Each comes in a register form, where the shift amount is in a register, and an immediate form, which ends in `i`:

| Register form | Immediate form | Shifts   | Fills the empty places with   |
| ------------- | -------------- | -------- | ----------------------------- |
| `sll`         | `slli`         | left     | zeros                         |
| `srl`         | `srli`         | right    | zeros                         |
| `sra`         | `srai`         | right    | copies of the sign bit        |

The names stand for **s**hift **l**eft **l**ogical, **s**hift **r**ight **l**ogical and **s**hift **r**ight **a**rithmetic.

## Shifting left

Shifting left by one place doubles a number, just as writing a 0 on the end of a decimal number multiplies it by ten.
Shifting left by `n` places multiplies by 2 to the power `n`.

```asm
li      t0, 0b0101      # t0 = 5
slli    t1, t0, 1       # t1 = 0b1010, which is 10
slli    t2, t0, 4       # t2 = 5 × 16 = 80
li      t3, 3           # t3 = 3
sll     t4, t0, t3      # t4 = 5 × 8 = 40 (the shift amount is in t3)
```

## Shifting right

Shifting right by `n` places divides by 2 to the power `n`.
Any bits shifted off the right-hand end are lost, so the answer is rounded down.

`srl` and `srli` fill the empty places on the left with zeros.
This is right for **unsigned** numbers, which are never negative.

```asm
li      t0, 100         # t0 = 100
srli    t1, t0, 2       # t1 = 100 / 4 = 25
srli    t2, t0, 3       # t2 = 12 (12.5 rounded down)
```

## Shifting right a negative number

A negative number has 1 as its top bit, called the **sign bit**.
If `srl` fills the left with zeros, a negative number becomes a large positive one.

`sra` and `srai` fill the empty places with copies of the sign bit instead, so a negative number stays negative.

```asm
li      t0, -12         # t0 = 0xFFFFFFF4
srai    t1, t0, 2       # t1 = -3
srli    t2, t0, 2       # t2 = 0x3FFFFFFD, which is 1,073,741,821
```

Arithmetic shifting right always rounds **down**, towards minus infinity.
So shifting -7 right by one place gives -4, not -3.
This is different from `div`, which rounds towards zero.

## Shift amounts

The shift amount must be between 0 and 31.
For an immediate, the assembler checks this for you.
For a register, the processor uses only the lowest 5 bits of it, so a shift by 33 is the same as a shift by 1.

## Comparing with Python

Python's `<<` and `>>` are shifts too.

```python
5 << 4      # 80
100 >> 3    # 12
-12 >> 2    # -3
```

Python's `>>` is an arithmetic shift, like `sra`.
Python has no logical right shift, because its numbers have no fixed size.

## Bit fields

A single 32-bit register can hold several small numbers side by side.
Each small number is called a **field**, and takes up a group of bits.

For example, a time of day could be stored with the minutes in bits 0 to 5 and the hours in bits 6 to 10:

```text
bit:   10 ...  6   5 ...  0
      [  hours  ] [ minutes ]
```

To **extract** a field, shift it down to bit 0, then use `andi` with a mask to clear the bits above it.

```asm
srli    t0, a0, 6       # move the hours down to bits 0 to 4
andi    t0, t0, 0x1F    # keep only those 5 bits
```

To **pack** fields together, shift each field up to its place, then combine them with `or`.

```asm
slli    t0, a0, 6       # move the hours up to bits 6 to 10
or      t0, t0, a1      # put the minutes in bits 0 to 5
```

In Python, the same steps are `(time >> 6) & 0x1F` and `(hours << 6) | minutes`.
