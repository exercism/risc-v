# Introduction

Assembly language is the language a processor understands, written as text.
Each line is one small step for the processor to carry out: add these two
numbers, copy that number over here, return to whoever called us.

## Registers

In Python or JavaScript you can make as many variables as you like.
A RISC-V processor instead has 32 **registers**: storage spaces built into the processor, each holding one 32-bit whole number.
Every calculation happens in registers.

The registers you will use first are:

| Register      | Used for                                                  |
| ------------- | --------------------------------------------------------- |
| `a0` to `a7`  | the arguments a function receives, and the value it returns |
| `t0` to `t6`  | temporary values, for working things out along the way      |
| `zero`        | always holds 0, and ignores anything written to it          |

## Instructions

An instruction is written as its name, then its operands, separated by commas.
The first operand is usually the register that receives the result.

```asm
li      t0, 120         # t0 = 120     (load immediate)
mv      t1, a0          # t1 = a0      (copy a register)
add     a0, a1, a2      # a0 = a1 + a2
sub     a0, a1, a2      # a0 = a1 - a2
addi    a0, a0, 5       # a0 = a0 + 5
addi    a0, a0, -5      # a0 = a0 - 5
```

A number written directly into an instruction, like the `5` above, is called an **immediate**.
There is no `subi` instruction, because `addi` with a negative immediate does the same job.

Everything after a `#` on a line is a comment, which the assembler ignores.

## Functions

Here is a complete function, with a Python version for comparison:

```asm
.text
.global add_three

# Returns the sum of its three arguments.
add_three:
        add     a0, a0, a1
        add     a0, a0, a2
        ret
```

```python
def add_three(a, b, c):
    return a + b + c
```

- `.text` tells the assembler that what follows is code.
- `.global add_three` makes the function visible to other files, so the tests can call it.
- `add_three:` is a **label**. It names the place where the function's code starts.
- The function's first argument arrives in `a0`, the second in `a1`, the third in `a2`, and so on.
- Whatever is in `a0` when the function reaches `ret` is its return value.
- `ret` returns to the code that called the function.

Lines starting with `.` are **directives**: instructions for the assembler itself, rather than for the processor.
