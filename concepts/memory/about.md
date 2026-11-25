# About

## Memory is a row of bytes

A RISC-V processor sees memory as one long row of bytes.
Each byte has an **address**, a whole number starting at 0.
On RV32 an address is 32 bits, so there can be up to 4,294,967,296 addresses (4 GiB).
In practice, a program can only use the parts of that range that the operating system has given it.
Reading or writing anywhere else stops the program with an error, such as a "segmentation fault".

An address is an ordinary number.
Nothing in a register says "this is an address": it is only an address because your code uses it as one.

## Pointers in C

In C, a variable of type `int32_t *` is a **pointer**: it holds the address of an `int32_t`.

| C                  | Meaning                                             |
| ------------------ | --------------------------------------------------- |
| `int32_t *p`       | `p` holds the address of an `int32_t`               |
| `const int32_t *p` | the function will only read through `p`             |
| `&x`               | the address of the variable `x`                     |
| `*p`               | the value at the address in `p`                     |
| `p[2]`             | the value 2 places after `*p`, which is 8 bytes on  |

The calling convention passes a pointer like any other argument, in `a0`–`a7`.
A function that returns a pointer returns it in `a0`.

Python has no pointers, but every Python variable that holds a list, dictionary or object really holds a reference to it.
Passing a list to a function passes that reference, so the function can change the caller's list.
A pointer is the same idea, with the reference written out as a number.

## Loads and stores

RISC-V is a **load-store** architecture.
Arithmetic instructions work only on registers.
Only load and store instructions touch memory.

| Instruction            | Meaning                                           |
| ---------------------- | ------------------------------------------------- |
| `lw  rd, offset(rs1)`  | `rd` = the 4-byte word at address `rs1 + offset`  |
| `sw  rs2, offset(rs1)` | the 4-byte word at address `rs1 + offset` = `rs2` |

The address is always a base register plus a 12-bit signed offset, from -2048 to 2047.
The offset can be left out, so `lw t0, (a0)` means the same as `lw t0, 0(a0)`.
To reach further than 2047 bytes, first add to the base register, for example with `addi` or `add`.

`sw` is the one common instruction whose first operand is not written to.
Its first operand is the value to store, and the second is the address.

Other loads and stores move 1 or 2 bytes at a time:
`lb`, `lbu` and `sb` work on bytes, and `lh`, `lhu` and `sh` work on 2-byte **halfwords**.
Later exercises cover them.

## Byte order

A word is 4 bytes, but in what order are they stored?
RISC-V is **little-endian**: the lowest byte of the number is stored at the lowest address.
The word `0x12345678` stored at address 100 looks like this:

| Address | 100    | 101    | 102    | 103    |
| ------- | ------ | ------ | ------ | ------ |
| Byte    | `0x78` | `0x56` | `0x34` | `0x12` |

As long as you load and store whole words, you never need to think about this.

## Alignment

A word is **aligned** when its address is a multiple of 4.
C compilers keep every `int32_t` aligned, so the pointers your functions receive are aligned.

The RISC-V specification allows a processor to handle a misaligned `lw` or `sw` slowly, or to stop the program with an error.
Keep word addresses aligned, and keep offsets for words multiples of 4.

## Read, modify, write

Changing a value in memory always takes three steps:

1. load it into a register,
2. change it in the register,
3. store it back to the same address.

```asm
        lw      t0, 0(a0)
        addi    t0, t0, 1
        sw      t0, 0(a0)
```

When a function changes two values, load everything it needs before it stores anything.
Two pointers might point at the same place, and a store through one changes what a later load through the other would read.

## Functions that return nothing

A C function declared `void` returns no value.
The caller ignores `a0` after the call, so a `void` function may leave anything there.
Such functions usually do their work by storing to memory through pointer arguments.
