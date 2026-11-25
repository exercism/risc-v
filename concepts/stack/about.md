# About

Every running program has a **stack**: an area of memory that functions use for values that must survive a call.
The register `sp` (`x2`), the **stack pointer**, holds the address of the top of the stack.

## The stack grows downward

When a program starts, the operating system sets `sp` to the high end of the stack area.
A function makes room by subtracting from `sp`, and gives the room back by adding the same amount:

```asm
        addi    sp, sp, -32     # take 32 bytes
        # ...
        addi    sp, sp, 32      # give them back
```

The bytes from `sp` up to just below the old `sp` belong to the function.
They are its **stack frame**.
Anything below `sp` is free space, and may be overwritten at any moment, so never store anything there.
Anything above the frame belongs to the callers, and must not be changed.

## Alignment

The RISC-V psABI (the document that defines the calling convention) requires `sp` to be a multiple of 16 at every call and on entry to every function.
So frame sizes are always multiples of 16.
Round up the space you need:

| Words to save | Bytes needed | Frame size |
| ------------- | ------------ | ---------- |
| 1 to 4        | 4 to 16      | 16         |
| 5 to 8        | 20 to 32     | 32         |
| 9 to 12       | 36 to 48     | 48         |

A frame of 16 bytes holds 4 words, at `0(sp)`, `4(sp)`, `8(sp)` and `12(sp)`.
By custom, `ra` goes in the highest word, and the `s` registers go in the words below it.

## The calling convention

| Register    | ABI name   | Preserved across a call? | Saved by |
| ----------- | ---------- | ------------------------ | -------- |
| `x0`        | `zero`     | (always 0)               | –        |
| `x1`        | `ra`       | no                       | callee, if it makes calls |
| `x2`        | `sp`       | yes                      | callee   |
| `x3`        | `gp`       | (not used by functions)  | –        |
| `x4`        | `tp`       | (not used by functions)  | –        |
| `x5`–`x7`   | `t0`–`t2`  | no                       | caller   |
| `x8`        | `s0` / `fp`| yes                      | callee   |
| `x9`        | `s1`       | yes                      | callee   |
| `x10`–`x17` | `a0`–`a7`  | no                       | caller   |
| `x18`–`x27` | `s2`–`s11` | yes                      | callee   |
| `x28`–`x31` | `t3`–`t6`  | no                       | caller   |

**Callee-saved** registers keep their value across a call.
A function that wants to use one must save it in its prologue and restore it in its epilogue.

**Caller-saved** registers may be changed by any call.
If a caller needs one of these values after a call, it either copies it to an `s` register first, or stores it in its own stack frame and loads it back after the call.

## A typical function

```asm
# Returns f(a) + f(b), for some function f.
sum_of_two:
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)
        sw      s1, 4(sp)

        mv      s0, a1          # b survives the first call
        call    f
        mv      s1, a0          # f(a) survives the second call
        mv      a0, s0
        call    f
        add     a0, a0, s1

        lw      s1, 4(sp)
        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret
```

Loading the registers back in the reverse order is a habit, not a rule: any order works, as long as each register is loaded from the word it was saved in.

## Leaf functions

A **leaf function** calls nothing.
If it uses only `a` and `t` registers, it needs no stack frame and does not touch `sp` or `ra`.
That makes leaf functions short and fast, and compilers try hard to produce them.

## The frame pointer

`s0` has a second name, `fp`, the **frame pointer**.
Some code sets `fp` to the value `sp` had on entry, so that the frame can be reached at fixed offsets from `fp` even while `sp` moves.
Debuggers can follow saved frame pointers to print a backtrace, much like a Python traceback.
You need one when a function moves `sp` by an amount only known at run time, as the local arrays concept shows.

## Stack overflow

The stack has a fixed size, often a few megabytes.
If functions keep calling functions without returning, for example in recursion that never stops, `sp` eventually moves past the end of the stack area.
The program then crashes with a **stack overflow**, the cousin of Python's `RecursionError`.

## Common mistakes

- Forgetting to restore `ra`, so that `ret` jumps back into the middle of your own function.
- Changing `sp` by different amounts in the prologue and the epilogue.
- Returning early from the middle of a function without running the epilogue.
  A good habit is to branch to a single epilogue at the end.
- Using an `s` register without saving it first.
  The tests may not notice, but the caller's value is destroyed.
- Keeping a value in a `t` or `a` register across a call.
