# Introduction

## Binary numbers

Inside a register, a number is stored as 32 **bits**.
A bit is a single digit that is either 0 or 1.
Writing a number with bits is called **binary**.

In everyday numbers, each place is worth ten times the place to its right: ones, tens, hundreds.
In binary, each place is worth **two** times the place to its right: 1, 2, 4, 8, 16, and so on.

| Place value | 8 | 4 | 2 | 1 |
| ----------- | - | - | - | - |
| Bit         | 1 | 1 | 0 | 1 |

So binary `1101` is 8 + 4 + 1 = 13.

We number the bits from the right, starting at 0.
Bit 0 is worth 1, bit 1 is worth 2, bit 2 is worth 4, and so on.
When a bit is 1 we say it is **set**, and when it is 0 we say it is **clear**.

In assembly, as in Python, you write a binary number with `0b` in front:

```asm
li      t0, 0b1101      # t0 = 13
```

## Hexadecimal

Long rows of bits are hard to read, so we often write them in **hexadecimal** (or "hex") instead.
Hex has sixteen digits: `0` to `9`, then `A` to `F` for ten to fifteen.
Each hex digit stands for exactly four bits:

| Hex  | Bits   | Hex  | Bits   | Hex  | Bits   | Hex  | Bits   |
| ---- | ------ | ---- | ------ | ---- | ------ | ---- | ------ |
| `0`  | `0000` | `4`  | `0100` | `8`  | `1000` | `C`  | `1100` |
| `1`  | `0001` | `5`  | `0101` | `9`  | `1001` | `D`  | `1101` |
| `2`  | `0010` | `6`  | `0110` | `A`  | `1010` | `E`  | `1110` |
| `3`  | `0011` | `7`  | `0111` | `B`  | `1011` | `F`  | `1111` |

You write a hex number with `0x` in front.
For example, `0x3C` is `0011 1100` in binary, which is 60.

```asm
li      t0, 0x3C        # t0 = 0b00111100 = 60
```

## Bitwise instructions

A **bitwise** instruction works on each bit position separately.
Bit 0 of the result depends only on bit 0 of the inputs, bit 1 only on bit 1, and so on.
No bit ever carries over into its neighbour, as it can when you add.

| `a` | `b` | `and` | `or` | `xor` |
| --- | --- | ----- | ---- | ----- |
| 0   | 0   | 0     | 0    | 0     |
| 0   | 1   | 0     | 1    | 1     |
| 1   | 0   | 0     | 1    | 1     |
| 1   | 1   | 1     | 1    | 0     |

- `and` gives 1 only where **both** bits are 1.
- `or` gives 1 where **either** bit is 1, or both are.
- `xor` ("exclusive or") gives 1 where the two bits are **different**.

Each one has a register form and an immediate form:

```asm
and     a0, a1, a2      # a0 = a1 & a2
andi    a0, a1, 0b11    # a0 = a1 & 0b11
or      a0, a1, a2      # a0 = a1 | a2
ori     a0, a1, 0x40    # a0 = a1 | 0x40
xor     a0, a1, a2      # a0 = a1 ^ a2
xori    a0, a1, 0b101   # a0 = a1 ^ 0b101
```

Python has the same operators:

```python
0b1100 & 0b1010   # => 0b1000
0b1100 | 0b1010   # => 0b1110
0b1100 ^ 0b1010   # => 0b0110
```

## Masks

A **mask** is a number whose set bits pick out the bit positions you care about.
The instruction you choose decides what happens at those positions:

- `and` with a mask **keeps** the bits under the mask's 1s, and clears all the others.
- `or` with a mask **sets** the bits under the mask's 1s, and leaves the others alone.
- `xor` with a mask **flips** the bits under the mask's 1s (0 becomes 1, 1 becomes 0), and leaves the others alone.

```asm
li      t0, 0b0110      # t0 = 0b0110
andi    t1, t0, 0b0011  # t1 = 0b0010  (kept bits 0 and 1)
ori     t2, t0, 0b0011  # t2 = 0b0111  (set bits 0 and 1)
xori    t3, t0, 0b0011  # t3 = 0b0101  (flipped bits 0 and 1)
```

## `not` and `neg`

The `not` pseudo-instruction flips **every** bit of a register.
It is the same as Python's `~` operator.

```asm
not     a0, a1          # a0 = ~a1
```

The assembler turns `not a0, a1` into `xori a0, a1, -1`.
That works because -1 is stored as 32 bits that are all 1, so `xor` with it flips them all.

Do not mix up `not` with the `neg` pseudo-instruction.
`neg a0, a1` gives the negative number, `0 - a1`, and the assembler turns it into `sub a0, zero, a1`.
For any number `x`, `neg` gives one more than `not`: `-x` is the same as `~x + 1`.

## Immediates are only 12 bits

As with `addi`, the immediate in `andi`, `ori` and `xori` must fit in 12 bits.
It must be between -2048 and 2047, so the largest positive mask is `0x7FF`.

Before the instruction uses the immediate, the processor copies its top bit (bit 11) into all of bits 12 to 31.
This is called **sign extension**.
It is why -1 becomes 32 set bits.

A mask such as `0xFFF` is too big, and the assembler will reject it.
When you need a bigger mask, load it with `li` first and use the register form:

```asm
li      t0, 0xFF00      # too big for an immediate
and     a0, a0, t0      # keep bits 8 to 15 of a0
```
