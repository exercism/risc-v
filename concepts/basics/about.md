# About

RISC-V is an **instruction set architecture**: a description of the instructions a processor understands.
It is open, so anyone may design and build a RISC-V processor without paying a licence fee.
That has made it popular for teaching, research, and small embedded devices.

This track uses **RV32**, the 32-bit version: every register holds 32 bits.

## Registers

RISC-V has 32 integer registers, `x0` to `x31`.
Each one also has an **ABI name** that describes how it is normally used, and assembly code almost always uses those names:

| Register    | ABI name     | Used for                                         |
| ----------- | ------------ | ------------------------------------------------ |
| `x0`        | `zero`       | always 0; writes to it are discarded             |
| `x1`        | `ra`         | return address (where `ret` goes back to)        |
| `x2`        | `sp`         | stack pointer                                    |
| `x5`–`x7`   | `t0`–`t2`    | temporaries                                      |
| `x8`–`x9`   | `s0`–`s1`    | saved registers                                  |
| `x10`–`x17` | `a0`–`a7`    | function arguments and return values             |
| `x18`–`x27` | `s2`–`s11`   | saved registers                                  |
| `x28`–`x31` | `t3`–`t6`    | temporaries                                      |

`x3` (`gp`) and `x4` (`tp`) have special jobs and are best left alone.
Later exercises cover `ra`, `sp` and the saved registers.

## Numbers have a fixed size

In Python, whole numbers can grow as large as you like.
In a register, a number is always exactly 32 bits, so it can hold values from -2,147,483,648 to 2,147,483,647.
If a calculation goes past either end, it **wraps around** with no error: adding 1 to 2,147,483,647 gives -2,147,483,648.

## Instructions and pseudo-instructions

Most arithmetic instructions come in two forms:

- a **register** form, such as `add a0, a1, a2`, which works on two registers;
- an **immediate** form, such as `addi a0, a1, 5`, which works on a register and a number written in the instruction.

An immediate in `addi` must fit in 12 bits, which means it is between -2048 and 2047.

Some instructions you write are **pseudo-instructions**.
The assembler turns them into one or more real instructions for you:

| You write      | The assembler produces                                    |
| -------------- | --------------------------------------------------------- |
| `li t0, 120`   | `addi t0, zero, 120`                                      |
| `li t0, 70000` | `lui t0, 17` then `addi t0, t0, 368` (17 × 4096 + 368)    |
| `mv t0, a0`    | `addi t0, a0, 0`                                          |
| `neg t0, a0`   | `sub t0, zero, a0`                                        |
| `ret`          | `jalr zero, 0(ra)`                                        |

Because `li` can build any 32-bit value, you never need to worry about the 12-bit limit when loading a constant.

## Assembly files

The solutions on this track are `.S` files.
The capital `S` means the file passes through the C preprocessor before it is assembled, so you can use C-style `/* ... */` comments as well as `#` comments.

A file is made of:

- **labels**, such as `add_three:`, which name a place in the program;
- **directives**, which start with a `.` and tell the assembler what to do;
- **instructions**, which become machine code.

The directives you need to start with are:

| Directive        | Meaning                                                      |
| ---------------- | ------------------------------------------------------------ |
| `.text`          | what follows is code                                         |
| `.global name`   | make the label `name` visible outside this file              |

## Calling convention

The tests on this track are written in C, and call your assembly functions.
For that to work, both sides follow the RISC-V **calling convention**:

- arguments arrive in `a0`, `a1`, `a2`, …, `a7`, in order;
- the return value goes in `a0`;
- the function may change `a0`–`a7` and `t0`–`t6` freely.

The C declaration of each function is at the top of the test file:

```c
extern int32_t add_three(int32_t a, int32_t b, int32_t c);
```
