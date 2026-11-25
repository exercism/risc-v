# Introduction

In the functions concept, a function that made a call kept its return address in a temporary register, such as `t6`.
That shortcut works only when you know exactly which registers the helper changes.
The standard way, which works for every function, is to save values in memory, on the **stack**.

## The stack

The **stack** is an area of memory that every running program has.
Each function that is running borrows a small piece of it, and gives the piece back before it returns.

The register `sp`, the **stack pointer**, holds the address of the **top** of the stack.
On RISC-V, the stack **grows downward**: to make room, a function makes `sp` smaller.
The space below the old `sp` is now the function's own.

```asm
        addi    sp, sp, -16     # make room for 16 bytes (4 words)
        # ... the 16 bytes from sp to sp + 15 are ours ...
        addi    sp, sp, 16      # give the 16 bytes back
```

The piece of the stack that one function uses is called its **stack frame**.

## Saving a register on the stack

A word is 4 bytes, so a 16-byte frame has room for 4 words, at offsets 0, 4, 8 and 12 from `sp`.
You already know `sw` and `lw`.
With `sp` as the address, they save a register in the frame and load it back later:

```asm
        sw      ra, 12(sp)      # save ra in the word at sp + 12
        # ... ra may now be changed, for example by a call ...
        lw      ra, 12(sp)      # put ra back as it was
```

Saving a value on the stack is often called **pushing** it, and loading it back is **popping** it.

## The 16-byte rule

The RISC-V calling convention says that `sp` must always be a multiple of 16.
So a frame is always 16, 32, 48, … bytes, even when you need less.
To save 5 words (20 bytes), you make a 32-byte frame and leave the rest unused.

## Saved registers

Registers `s0` to `s11` are the **saved registers**.
The calling convention says that a callee must leave them exactly as it found them.
So a value in an `s` register is still there after a `call`, whatever the callee is.

The price is that a function which uses an `s` register must save its old value on the stack first, and restore it before `ret`.
Here is the whole calling convention:

| Register      | Name           | After a `call`, it…                 | Who saves it                                        |
| ------------- | -------------- | ----------------------------------- | --------------------------------------------------- |
| `a0`–`a7`     | arguments      | may have changed (`a0` is the result) | the caller, if it needs the value later           |
| `t0`–`t6`     | temporaries    | may have changed                    | the caller, if it needs the value later             |
| `s0`–`s11`    | saved          | is unchanged                        | the callee, if it uses the register                 |
| `ra`          | return address | has changed                         | the callee, if it makes a call                      |
| `sp`          | stack pointer  | is unchanged                        | the callee, by giving back all the room it took     |

The `a` and `t` registers are **caller-saved**, and the `s` registers, `ra` and `sp` are **callee-saved**.

## Prologue and epilogue

A function that makes calls starts with a **prologue**: it makes its frame and saves `ra` and each `s` register it will use.
It ends with an **epilogue**: it restores them all, gives the frame back, and returns.

Here is the `triple_then_add` function from the functions concept, written the standard way:

```asm
# Returns a * 3 + b.
triple_then_add:
        addi    sp, sp, -16     # prologue: make a 16-byte frame
        sw      ra, 12(sp)      # save our return address
        sw      s0, 8(sp)       # save the caller's s0, because we use s0

        mv      s0, a1          # keep b in s0: it survives any call
        call    triple          # a0 = a * 3
        add     a0, a0, s0      # a0 = a * 3 + b

        lw      s0, 8(sp)       # epilogue: restore the caller's s0
        lw      ra, 12(sp)      # restore our return address
        addi    sp, sp, 16      # give the frame back
        ret
```

This version would still work if `triple` used every `t` register, or called other functions itself, or was written in C by somebody else.

Two rules keep the stack tidy:

- every value you push is popped, and every `addi sp, sp, -N` has a matching `addi sp, sp, N` before `ret`;
- every `s` register you change, and `ra`, is restored before `ret`.

A leaf function, one that makes no calls, does not need a frame at all, as long as it uses only `a` and `t` registers.

## Functions that build on functions

Because every function follows the same rules, functions **compose**: any function can call any other, without knowing how it works inside.
A function that calls a helper may itself be called by another function, which is called by another, and so on.
Each one has its own frame, below its caller's frame on the stack.

Python keeps a stack of calls in the same way.
When an error happens, the **traceback** lists that stack, from the first call down to the function that was running:

```python
def triple(n):
    return n * 3 + undefined_name

def triple_then_add(a, b):
    return triple(a) + b

triple_then_add(2, 5)
```

```text
Traceback (most recent call last):
  File "demo.py", line 7, in <module>
    triple_then_add(2, 5)
  File "demo.py", line 5, in triple_then_add
    return triple(a) + b
  File "demo.py", line 2, in triple
    return n * 3 + undefined_name
NameError: name 'undefined_name' is not defined
```

Each line pair in the traceback is one frame on Python's call stack.
In assembly, you build and remove the frames yourself.
