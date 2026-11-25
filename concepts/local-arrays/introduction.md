# Introduction

Sometimes a function needs a list of its own for a short time: a scratch list to count things in, or a copy of some values.
In Python, you just make one inside the function:

```python
def count_sizes(values):
    counts = [0] * 8          # a temporary list
    for v in values:
        counts[v] = counts[v] + 1
    return max(counts)        # counts is thrown away after this
```

In assembly, a function makes room for a temporary array in its own stack frame.
An array that lives in a function's stack frame is called a **local array**.
A temporary array like this is often called a **buffer**.

## Making room on the stack

You already make room on the stack with `addi sp, sp, -N`.
A local array is just more room.
An array of 8 words needs 8 × 4 = 32 bytes:

```asm
        addi    sp, sp, -32     # room for 8 words, at sp to sp + 31
        # ... use the array ...
        addi    sp, sp, 32      # give the room back
```

The 16-byte rule still holds, so round the size up to a multiple of 16.
An array of 5 words (20 bytes) needs a 32-byte frame.

If the function also saves registers, put the array at the bottom of the frame and the saved registers above it.
This frame has an 8-word array at offsets 0 to 31, and `ra` at offset 44:

```asm
        addi    sp, sp, -48     # 32 bytes of array + room for ra, rounded up to 48
        sw      ra, 44(sp)      # save ra above the array
```

## Using the array

Element `i` of the array is at address `sp + 4 * i`, so `sp` plays the part of the array's address.
When `i` is a number you know, write the offset into `lw` or `sw`.
When `i` is in a register, work out the address as you did in the arrays concept:

```asm
        sw      t0, 8(sp)       # element 2 = t0
        slli    t1, a1, 2       # t1 = 4 * i
        add     t1, sp, t1      # t1 = the address of element i
        lw      t2, 0(t1)       # t2 = element i
```

## Starting from zero

Python's `[0] * 8` gives a list full of zeros.
A new local array is **not** full of zeros.
Its memory still holds whatever earlier functions left there, which is called **garbage**.
If your code needs zeros, store them yourself, with a loop:

```asm
        mv      t0, sp          # t0 = pointer to element 0
        addi    t1, sp, 32      # t1 = end of the 8-word array
clear_loop:
        beq     t0, t1, clear_done  # stop at the end
        sw      zero, 0(t0)     # this element = 0
        addi    t0, t0, 4       # move to the next element
        j       clear_loop
clear_done:
```

## Passing the array to another function

The address of the array is in `sp`, so you can pass it to another function, just like any other array:

```asm
        mv      a0, sp          # a0 = the address of the local array
        li      a1, 8           # a1 = the number of elements
        call    show            # show(array, 8)
```

The callee makes its own frame below yours, so your array is safe while it runs.
It can read the array, and it can also write to it.

## The array disappears

When your function gives its frame back with `addi sp, sp, N`, the array is gone.
The memory is still there, but the next function that is called will use it for its own frame and overwrite it.

So **never return the address of a local array**.
The caller would get a pointer to memory that no longer belongs to anyone.
This is different from Python, where a list lives on for as long as something still refers to it.

If the caller needs the results, copy them into an array that the caller passed in.

## Arrays whose size is known only at run time

Sometimes the size depends on an argument, such as `n` words.
You can subtract a size held in a register from `sp`, with `sub`.
The size must still be rounded up to a multiple of 16:

```asm
        slli    t0, a1, 2       # t0 = 4 * n, the bytes needed
        addi    t0, t0, 15      # add 15 ...
        andi    t0, t0, -16     # ... and clear the low 4 bits: rounded up to a multiple of 16
        sub     sp, sp, t0      # make room for n words
```

In binary, -16 is all 1s except for the lowest 4 bits, which are 0.
So `andi` with -16 rounds down to a multiple of 16, and adding 15 first turns that into rounding up.
For example, 20 + 15 = 35 rounds down to 32, and 32 + 15 = 47 rounds down to 32.

Now the frame has a size that the code does not know in advance, so `addi sp, sp, N` cannot give it back.
Instead, keep a copy of `sp` from before the `sub` in `s0`, and copy it back afterwards.
Because `s0` is a saved register, save it first.

```asm
# Calls show with a temporary array holding 0, 1, 4, 9, ... (n squares).
# n arrives in a0.
show_squares:
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)       # we use s0, so save it
        sw      s1, 4(sp)
        mv      s0, sp          # s0 = sp before the array
        mv      s1, a0          # s1 = n

        slli    t0, a0, 2       # t0 = 4 * n
        addi    t0, t0, 15
        andi    t0, t0, -16     # round up to a multiple of 16
        sub     sp, sp, t0      # the array starts at sp

        li      t1, 0           # t1 = i
show_squares_loop:
        bge     t1, s1, show_squares_call
        mul     t2, t1, t1      # t2 = i * i
        slli    t3, t1, 2
        add     t3, sp, t3      # t3 = the address of element i
        sw      t2, 0(t3)       # element i = i * i
        addi    t1, t1, 1
        j       show_squares_loop
show_squares_call:
        mv      a0, sp          # the array
        mv      a1, s1          # its length
        call    show

        mv      sp, s0          # give the array back, whatever its size
        lw      s1, 4(sp)
        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret
```

When `s0` is used like this, to remember where the frame is, it is called the **frame pointer**, and `fp` is its other name.

## Stack overflow

The stack is not very big: often a few megabytes, and much less on small devices.
A local array that is too large moves `sp` past the end of the stack, and the program crashes with a **stack overflow**.
Local arrays are for small, short-lived buffers of a few dozen or a few hundred words.
