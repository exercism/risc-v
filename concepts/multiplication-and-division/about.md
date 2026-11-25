# About

Multiplication and division are not part of the base RISC-V instruction set.
They come from the **M extension**, an optional add-on that almost every RISC-V processor for Linux includes.
A processor described as `rv32im` or `rv32gc` has it.

## The instructions

| Instruction          | Result                                                           |
| -------------------- | ---------------------------------------------------------------- |
| `mul rd, rs1, rs2`    | the low 32 bits of `rs1 * rs2`                                   |
| `mulh rd, rs1, rs2`   | the high 32 bits of `rs1 * rs2`, both signed                     |
| `mulhu rd, rs1, rs2`  | the high 32 bits of `rs1 * rs2`, both unsigned                   |
| `mulhsu rd, rs1, rs2` | the high 32 bits of `rs1 * rs2`, `rs1` signed and `rs2` unsigned |
| `div rd, rs1, rs2`    | `rs1 / rs2`, signed, truncated towards zero                      |
| `divu rd, rs1, rs2`   | `rs1 / rs2`, unsigned                                            |
| `rem rd, rs1, rs2`    | the remainder of `rs1 / rs2`, signed                             |
| `remu rd, rs1, rs2`   | the remainder of `rs1 / rs2`, unsigned                           |

None of them has an immediate form.
A constant multiplier or divisor must be loaded into a register with `li` first.

## Multiplication and overflow

Multiplying two 32-bit numbers can give an answer up to 64 bits long.
`mul` returns only the low 32 bits, so a result that does not fit wraps around with no error:

```asm
li      t0, 65536       # t0 = 2 to the power 16
mul     t1, t0, t0      # t1 = 0, because 2 to the power 32 does not fit
```

The low 32 bits are the same whether the operands are signed or unsigned, so there is no `mulu`.

To get the full 64-bit product, use `mulh` (or `mulhu` for unsigned numbers) for the high half and `mul` for the low half.
Writing the two instructions next to each other, with the same source registers, lets some processors do the work only once.
Later exercises on 64-bit integers use this.

## Division rounds towards zero

`div` discards the fractional part of the quotient, which moves it towards zero.
`rem` is then defined so that

```
dividend = quotient * divisor + remainder
```

which means the remainder has the same sign as the dividend.

| `a`  | `b`  | `div a, b` | `rem a, b` | Python `a // b` | Python `a % b` |
| ---- | ---- | ---------- | ---------- | --------------- | -------------- |
| 7    | 2    | 3          | 1          | 3               | 1              |
| -7   | 2    | -3         | -1         | -4              | 1              |
| 7    | -2   | -3         | 1          | -4              | -1             |
| -7   | -2   | 3          | -1         | 3               | -1             |

This matches C, Java, JavaScript's `Math.trunc(a / b)` and `a % b`, and Python's `int(a / b)` and `math.fmod(a, b)`.
Python's own `//` and `%` round towards minus infinity instead.

## No exceptions

RISC-V division never stops the program.
The special cases give fixed answers:

| Case                                | `div` / `divu`        | `rem` / `remu`   |
| ----------------------------------- | --------------------- | ---------------- |
| divide by zero                      | -1 (all 32 bits set)  | the dividend     |
| `div` of -2,147,483,648 by -1       | -2,147,483,648        | 0                |

The second row is **signed overflow**: the true answer, 2,147,483,648, is one more than the largest signed 32-bit number, so it wraps around.

If a program needs to treat division by zero as an error, it must check the divisor itself, before dividing.

## Cost

On many small processors, `mul` takes a few cycles, while `div` and `rem` can take 30 or more.
Compilers therefore replace division by a constant with a multiplication and shifts where they can.
When you need both the quotient and the remainder, write `div` immediately followed by `rem` with the same operands: some processors notice this and do the division only once.
