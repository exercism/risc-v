# About

## Base instruction sets and extensions

RISC-V is a **modular** instruction set.
Every processor implements one **base** instruction set, and may add any number of optional **extensions**.

The bases are:

| Base     | Meaning                                                  |
| -------- | -------------------------------------------------------- |
| `RV32I`  | 32-bit registers, 32 integer registers                   |
| `RV32E`  | 32-bit registers, only 16 integer registers, for tiny chips |
| `RV64I`  | 64-bit registers                                         |

Some common extensions are:

| Name  | What it adds                                                    |
| ----- | --------------------------------------------------------------- |
| **M** | integer multiplication and division                             |
| **A** | atomic memory operations, for programs that run on several cores |
| **F** | single-precision floating point                                 |
| **D** | double-precision floating point                                 |
| **C** | compressed 16-bit encodings of common instructions               |
| **V** | vector instructions                                             |
| **B** | bit manipulation: Zba, Zbb and Zbs together                     |

**G** is short for `IMAFD` plus two small extensions (`Zicsr` and `Zifencei`), the set a general-purpose processor is expected to have.

## The arch string

A processor, or a program, describes the instructions it uses with an **arch string**.
It starts with the base, then lists single-letter extensions in a fixed order, then multi-letter extensions separated by underscores:

```
rv32imac_zba_zbb_zbs
```

Compilers take an arch string with an option such as `-march=rv32imac_zbb`.
In an assembly file, `.option arch` adds or removes extensions for the rest of the file:

```asm
.option arch, +zbb      # allow Zbb instructions
.option arch, -zbb      # no longer allow them
```

The assembler refuses an instruction from an extension that is not enabled.
That protects you from building a program that would stop with an "illegal instruction" error on a processor without that extension.

## The bit-manipulation extensions

The B extension is split into smaller parts, so that a chip designer can choose only what is needed:

| Extension | Purpose                                | Examples                      |
| --------- | -------------------------------------- | ----------------------------- |
| **Zba**   | address calculation                    | `sh1add`, `sh2add`, `sh3add`  |
| **Zbb**   | basic bit manipulation                 | `cpop`, `clz`, `min`, `rol`   |
| **Zbs**   | single-bit instructions                | `bset`, `bclr`, `binv`, `bext` |
| **Zbc**   | carry-less multiplication, for checksums and cryptography | `clmul` |

Zbs is a natural partner for bit sets: `bset`, `bclr`, `binv` and `bext` add, remove, toggle and test one member without building a mask first.

## Zbb instructions on RV32

### Counting

| Instruction    | Result                                     | Python                       |
| -------------- | ------------------------------------------ | ---------------------------- |
| `cpop rd, rs`  | number of set bits                          | `x.bit_count()`              |
| `clz rd, rs`   | number of 0 bits above the highest set bit  | `32 - x.bit_length()`        |
| `ctz rd, rs`   | number of 0 bits below the lowest set bit   | `(x & -x).bit_length() - 1`  |

`clz` and `ctz` both give 32 for 0.
The Python expressions in the table give 32 for `clz` of 0, but -1 for `ctz` of 0.

For a non-empty bit set:

- the smallest member is `ctz`;
- the largest member is `31 - clz`;
- the number of members is `cpop`.

`32 - clz` is the number of bits needed to write an unsigned number, which is useful for rounding up to a power of two or for working out a logarithm base 2.

### Logic with an inverted operand

| Instruction         | Result          |
| ------------------- | --------------- |
| `andn rd, rs1, rs2` | `rs1 & ~rs2`    |
| `orn rd, rs1, rs2`  | `rs1 \| ~rs2`   |
| `xnor rd, rs1, rs2` | `~(rs1 ^ rs2)`  |

`andn` is the set difference `s - t` in one instruction, replacing `not` and `and`.

### Minimum and maximum

| Instruction          | Result                     |
| -------------------- | -------------------------- |
| `min rd, rs1, rs2`   | smaller value, signed      |
| `max rd, rs1, rs2`   | larger value, signed       |
| `minu rd, rs1, rs2`  | smaller value, unsigned    |
| `maxu rd, rs1, rs2`  | larger value, unsigned     |

They replace a compare and a branch, and so they run at the same speed whatever the values are.
Clamping a value into a range takes two of them:

```asm
        max     t0, t0, t1      # no smaller than t1
        min     t0, t0, t2      # no larger than t2
```

The absolute value of a signed number takes two instructions:

```asm
        neg     t1, t0          # t1 = -t0
        max     t1, t1, t0      # t1 = |t0|
```

### Rotates

| Instruction           | Result                                 |
| --------------------- | -------------------------------------- |
| `rol rd, rs1, rs2`    | `rs1` rotated left by `rs2 & 31`       |
| `ror rd, rs1, rs2`    | `rs1` rotated right by `rs2 & 31`      |
| `rori rd, rs1, shamt` | `rs1` rotated right by `shamt`, 0 to 31 |

There is no `roli`: rotating left by `n` is the same as rotating right by `32 - n`.
Rotating left by a negative amount is the same as rotating right, because only the lowest 5 bits of the amount count.

Without Zbb, a rotate left by the amount in `a1` takes four instructions:

```asm
        sll     t0, a0, a1      # bits that move up
        neg     t1, a1          # 32 - a1, in the lowest 5 bits
        srl     t1, a0, t1      # bits that come round to the bottom
        or      a0, t0, t1
```

Rotates are common in hash functions, random number generators and cryptography.

### Bytes

| Instruction      | Result                                                         |
| ---------------- | -------------------------------------------------------------- |
| `rev8 rd, rs`    | the four bytes in reverse order                                |
| `orc.b rd, rs`   | each byte becomes `0xFF` if any of its bits is set, else `0x00` |

`rev8` turns `0x12345678` into `0x78563412`.
It converts between **little-endian** order, which RISC-V uses in memory, and **big-endian** order, which many file formats and network protocols use.

`orc.b` helps search a string four bytes at a time.
If a word loaded from a string has a zero byte, that byte of the `orc.b` result is `0x00`, so the result is not `0xFFFFFFFF`.
`orc.b` of `0x00120300` is `0x00FFFF00`.

### Sign and zero extension

| Instruction      | Result                                      | Replaces                        |
| ---------------- | ------------------------------------------- | ------------------------------- |
| `sext.b rd, rs`  | bits 0–7 of `rs`, sign-extended to 32 bits   | `slli 24` then `srai 24`        |
| `sext.h rd, rs`  | bits 0–15 of `rs`, sign-extended to 32 bits  | `slli 16` then `srai 16`        |
| `zext.h rd, rs`  | bits 0–15 of `rs`, zero-extended to 32 bits  | `slli 16` then `srli 16`        |

There is no `zext.b` in Zbb, because `andi rd, rs, 0xFF` already does that in one base instruction.

## Python equivalents

```python
x.bit_count()                                   # cpop
32 - x.bit_length()                             # clz, for 0 <= x < 2**32
(x & -x).bit_length() - 1                       # ctz, for x != 0
a & ~b                                          # andn
min(a, b), max(a, b)                            # min, max
((x << n) | (x >> (32 - n))) & 0xFFFFFFFF       # rol, for 0 < n < 32
int.from_bytes(x.to_bytes(4, "little"), "big")  # rev8
```
