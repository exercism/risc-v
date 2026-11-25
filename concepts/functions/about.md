# About

A **function** (also called a **subroutine** or **procedure**) is a piece of code that other code can jump to, and that jumps back when it is done.
The code that jumps to it is the **caller**, and the function itself is the **callee**.

## Jump and link

RISC-V has two jump instructions that remember where they came from:

| Instruction           | Meaning                                                          |
| --------------------- | ---------------------------------------------------------------- |
| `jal rd, label`       | `rd` = address of the next instruction, then jump to `label`     |
| `jalr rd, offset(rs)` | `rd` = address of the next instruction, then jump to `rs + offset` |

Saving the address of the next instruction is called **linking**, and `rd` is the **link register**.
When `rd` is `zero`, the address is thrown away, and the instruction is a plain jump.

Many of the jumps you write are pseudo-instructions for these two:

| You write      | The assembler produces                              |
| -------------- | --------------------------------------------------- |
| `j label`      | `jal zero, label`                                   |
| `jal label`    | `jal ra, label`                                     |
| `call label`   | `jal ra, label`, or `auipc ra, …` then `jalr ra, …(ra)` |
| `tail label`   | `jal zero, label`, or `auipc t1, …` then `jalr zero, …(t1)` |
| `jr rs`        | `jalr zero, 0(rs)`                                  |
| `jalr rs`      | `jalr ra, 0(rs)`                                    |
| `ret`          | `jalr zero, 0(ra)`                                  |

`jal` can only reach labels within about 1 MiB of itself.
`call` can reach anywhere: the assembler writes the two-instruction `auipc` and `jalr` form, and the linker shrinks it to a single `jal` when the target is close enough.
This shrinking is called **linker relaxation**.

## The return address register

The calling convention names `x1` as `ra`, the return address.
`call` writes it and `ret` reads it.
Because there is only one `ra`, a function that calls another function overwrites the address it must return to.
Every function that calls another must keep its own `ra` somewhere first.

## The calling convention

The RISC-V calling convention divides the registers into two groups.

| Registers               | Who must keep them                                      |
| ----------------------- | ------------------------------------------------------- |
| `a0`–`a7`, `t0`–`t6`, `ra` | **caller-saved**: a callee may change them freely   |
| `s0`–`s11`, `sp`        | **callee-saved**: a callee must put them back as it found them |

Arguments go in `a0` to `a7`, and the result comes back in `a0` (and `a1`, for a value too big for one register).

A caller that needs a value after a call must either keep it in a callee-saved register, or save it in memory on the stack.
This works whoever wrote the callee, because every function follows the same rules.

## The leaf-helper shortcut

When you write both the caller and a small helper yourself, you know exactly which registers the helper changes.
You can then keep values, and even `ra`, in temporary registers the helper does not touch:

```asm
        mv      t6, ra          # keep our return address
        call    helper          # helper is known not to touch t6
        mv      ra, t6
        ret
```

Or you can call the helper with a different link register, so `ra` is never changed:

```asm
        jal     t5, helper      # return address in t5
        # ...
helper:
        # ...
        jr      t5              # go back
```

This is fast, because nothing is stored in memory.
Compilers do something like it for functions inside one file, when they can see all of the code.
But it is fragile:

- if the helper is changed later to use another register, the caller silently breaks;
- it cannot be used for a function written by someone else, such as a C library function;
- a helper called with `jal t5` cannot call anything itself without losing `t5`.

## Leaf and non-leaf functions

A **leaf** function calls no other function, so it never needs to keep `ra`.
A **non-leaf** function does call others, and the standard way for it to keep `ra` is to save it on the stack when it starts, and load it back before it returns.
The stack is the next concept.

## Symbols

A label marked with `.global` becomes a symbol that other files can link to.
A plain label is a local symbol: other files cannot call it, so two files may each have a helper with the same name.
Labels that start with `.L` are not even put in the symbol table, which is why they suit branch targets.

## Tail calls

When the very last thing a function does is call another function and return its result, it can jump to it instead, with `tail helper`.
The helper then returns straight to the original caller, using the `ra` that was never changed.
This is called a **tail call**.
