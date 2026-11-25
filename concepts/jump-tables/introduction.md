# Introduction

In Python, a function is a value.
You can put functions in a list or a dictionary, pick one out, and call it:

```python
def add(a, b):
    return a + b

def sub(a, b):
    return a - b

OPERATIONS = {"+": add, "-": sub}
OPERATIONS["-"](7, 2)   # => 5
```

Assembly can do the same thing, because code lives in memory, and a place in memory is just a number.

## Code has an address

Each instruction of your program is stored in memory, so each one has an address.
A label in `.text` stands for the address of the instruction after it, just as a label in `.rodata` stands for the address of some data.

`la` works on code labels too:

```asm
        la      t0, triple      # t0 = the address of the first instruction of triple
```

An address of code, held in a register or in memory, is called a **code pointer**.
When it is the address of a function, it is a **function pointer**.

## Jumping to an address in a register

`j label` jumps to a label written in the instruction.
`jr` jumps to the address held in a register instead:

```asm
        la      t0, triple      # t0 = the address of triple
        jr      t0              # jump to that address
```

This is called an **indirect jump**, because the instruction does not say where it goes: the register does.

You have used an indirect jump all along.
`ret` is really `jalr zero, 0(ra)`, and `jr t0` is really `jalr zero, 0(t0)`.
Both jump to the address in a register, and throw away the return address, because the link register is `zero`.

`jalr t0` is short for `jalr ra, 0(t0)`.
It is a `call` through a register: it puts the return address in `ra`, then jumps to the address in `t0`.

## A table of code addresses

`.word` can hold the address of a label, not only a number.
The linker fills in the address when it builds the program.
So a list of code addresses is just an array of words:

```asm
.section .rodata
        .balign 4
transform_table:                # one code address for each code: 0, 1, 2
        .word   .Ltransform_times_ten
        .word   .Ltransform_tenths
        .word   .Ltransform_last_digit
```

This is called a **jump table**.

## A switch statement

Many languages have a `switch` or `match` statement that runs different code for each value of a number.
A jump table is how a compiler often builds one.
The steps are:

1. Check that the number is in the table's range.
   If it is not, go to the code for "any other value".
   One unsigned comparison checks both ends of the range.
2. Multiply the number by 4 with `slli`, because each address is a word.
3. Add the address of the table, and load the code address with `lw`.
4. Jump to it with `jr`.

```asm
# Returns n * 10 for code 0, n / 10 for code 1, n % 10 for code 2,
# and -1 for any other code.
# code arrives in a0, n in a1.
transform:
        li      t0, 3                           # the table has 3 entries
        bgeu    a0, t0, .Ltransform_other       # code < 0 or code >= 3
        slli    t1, a0, 2                       # t1 = 4 * code
        la      t2, transform_table             # t2 = the address of the table
        add     t2, t2, t1                      # t2 = the address of entry code
        lw      t3, 0(t2)                       # t3 = the code address in that entry
        jr      t3                              # jump to it
.Ltransform_times_ten:
        li      t0, 10
        mul     a0, a1, t0
        ret
.Ltransform_tenths:
        li      t0, 10
        div     a0, a1, t0
        ret
.Ltransform_last_digit:
        li      t0, 10
        rem     a0, a1, t0
        ret
.Ltransform_other:
        li      a0, -1
        ret
```

```python
def transform(code, n):
    match code:
        case 0:
            return n * 10
        case 1:
            return int(n / 10)
        case 2:
            return n - int(n / 10) * 10
        case _:
            return -1
```

Never leave out the range check.
Without it, a code of 7 would load a word from past the end of the table, and jump to whatever address happened to be there.

However many cases there are, a jump table goes to the right one in the same few instructions.
A chain of `beq` instructions has to test the cases one at a time.

## Jumping back into a loop

The blocks of code that a jump table chooses between do not have to end with `ret`.
Inside a loop, each block can finish with `j` back to the loop, to carry on with the next step:

```asm
.Lscore_try:
        addi    t4, t4, 5               # one case of the switch: 5 points
        j       .Lscore_next            # back to the loop
```

## Tail calls through a table

The entries of a jump table can also be the addresses of whole functions, each ending with `ret`.
The function that uses the table jumps to one of them with `jr`, and never changes `ra`.
So when that function reaches its `ret`, it goes straight back to the original caller.
This is a tail call, like `tail` from the functions concept, but through a register.

## Function pointers in C

In a C declaration, a function pointer looks like this:

```c
extern int32_t apply_to_ten(int32_t (*fn)(int32_t));
```

Read `int32_t (*fn)(int32_t)` as: `fn` is a pointer to a function that takes one `int32_t` and returns an `int32_t`.
The pointer arrives in a register like any other argument.

To use it, put the function's arguments in `a0`, `a1`, … and jump to it.
If the last thing your function does is call `fn` and return its result, a tail jump with `jr` is all you need:

```asm
# Returns fn(10).
# fn arrives in a0.
apply_to_ten:
        mv      t0, a0          # t0 = fn: a0 is needed for the argument
        li      a0, 10          # fn's argument
        jr      t0              # fn returns straight to our caller
```

```python
def apply_to_ten(fn):
    return fn(10)
```

Notice that `fn`'s argument goes in `a0`, so the pointer must be moved out of `a0` first.

Calling `fn` with `jalr` and coming back is harder.
`fn` was written by someone else, so it may change any of `a0`–`a7`, `t0`–`t6` and `ra`.
There is no temporary register where your own return address is safe.
The stack concept gives you a safe place to keep it.

A function can also **return** a function pointer.
It is just an address in `a0`:

```asm
        la      a0, triple      # return the address of triple
        ret
```
