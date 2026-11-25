# Introduction

There are only 32 registers, but programs often work with far more numbers than that.
The rest are kept in **memory**.

## Memory and addresses

Memory is a huge row of bytes.
A **byte** is 8 bits, and every byte in the row has its own number, called its **address**.
The first byte has address 0, the next has address 1, and so on, up into the billions.

An address is just a number, so it fits in a register like any other number.
The processor can **load** a value from memory into a register, and **store** a value from a register into memory.
All the calculations still happen in registers.

## Pointers

A C function can take a **pointer**: the address of a value in memory, rather than the value itself.
In the C declarations in the test file, a `*` after the type marks a pointer:

```c
extern int32_t first_score(const int32_t *scores);
```

Here `scores` is the address of some `int32_t` values.
When the function is called, that address arrives in `a0`, just like any other argument.
`const` means the function promises only to read the values, not to change them.

In C, `&` gives the address of a variable, so `first_score(&x)` passes the address of `x`.

Python works in a similar way when you pass a list to a function.
The function does not get a copy: it gets the same list, so any change it makes is seen by the caller.

```python
def add_a_point(scores):
    scores[0] = scores[0] + 1

team = [5, 3]
add_a_point(team)
print(team)  # [6, 3]
```

## Words

An `int32_t` takes 32 bits, which is 4 bytes.
A 4-byte value is called a **word**.

When several `int32_t` values sit next to each other in memory, each one starts 4 bytes after the one before.
If the first value is at address `a0`, the values are at these addresses:

| Value   | Address   | Offset from `a0` |
| ------- | --------- | ---------------- |
| first   | `a0`      | 0                |
| second  | `a0 + 4`  | 4                |
| third   | `a0 + 8`  | 8                |
| fourth  | `a0 + 12` | 12               |

The distance from the starting address is called the **offset**.

The processor expects a word's address to be a multiple of 4.
This is called **alignment**.
The pointers the tests pass you are always aligned, and offsets that are multiples of 4 keep them aligned.

## Loading a word

`lw` (load word) reads the word at an address and puts it in a register.
The address is written as `offset(register)`, and means "the register's value plus the offset".

```asm
        lw      t0, 0(a0)       # t0 = the word at address a0
        lw      t1, 4(a0)       # t1 = the word at address a0 + 4
```

Loading does not change memory: the value is copied into the register.

## Storing a word

`sw` (store word) writes a register into the word at an address.

```asm
        sw      t0, 4(a0)       # the word at address a0 + 4 = t0
        sw      zero, 0(a1)     # the word at address a1 = 0
```

Look carefully at the order of the operands.
Almost every instruction puts the register that receives the result first.
`sw` puts the **value** first and the address second, because the result goes to memory, not to a register.

The offset is an immediate that must fit in 12 bits, so it is between -2048 and 2047.
It can be negative: `-4(a0)` is the word just before the one at `a0`.

## Changing a value in memory

The processor cannot do arithmetic on memory directly.
To change a value in memory, you load it, change it in a register, and store it back.

```asm
# Takes 3 points from the second score.
take_three:
        lw      t0, 4(a0)       # load the second score
        addi    t0, t0, -3      # change it in the register
        sw      t0, 4(a0)       # store it back in the same place
        ret
```

```python
def take_three(scores):
    scores[1] = scores[1] - 3
```

A function like this one returns nothing: its C declaration starts with `void`.
It does its job by changing memory, and the caller ignores whatever is left in `a0`.
