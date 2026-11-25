# About

A **shift** moves the bits of a register left or right.
Bits shifted past either end are discarded, and the places left empty are filled in.

## The shift instructions

| Instruction        | Meaning                                                  |
| ------------------ | -------------------------------------------------------- |
| `sll rd, rs1, rs2` | shift `rs1` left by `rs2`, filling with zeros            |
| `srl rd, rs1, rs2` | shift `rs1` right by `rs2`, filling with zeros           |
| `sra rd, rs1, rs2` | shift `rs1` right by `rs2`, filling with the sign bit    |
| `slli rd, rs1, n`  | shift `rs1` left by the immediate `n`                    |
| `srli rd, rs1, n`  | shift `rs1` right by `n`, filling with zeros             |
| `srai rd, rs1, n`  | shift `rs1` right by `n`, filling with the sign bit      |

There is no "shift left arithmetic".
Shifting left fills the right-hand end, and there is only one sensible thing to fill it with: zeros.

## Shift amounts

On RV32, an immediate shift amount must be between 0 and 31, and the assembler rejects anything else.

A register shift amount uses only the lowest 5 bits of the register, which is the same as taking the amount modulo 32.
So shifting by 32 leaves the value unchanged, and shifting by -1 shifts by 31.

This differs from Python, where `1 << 40` is simply 1,099,511,627,776, and a negative shift amount raises an error.
In C, shifting a 32-bit value by 32 or more is undefined behaviour, so portable code never relies on either result.

## Multiplying and dividing by powers of two

`slli rd, rs, n` multiplies by 2ⁿ, wrapping around if the result does not fit, just like `mul`.
A shift is often faster than a multiplication, and it is available on processors without the M extension.

For **unsigned** values, `srli rd, rs, n` divides by 2ⁿ, rounding down, and gives the same result as `divu`.

For **signed** values, `srai rd, rs, n` divides by 2ⁿ rounding towards minus infinity, whereas `div` rounds towards zero.
The two agree for non-negative values and for exact divisions, but differ for negative values that are not exact:

| Value | `srai` by 1 | `div` by 2 |
| ----- | ----------- | ---------- |
| 7     | 3           | 3          |
| -8    | -4          | -4         |
| -7    | -4          | -3         |
| -1    | -1          | 0          |

Shifting -1 right arithmetically by any amount gives -1, because every bit is already a copy of the sign bit.

Python's `>>` behaves like `srai`: `-7 >> 1` is -4, matching Python's `-7 // 2`.

## Choosing logical or arithmetic

Use the logical shift `srl` when the bits represent an unsigned number or a pattern of flags or fields.
Use the arithmetic shift `sra` when the bits represent a signed number and you want to keep its sign.

## Bit fields

Packing several small values into one register or word saves space, and is common in hardware registers, file formats and network packets.

To read a field of width `w` that starts at bit `p`:

```asm
srli    t0, a0, p       # move the field down to bit 0
andi    t0, t0, mask    # mask = 2^w - 1, keeping only the field
```

If the field is the top field of the word, the `andi` is not needed, because `srli` has already filled the bits above it with zeros.
`andi` takes a 12-bit signed immediate, so for a mask wider than 11 bits, load it into a register with `li` and use `and`.

A signed field at the top of the word can be read with `srai` instead, which keeps its sign.
A signed field elsewhere can be read by first shifting it left until its top bit reaches bit 31, then shifting it right arithmetically:

```asm
slli    t0, a0, 32 - p - w   # field's top bit to bit 31
srai    t0, t0, 32 - w       # field down to bit 0, sign-extended
```

To write fields into a word, shift each one into place and combine them with `or`.
This only works if each value fits in its field: a value that is too large spills into the field next to it.
