# Introduction

A register holds 32 bits.
Each bit is a 0 or a 1.
The same 32 bits can be read as a number in two different ways: **signed** or **unsigned**.

## Writing numbers in hexadecimal

Numbers in this concept are often written in **hexadecimal** (base 16), because it shows the bits more clearly than decimal does.
A hexadecimal number starts with `0x`.
Its digits are `0` to `9`, then `A` to `F` for 10 to 15.
Each hexadecimal digit stands for exactly 4 bits, so a 32-bit register is 8 hexadecimal digits.

| Hexadecimal  | Decimal        |
| ------------ | -------------- |
| `0x10`       | 16             |
| `0xFF`       | 255            |
| `0x7FFFFFFF` | 2147483647     |
| `0x80000000` | 2147483648     |
| `0xFFFFFFFF` | 4294967295     |

Python and C both understand this notation:

```python
print(0xFF)         # 255
print(0xFFFFFFFF)   # 4294967295
```

## Signed and unsigned

An **unsigned** number is never negative.
In 32 bits, it can be anything from 0 to 4294967295 (that is, 2³² - 1).

A **signed** number can be negative.
RISC-V, like almost every processor, stores signed numbers in **two's complement**.
In 32 bits, a signed number can be anything from -2147483648 to 2147483647.
The numbers from 0 to `0x7FFFFFFF` mean the same thing either way.
For the bit patterns from `0x80000000` to `0xFFFFFFFF`, the signed value is the unsigned value minus 2³².

| Bits         | As unsigned | As signed   |
| ------------ | ----------- | ----------- |
| `0x00000005` | 5           | 5           |
| `0x7FFFFFFF` | 2147483647  | 2147483647  |
| `0x80000000` | 2147483648  | -2147483648 |
| `0xFFFFFFFE` | 4294967294  | -2          |
| `0xFFFFFFFF` | 4294967295  | -1          |

In Python you can turn one into the other like this:

```python
bits = -1 % 2**32                                   # 4294967295, the unsigned value
signed = bits - 2**32 if bits >= 2**31 else bits    # -1, the signed value
```

## The instruction decides

A register does not know whether it holds a signed or an unsigned number.
It just holds 32 bits.
The **instruction** decides how to read them.

In C, the type tells the compiler which instructions to use.
`int32_t` is a signed 32-bit number, and `uint32_t` is an unsigned one.
The functions in this exercise mostly take and return `uint32_t`.

## Adding and subtracting wrap around

In Python, whole numbers can grow as big as you like.
A register cannot: when a result does not fit in 32 bits, it **wraps around**.

```asm
        li      t0, 0xFFFFFFFF          # t0 = 4294967295 (or -1, if signed)
        add     t0, t0, t0              # t0 = 0xFFFFFFFE: 8589934590 is too big, so it wraps
        li      t1, 0                   # t1 = 0
        addi    t1, t1, -1              # t1 = 0xFFFFFFFF: 0 - 1 wraps to 4294967295
```

Wrapping around gives exactly the same bits whether you think of the numbers as signed or unsigned.
So there is only one `add`, one `sub` and one `addi`.
The answer is always the true answer, wrapped to 32 bits.
In Python, you get the same result with `% 2**32`:

```python
(0xFFFFFFFF + 0xFFFFFFFF) % 2**32   # 4294967294
(0 - 1) % 2**32                     # 4294967295
(4 - 0xFFFFFFFC) % 2**32            # 8
```

The last line shows something useful.
Subtracting unsigned numbers gives the right distance from one to the other, even if the count wrapped around to 0 in between.

## Unsigned comparisons

Comparing is different.
Is `0xFFFFFFFF` bigger than 1?
As unsigned, yes: 4294967295 > 1.
As signed, no: -1 < 1.
So RISC-V has signed and unsigned versions of each comparison.
The unsigned ones end in `u`.

| Signed            | Unsigned            | Meaning                             |
| ----------------- | ------------------- | ----------------------------------- |
| `blt  a, b, label`| `bltu a, b, label`  | jump if `a < b`                     |
| `bge  a, b, label`| `bgeu a, b, label`  | jump if `a >= b`                    |
| `bgt  a, b, label`| `bgtu a, b, label`  | jump if `a > b` (pseudo-instruction)  |
| `ble  a, b, label`| `bleu a, b, label`  | jump if `a <= b` (pseudo-instruction) |
| `slt  d, a, b`    | `sltu  d, a, b`     | `d = 1` if `a < b`, else 0          |
| `slti d, a, imm`  | `sltiu d, a, imm`   | `d = 1` if `a < imm`, else 0        |
| `sgt  d, a, b`    | `sgtu  d, a, b`     | `d = 1` if `a > b`, else 0 (pseudo-instruction) |

`beq` and `bne` have no unsigned version, because two numbers are equal exactly when their bits are equal.

```asm
        li      t0, -1                  # t0 = 0xFFFFFFFF
        li      t1, 1                   # t1 = 1
        slt     t2, t0, t1              # t2 = 1, because -1 < 1
        sltu    t3, t0, t1              # t3 = 0, because 4294967295 is not < 1
```

`sltiu` takes a small immediate, from -2048 to 2047, but compares it as unsigned.
`sltiu d, a, 1` is 1 only when `a` is 0, because 0 is the only unsigned number less than 1.
That is exactly what `seqz d, a` does, and the assembler turns `seqz` into this `sltiu`.

## Checking a range with one comparison

To check that a number is between `low` and `high`, you might use two comparisons: `low <= x` and `x <= high`.
With unsigned numbers you need only one.

Subtract `low` first.
If `x` was in the range, the result is between 0 and `high - low`.
If `x` was too small, the subtraction wraps around to a huge unsigned number.
So one unsigned comparison catches both "too small" and "too big".

Here is a check that an age is a teenage age, from 13 to 19:

```python
def is_teen(age):
    return 13 <= age <= 19
```

```asm
is_teen:
        addi    t0, a0, -13             # t0 = age - 13, huge if age < 13
        li      t1, 7                   # 7 ages: 13, 14, ..., 19
        bltu    t0, t1, .Lis_teen_yes   # if 0 <= age - 13 < 7, it is a teen age
        li      a0, 0                   # not a teenager
        ret
.Lis_teen_yes:
        li      a0, 1                   # a teenager
        ret
```

The number to compare with is `high - low + 1`, the count of values in the range.
When you want a 0 or 1 answer and that count fits in an immediate, `sltiu` does the comparison with no branch at all.

## Unsigned division

You met `divu` and `remu` alongside `div` and `rem`.
They read their operands as unsigned:

```asm
        li      t0, 0xFFFFFFFE          # 4294967294, or -2 if signed
        li      t1, 2
        div     t2, t0, t1              # t2 = -1, because -2 / 2 = -1
        divu    t3, t0, t1              # t3 = 2147483647, because 4294967294 / 2
        remu    t4, t0, t1              # t4 = 0
```

For numbers from 0 to `0x7FFFFFFF`, `div` and `divu` agree.
For bigger unsigned numbers, only `divu` and `remu` give the right answer.

Dividing by zero does not stop the program:

- `divu` by zero gives `0xFFFFFFFF`, the biggest unsigned number;
- `remu` by zero gives the dividend, unchanged.
