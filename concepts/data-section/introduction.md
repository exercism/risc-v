# Introduction

So far, every value your functions used came from a register, an immediate, or memory that the caller gave you.
A program can also keep its own values in memory: text to show, tables of numbers, and counters.
You write these values into the assembly file, and the assembler puts them in memory before the program starts.

## Sections

An assembly file is divided into **sections**.
Each section holds one kind of thing, and the program keeps each kind in a different part of memory.

| Directive          | What goes in it                                   | Can the program change it? |
| ------------------ | ------------------------------------------------- | -------------------------- |
| `.text`            | code: your functions                              | no                         |
| `.section .rodata` | **r**ead-**o**nly **data**: text and tables       | no                         |
| `.data`            | data that the program may change                  | yes                        |
| `.bss`             | data that starts as all zeros                     | yes                        |

Writing one of these directives switches to that section.
Everything after it goes in that section, until the next section directive.
You can switch back and forth as often as you like.

## Labels on data

You met labels as names for places in the code.
A label can also name a place in a data section.
The label stands for the **address** of the first byte after it.

```asm
.section .rodata
motto:                          # motto is the address of the 'I'
        .string "Ia manuia"     # the bytes of "Ia manuia", then a zero byte

.text
.global show_motto
show_motto:
        # ...
```

Labels on data do not need `.global` unless code in another file must use them.
`.global` works on a data label in the same way as on a function.

## Data directives

These directives put values into the current section:

| Directive                | Puts in memory                                         |
| ------------------------ | ------------------------------------------------------ |
| `.string "text"`         | the bytes of the text, then a zero byte                |
| `.asciz "text"`          | the same as `.string`                                  |
| `.ascii "text"`          | the bytes of the text, with **no** zero byte           |
| `.byte 1, 2, 3`          | one byte for each number                               |
| `.half 1, 2` or `.2byte` | two bytes (a halfword) for each number                 |
| `.word 1, 2` or `.4byte` | four bytes (a word) for each number                    |
| `.zero 8` or `.space 8`  | 8 bytes that are all zero                              |

Each value goes straight after the one before, with no gaps.
A `.string` already ends with its zero terminator, so you never add one yourself.
With `.ascii` you must add it, for example with `.byte 0`, or the string does not end.

Letters that are not in the ASCII table, such as the `ā` in Tongan, are written in **UTF-8**, which uses two or more bytes for them.
You can type them straight into a `.string`, and the assembler stores their bytes.

## Alignment

You saw in the memory concept that the address of a word must be a multiple of 4.
The assembler does not move a `.word` to a good address by itself.
If some bytes come before it, write `.balign 4` first.
It adds zero bytes until the address is a multiple of 4.

```asm
.section .rodata
team_sizes:                     # players in a team
        .byte   7, 11, 15       # netball, cricket, rugby
        .balign 4               # skip to the next multiple of 4
squares:
        .word   0, 1, 4, 9, 16  # five words, 20 bytes
```

## Loading an address

`la` means **l**oad **a**ddress.
It puts the address of a label into a register:

```asm
        la      t0, squares     # t0 = the address of squares
```

`la` is a pseudo-instruction.
The assembler turns it into two instructions, `auipc` and `addi`, that work out the address from where the code is.
You only need to write `la`.

A function can return a string by returning its address.
The C declaration says the function returns a `const char *`:

```c
extern const char *show_motto(void);
```

```asm
show_motto:
        la      a0, motto       # return the address of the string
        ret
```

```python
MOTTO = "Ia manuia"

def show_motto():
    return MOTTO
```

The string is not copied.
The caller gets the address of the bytes in `.rodata`, and reads them from there.

## Reading a table

Once you have the address of a table in a register, you read it just like an array that a caller gave you.
For element `i` of a `.byte` table, add `i` to the address and use `lbu`.
For a `.word` table, add `4 * i` and use `lw`.

```asm
# Returns squares[i].
# i arrives in a0.
square:
        la      t0, squares     # t0 = the address of squares[0]
        slli    t1, a0, 2       # t1 = 4 * i
        add     t0, t0, t1      # t0 = the address of squares[i]
        lw      a0, 0(t0)       # a0 = squares[i]
        ret
```

```python
SQUARES = (0, 1, 4, 9, 16)

def square(i):
    return SQUARES[i]
```

## Never write to `.rodata`

The read-only data really is read-only.
If your code stores into it with `sw` or `sb`, the program crashes straight away with a **segmentation fault**.
Put anything you want to change in `.data` or `.bss` instead.

## Variables that keep their value

A register keeps nothing between calls: the next call starts again with whatever the caller puts in.
A value in `.data` stays in memory, so the next call sees whatever the last call stored there.
It is like a Python variable at the top level of a module, which a function changes with `global`.

```asm
.data
        .balign 4
last_score:                     # starts as 0
        .word   0

.text
# Remembers a new score, and returns the one remembered before it.
# score arrives in a0.
swap_score:
        la      t0, last_score  # t0 = the address of last_score
        lw      t1, 0(t0)       # t1 = the old score
        sw      a0, 0(t0)       # last_score = the new score
        mv      a0, t1          # return the old score
        ret
```

```python
last_score = 0

def swap_score(score):
    global last_score
    old = last_score
    last_score = score
    return old
```

The same variable could go in `.bss` as `.zero 4`, because it starts as 0.
