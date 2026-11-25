# Introduction

## The base and the extensions

RISC-V is built in layers.
At the bottom is the **base** instruction set, called **I** for "integer".
Every RISC-V processor has it: `add`, `and`, `sll`, `lw`, `beq` and the other instructions you have met.

On top of the base, a processor may add **extensions**: optional groups of extra instructions.
Each extension has a name:

| Name    | What it adds                                            |
| ------- | ------------------------------------------------------- |
| **M**   | multiplication and division: `mul`, `div`, `rem`        |
| **C**   | shorter, 16-bit versions of common instructions         |
| **V**   | vector instructions, which work on many numbers at once |
| **Zbb** | basic bit manipulation, the subject of this concept     |

So `mul` was never part of the base: it comes from the M extension.
The processor that runs the tests on this track happens to have it.

A processor's **arch string** lists what it has, base first.
For example, `rv32imc_zbb` means a 32-bit RISC-V with the base I, plus M, C and Zbb.
Single-letter extensions come first, and longer names that start with `Z` come after an underscore.

Not every processor has every extension.
A small chip in a toy may have only the base, and it cannot run a `mul` at all.

## Turning Zbb on

The assembler only accepts Zbb instructions if you ask for them.
This line at the top of the `.S` file asks for them:

```asm
.option arch, +zbb      # allow Zbb instructions in this file
```

`.option arch` is a directive that changes which instructions the assembler accepts.
`+zbb` means "add the Zbb extension".
The stub for this exercise already starts with this line, so leave it there.

Each Zbb instruction does a job that took a loop, or several instructions, with the base alone.

## Counting the set bits

In the bit-sets concept, you counted the members of a set with a loop that cleared one bit each time round.
`cpop`, short for "count population", counts the set bits in one step:

```asm
        li      t0, 0b10110     # t0 has 3 set bits
        cpop    t1, t0          # t1 = 3
```

Python 3.10 and later can do the same:

```python
(0b10110).bit_count()       # => 3
```

## Counting zeros at either end

`ctz` counts the **trailing** zeros: the 0 bits below the lowest set bit, starting from bit 0.
`clz` counts the **leading** zeros: the 0 bits above the highest set bit, starting from bit 31.

| Value        | Bits                                      | `ctz` | `clz` |
| ------------ | ----------------------------------------- | ----- | ----- |
| `0x00000001` | `0000 0000 0000 0000 0000 0000 0000 0001` | 0     | 31    |
| `0x00000300` | `0000 0000 0000 0000 0000 0011 0000 0000` | 8     | 22    |
| `0x80000000` | `1000 0000 0000 0000 0000 0000 0000 0000` | 31    | 0     |
| `0x00000000` | `0000 0000 0000 0000 0000 0000 0000 0000` | 32    | 32    |

```asm
        li      t0, 0x300       # bits 8 and 9 are set
        ctz     t1, t0          # t1 = 8
        clz     t2, t0          # t2 = 22
```

For a bit set, the number of trailing zeros is the position of the lowest set bit, so `ctz` finds the smallest member without a loop.

When the value is 0, there is no set bit to stop at, and both instructions count all 32 bits.
Remember that 32 is not a bit position: positions only go from 0 to 31.

Python's `bit_length()` tells you how many bits a number needs, which is 32 minus the leading zeros:

```python
(0x300).bit_length()        # => 10, and 32 - 10 is 22
```

## Masks with a flip built in

In the bit-sets concept, you found the difference of two sets with `not` and then `and`.
Zbb has instructions that flip their second operand for you:

| Instruction         | Result in `rd`  | Python     |
| ------------------- | --------------- | ---------- |
| `andn rd, rs1, rs2` | `rs1 & ~rs2`    | `a & ~b`   |
| `orn rd, rs1, rs2`  | `rs1 \| ~rs2`   | `a \| ~b`  |
| `xnor rd, rs1, rs2` | `~(rs1 ^ rs2)`  | `~(a ^ b)` |

```asm
        li      t0, 0b1100
        li      t1, 0b1010
        andn    t2, t0, t1      # t2 = 0b0100: in t0 but not in t1
```

Notice that the operand order matters for `andn` and `orn`: it is the second register that is flipped.

## Smaller and larger

Finding the smaller of two numbers needs a branch with the base instructions.
Zbb has instructions that do it directly, like Python's `min` and `max`:

| Instruction | Result                  | Compares as |
| ----------- | ----------------------- | ----------- |
| `min`       | the smaller of the two  | signed      |
| `max`       | the larger of the two   | signed      |
| `minu`      | the smaller of the two  | unsigned    |
| `maxu`      | the larger of the two   | unsigned    |

Signed and unsigned give different answers when a top bit is set:

```asm
        li      t0, -1          # 0xFFFFFFFF
        li      t1, 5
        min     t2, t0, t1      # t2 = -1, because -1 < 5
        minu    t3, t0, t1      # t3 = 5, because 0xFFFFFFFF is 4294967295 as unsigned
```

Choose the one that matches the type in the C declaration: `min` and `max` for `int32_t`, `minu` and `maxu` for `uint32_t`.

## Rotating

A shift pushes bits off one end of the register, and they are lost.
A **rotate** moves them round to the other end instead, like people moving round a circle.

`rol` rotates left: each bit moves up by the shift amount, and the bits that go past bit 31 come back in at bit 0.
`ror` rotates right, and `rori` rotates right by an amount written in the instruction.

```asm
        li      t0, 0xF0000001
        li      t1, 4
        rol     t2, t0, t1      # t2 = 0x0000001F: the top four bits came round to the bottom
        rori    t3, t0, 4       # t3 = 0x1F000000: the bottom bit came round to the top
```

There is no `roli`.
To rotate left by a fixed amount `n`, rotate right by `32 - n` with `rori`.

Like the shift instructions, the rotates use only the lowest 5 bits of the amount register, so rotating by 32 is the same as rotating by 0.

With only the base instructions, a rotate takes a `sll`, a `srl` and an `or`.
In Python:

```python
def rotate_left(x, n):
    n = n % 32
    return ((x << n) | (x >> (32 - n))) & 0xFFFFFFFF
```
