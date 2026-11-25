# About

A **constant** is a name for a number that the assembler works out while it reads your file.
Constants replace **magic numbers**: bare numbers whose meaning the reader has to guess.

## Defining constants

There are several directives that define a constant:

| Directive              | Meaning                                                         |
| ---------------------- | --------------------------------------------------------------- |
| `.equ  NAME, value`    | `NAME` stands for `value`                                       |
| `.set  NAME, value`    | the same as `.equ`                                              |
| `NAME = value`         | the same as `.equ`                                              |
| `.equiv NAME, value`   | like `.equ`, but it is an error if `NAME` is already defined    |

With `.equ` and `.set`, a later line may give the same name a new value, and the code after that line uses the new value.
That is rarely what you want, so most programs give each name a value only once.
`.equiv` makes the assembler check this for you.

Write constant names in UPPER_SNAKE_CASE, such as `BOARD_SIZE`, and define them near the top of the file, before the code that uses them.

## Constant expressions

The value of a constant can be an expression built from numbers, other constants and these operators:

| Operator              | Meaning                              |
| --------------------- | ------------------------------------ |
| `+` `-` `*` `/` `%`   | add, subtract, multiply, divide, remainder |
| `<<` `>>`             | shift left, shift right              |
| `&` `\|` `^` `~`      | bitwise and, or, xor, not            |
| `(` `)`               | grouping                             |

```asm
.equ    BOARD_SIZE, 8
.equ    BOARD_SQUARES, BOARD_SIZE * BOARD_SIZE      # 64
.equ    LAST_INDEX, BOARD_SIZE - 1                  # 7
```

The assembler does this arithmetic itself, once, while assembling.
The `*` above is not a `mul` instruction: the finished program only holds the answer.
Division in an expression rounds towards zero, like C, and dividing by zero is an error.

## Using constants in instructions

A constant, or an expression, can be used wherever an instruction takes an immediate:

```asm
        li      t0, BOARD_SQUARES               # t0 = 64
        addi    a0, a0, BOARD_SIZE - 1          # a0 = a0 + 7
        addi    a0, a0, -BOARD_SIZE             # a0 = a0 - 8
```

The usual limits on immediates still apply.
`addi` holds a 12-bit signed immediate, from -2048 to 2047, and the assembler reports an error if a constant does not fit.
`li` accepts any 32-bit value, because the assembler turns it into one or two instructions as needed.

Branches compare registers, never immediates, so to compare with a constant, load it with `li` first.

## Constants take no space

A constant is not a variable.
It has no address, takes no memory and uses no register.
`li t0, BOARD_SQUARES` produces exactly the same machine code as `li t0, 64`.

A constant may still appear in the object file's symbol table, marked as an _absolute_ symbol, which is a name for a plain number rather than for a place in memory.
`.global` can even export it to other files, but that is uncommon.

## Enumerations

A set of related codes, like Python's `enum.IntEnum` or C's `enum`, is written as one constant per code, with a shared start to the names:

```asm
.equ    RESULT_WIN, 1
.equ    RESULT_DRAW, 2
.equ    RESULT_LOSS, 3
```

The assembler does not know that these belong together, and it does not check that a register holds one of them.
That is up to you.

## Sharing constants with C

Assembly and C files are translated separately, and an `.equ` in one cannot be seen from a C file.
The simplest approach, used on this track, is for both files to define the same names with the same values: `.equ` in the assembly, and `#define` or `enum` in C.

A file ending in capital `.S` is passed through the **C preprocessor** before it is assembled.
That means a `.S` file can also use `#define`, and can `#include` a header file that C code includes too, so there is only one copy of each value.
Be careful mixing the two: a `#` comment at the very start of a line looks like a preprocessor command, and a C header full of declarations is not valid assembly.
