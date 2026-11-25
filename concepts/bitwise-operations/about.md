# About

## Binary and hexadecimal

A register holds 32 bits, numbered from bit 0 on the right to bit 31 on the left.
Bit *n* is worth 2 to the power *n*: bit 0 is worth 1, bit 1 is worth 2, bit 2 is worth 4, and so on.

The assembler accepts numbers in several bases:

| Written as   | Base        | Example      | Value |
| ------------ | ----------- | ------------ | ----- |
| `0b...`      | binary      | `0b101101`   | 45    |
| `0x...`      | hexadecimal | `0x2D`       | 45    |
| plain digits | decimal     | `45`         | 45    |

Each hex digit stands for exactly four bits, so a 32-bit register is always eight hex digits.
That makes hex the usual way to write masks and bit patterns.
Hex digits may be written in upper or lower case: `0x2D` and `0x2d` are the same.

## The bitwise instructions

| Instruction          | Meaning             | Python      |
| -------------------- | ------------------- | ----------- |
| `and rd, rs1, rs2`   | `rd = rs1 & rs2`    | `a & b`     |
| `andi rd, rs1, imm`  | `rd = rs1 & imm`    | `a & 5`     |
| `or rd, rs1, rs2`    | `rd = rs1 \| rs2`   | `a \| b`    |
| `ori rd, rs1, imm`   | `rd = rs1 \| imm`   | `a \| 5`    |
| `xor rd, rs1, rs2`   | `rd = rs1 ^ rs2`    | `a ^ b`     |
| `xori rd, rs1, imm`  | `rd = rs1 ^ imm`    | `a ^ 5`     |

Each bit of the result depends only on the bits in the same position of the inputs.
Unlike `add`, nothing carries from one position to the next.

Some useful facts:

- `x & 0` is 0, and `x & -1` is `x`.
- `x | 0` is `x`, and `x | -1` is -1.
- `x ^ 0` is `x`, and `x ^ x` is 0.
- `xor` undoes itself: `(x ^ m) ^ m` is `x` again.

## Masks

A **mask** is a value whose set bits select positions:

| Goal                           | Instruction            |
| ------------------------------ | ---------------------- |
| keep only the masked bits      | `and` with the mask    |
| set the masked bits to 1       | `or` with the mask     |
| clear the masked bits to 0     | `and` with `~mask`     |
| flip the masked bits           | `xor` with the mask    |

A mask for the lowest *n* bits is 2 to the power *n*, minus 1.
For example, `0xF` selects bits 0 to 3, and `0xFF` selects bits 0 to 7.

## Pseudo-instructions

| You write      | The assembler produces    | Meaning                  |
| -------------- | ------------------------- | ------------------------ |
| `not rd, rs`   | `xori rd, rs, -1`         | flip every bit (`~rs`)   |
| `neg rd, rs`   | `sub rd, zero, rs`        | negate (`-rs`)           |

In two's complement, the way RISC-V stores negative numbers, `-x` equals `~x + 1`.
So `not` and `neg` always give results that differ by exactly 1.

There is no `nand`, `nor` or `xnor` in the base instruction set.
You build them from `not` and the other instructions.
The Zbb extension, covered later on this track, adds `andn`, `orn` and `xnor`.

## Immediates and sign extension

The immediate in `andi`, `ori` and `xori` is 12 bits, between -2048 and 2047.
Before it is used, it is **sign-extended**: bit 11 of the immediate is copied into bits 12 to 31.

| Immediate | 32-bit value used |
| --------- | ----------------- |
| `0x7FF`   | `0x000007FF`      |
| `-1`      | `0xFFFFFFFF`      |
| `-256`    | `0xFFFFFF00`      |

This means `andi` can keep the low bits (`andi a0, a0, 0x7FF`), or clear a few low bits (`andi a0, a0, -256` clears bits 0 to 7).
It cannot use a mask like `0xFFF` or `0xFF00`, which the assembler rejects as out of range.
For those, load the mask into a register with `li` and use the register form.

## Signed and unsigned

The bitwise instructions do not care whether a number is signed or unsigned.
They work on the 32 bits only.
In C the test file uses `uint32_t`, an unsigned 32-bit integer, because bit patterns are usually thought of as unsigned.
