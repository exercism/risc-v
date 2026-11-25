# About

## Booleans

RISC-V has no boolean type.
By convention, a register holds 1 for true and 0 for false.
C follows the same convention: a comparison such as `a < b` has the value 1 or 0, and a C `bool` is stored as 0 or 1.
In Python, `True == 1` and `False == 0`, and `int(a < b)` gives the same answer as `slt`.

Many C functions treat any non-zero value as true.
When a function promises to return exactly 0 or 1, make sure it does: `snez` turns any number into 0 or 1.

## Comparison instructions

The base instruction set has four comparison instructions.
Each writes 1 to the destination register if the comparison is true, and 0 if it is false.

| Instruction           | Meaning                                         |
| --------------------- | ----------------------------------------------- |
| `slt rd, rs1, rs2`    | `rd = (rs1 < rs2)`, comparing as signed numbers |
| `slti rd, rs1, imm`   | `rd = (rs1 < imm)`, comparing as signed numbers |
| `sltu rd, rs1, rs2`   | `rd = (rs1 < rs2)`, comparing as unsigned numbers |
| `sltiu rd, rs1, imm`  | `rd = (rs1 < imm)`, comparing as unsigned numbers |

The immediate in `slti` and `sltiu` must fit in 12 bits, between -2048 and 2047.
Signed and unsigned numbers are covered in a later concept.
Until then, use `slt` and `slti`, and the pseudo-instructions below.

## Pseudo-instructions

| You write          | The assembler produces    | Meaning                    |
| ------------------ | ------------------------- | -------------------------- |
| `seqz rd, rs`      | `sltiu rd, rs, 1`         | `rd = (rs == 0)`           |
| `snez rd, rs`      | `sltu rd, zero, rs`       | `rd = (rs != 0)`           |
| `sltz rd, rs`      | `slt rd, rs, zero`        | `rd = (rs < 0)`            |
| `sgtz rd, rs`      | `slt rd, zero, rs`        | `rd = (rs > 0)`            |
| `sgt rd, rs1, rs2` | `slt rd, rs2, rs1`        | `rd = (rs1 > rs2)`         |
| `sgtu rd, rs1, rs2`| `sltu rd, rs2, rs1`       | `rd = (rs1 > rs2)`, unsigned |

`seqz` works because, as an unsigned number, only 0 is less than 1.
`snez` works because, as an unsigned number, every value except 0 is greater than 0.

## Equal and not equal

There is no "set if equal" instruction.
Subtract, then test the difference:

```asm
sub     t0, a0, a1      # t0 = a0 - a1
seqz    t0, t0          # t0 = (a0 == a1)
```

Use `snez` instead of `seqz` for "not equal".
To compare with a constant, use `addi` with the negated constant instead of `sub`.

## The other comparisons

Every comparison can be built from "less than", by swapping the operands, by flipping the answer, or both:

| Question   | How                                             |
| ---------- | ----------------------------------------------- |
| `a < b`    | `slt rd, a, b`                                  |
| `a > b`    | `sgt rd, a, b` (which is `slt rd, b, a`)        |
| `a >= b`   | `slt rd, a, b`, then `xori rd, rd, 1`           |
| `a <= b`   | `sgt rd, a, b`, then `xori rd, rd, 1`           |

For a constant, `a <= 9` is the same as `a < 10`, so a single `slti rd, a, 10` is enough.

## Combining booleans

When both inputs are 0 or 1:

| Instruction        | Result                      | Python           |
| ------------------ | --------------------------- | ---------------- |
| `and rd, rs1, rs2` | 1 only if both are 1        | `rs1 and rs2`    |
| `or rd, rs1, rs2`  | 1 if either is 1            | `rs1 or rs2`     |
| `xori rd, rs, 1`   | 1 if `rs` is 0, 0 if it is 1 | `not rs`        |

These are **bitwise** instructions, covered fully in a later concept.
They act on all 32 bits at once, so with other inputs they can give answers other than 0 or 1.
For example, `or` of 1 and 2 gives 3, and `and` of 1 and 2 gives 0.
Convert counts and other numbers to 0 or 1 with `snez` first.

The pseudo-instruction `not` is **not** a boolean "not": it flips all 32 bits, so `not` of 1 gives -2.
Use `xori rd, rs, 1` to flip a boolean.

## Comparisons or branches?

Anything you can do with comparison instructions, you can also do with branches.
Comparison instructions avoid jumps, which keeps code short and easy to follow, and is often faster on real processors.
Use a branch when you need to choose between doing different work, and a comparison when you just need a 0 or 1 answer.
