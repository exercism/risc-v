# About

Normally the processor runs instructions one after another.
A register called the **program counter** (`pc`) holds the address of the next instruction, and after each instruction it moves on by 4 bytes.
Branches and jumps change the program counter, so the processor carries on somewhere else.

## Conditional branches

RISC-V has six conditional branch instructions.
Each compares two registers and jumps if the comparison is true:

| Instruction           | Jumps when                     |
| --------------------- | ------------------------------ |
| `beq  rs1, rs2, label` | `rs1 == rs2`                  |
| `bne  rs1, rs2, label` | `rs1 != rs2`                  |
| `blt  rs1, rs2, label` | `rs1 < rs2`, as signed numbers  |
| `bge  rs1, rs2, label` | `rs1 >= rs2`, as signed numbers |
| `bltu rs1, rs2, label` | `rs1 < rs2`, as unsigned numbers  |
| `bgeu rs1, rs2, label` | `rs1 >= rs2`, as unsigned numbers |

The unsigned forms, `bltu` and `bgeu`, are covered in a later concept.

There is no branch that compares a register with an immediate.
Load the number into a temporary register with `li` first, or compare with `zero`.

## Branch pseudo-instructions

The assembler turns each of these into a real branch:

| You write          | The assembler produces |
| ------------------ | ---------------------- |
| `bgt  a, b, label` | `blt  b, a, label`     |
| `ble  a, b, label` | `bge  b, a, label`     |
| `beqz a, label`    | `beq  a, zero, label`  |
| `bnez a, label`    | `bne  a, zero, label`  |
| `bltz a, label`    | `blt  a, zero, label`  |
| `bgez a, label`    | `bge  a, zero, label`  |
| `blez a, label`    | `bge  zero, a, label`  |
| `bgtz a, label`    | `blt  zero, a, label`  |

`bgt` and `ble` work by swapping the two operands: `a > b` is the same test as `b < a`.

## Unconditional jumps

`j label` always jumps.
It is a pseudo-instruction for `jal zero, label`.
`jal` ("jump and link") also saves the address of the next instruction in a register, which is how functions are called.
Writing the address to `zero` throws it away, leaving a plain jump.

## How far a branch can reach

The target of a branch is stored in the instruction as a distance from the branch itself, not as a full address.
A conditional branch can reach about 4 KiB forwards or backwards, and `j` can reach about 1 MiB.
That is plenty for the functions on this track.
If a branch target is too far away, the assembler reports an error, or quietly rewrites the branch as an opposite branch around a `j`.

## Labels

A label is a name followed by a colon.
It stands for the address of whatever comes after it.

- A label such as `alert_level:` is a normal symbol.
  With `.global` it can be seen from other files.
- A label starting with `.L`, such as `.Lalert_done:`, is a **local label**.
  The assembler does not put it in the symbol table, so it cannot clash with names in other files, and debuggers and disassemblers do not show it.
  It must still be unique within its own file.

The assembler also accepts **numeric labels**, such as `1:`.
A numeric label can be used many times in one file.
To refer to one, write the number followed by `f` (the next one **f**orwards) or `b` (the nearest one **b**ackwards):

```asm
        bgez    a0, 1f                  # jump forwards to the next 1:
        sub     a0, zero, a0
1:
        ret
```

Numeric labels are short, but they say nothing about what they mark.
This track uses named `.L` labels in its examples.

## Structuring decisions

| Python                      | Assembly pattern                                                                 |
| --------------------------- | -------------------------------------------------------------------------------- |
| `if c: body`                | branch on _not_ `c` past `body`                                                  |
| `if c: A else: B`           | branch on _not_ `c` to `B`; after `A`, `j` past `B`                              |
| `if c1: A elif c2: B else: C` | branch on `c1` to `A`, on `c2` to `B`, fall through to `C`; each part ends with `j` or `ret` |

A compound condition is a sequence of branches:

- `if a and b:` branches past the body as soon as either part is false.
- `if a or b:` branches into the body as soon as either part is true.

This is the same **short-circuit** behaviour that Python's `and` and `or` have: once the answer is known, the rest of the condition is not checked.
