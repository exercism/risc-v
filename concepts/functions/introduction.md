# Introduction

In Python you often split a big job into small functions, and one function calls another.
Assembly works the same way.
The small function is called a **helper**.

The function that makes the call is the **caller**.
The function being called is the **callee**.
So far the tests have been the caller, and your functions have been the callee.
Now your own functions will call each other.

## Calling a function

`call` jumps to a label, just like `j`.
But first it remembers where to come back to.

```asm
        call    triple          # run triple, then come back here
        addi    a0, a0, 1       # this runs after triple returns
```

The place to come back to is the address of the instruction just after the `call`.
It is called the **return address**.
`call` puts it in a special register, `ra`, then jumps to the label.

When the helper reaches `ret`, the processor jumps to the address in `ra`.
So `ret` means "go back to whoever called me".

Arguments and results work exactly as they do for the tests:

- the caller puts the arguments in `a0`, `a1`, `a2`, … before the `call`;
- the callee leaves its result in `a0` before the `ret`;
- after the `call`, the caller finds the result in `a0`.

Here is a helper, and the same thing in Python:

```asm
# Returns n * 3.
triple:
        li      t0, 3           # t0 = 3
        mul     a0, a0, t0      # a0 = n * 3
        ret                     # go back to the address in ra
```

```python
def triple(n):
    return n * 3
```

## Helpers that the tests cannot see

A function needs `.global` only if code in another file calls it, as the tests do.
A helper used only inside your own file can be a plain label, with no `.global`.
The tests cannot see it or call it.

## Keeping your own return address

There is only one `ra` register.
When the tests call your function, `ra` holds the address to go back to in the tests.
If your function then does a `call`, the `call` writes a **new** address into `ra`, and the old one is lost.
Your function's own `ret` would then jump back to just after its `call`, and run the same code again, for ever.

So a function that calls another function must keep its own `ra` safe.
For now, copy it into a temporary register that the helper does not use, and copy it back before `ret`:

```asm
# Returns n * 9, by tripling twice.
nine_times:
        mv      t6, ra          # keep our return address in t6
        call    triple          # a0 = n * 3
        call    triple          # a0 = n * 9
        mv      ra, t6          # put our return address back
        ret                     # go back to the tests
```

This works only because **you** wrote `triple`, and you know it does not touch `t6`.

## The calling convention

The tests on this track, and all C code, follow a set of rules called the **calling convention**.
One of the rules is this: a callee may change `a0` to `a7` and `t0` to `t6` freely, without putting them back.

So after a `call`, the caller must think of all of those registers as changed, unless it knows better.
In this exercise you do know better, because you write every helper yourself.
A value you need after a call must be in a register the helper does not touch:

```asm
# Returns a * 3 + b.
triple_then_add:
        mv      t6, ra          # keep our return address
        mv      t5, a1          # keep b: triple does not touch t5
        call    triple          # a0 = a * 3 (and t0 is changed)
        add     a0, a0, t5      # a0 = a * 3 + b
        mv      ra, t6          # put our return address back
        ret
```

If `b` had been kept in `t0` instead, `triple` would have overwritten it with 3.

## Choosing the link register

`call` and `ret` are pseudo-instructions.
`call label` is really `jal ra, label`: **jump and link**.
It puts the return address in `ra` (the **link register**), then jumps.
`ret` is really `jalr zero, 0(ra)`: jump to the address in `ra`.

You can name a different link register.
For a small helper, `jal t5, label` keeps the return address in `t5`, and leaves `ra` alone.
The helper then goes back with `jr t5`, instead of `ret`:

```asm
# Returns n * 2. Called with jal t5, so it returns with jr t5.
double_it:
        add     a0, a0, a0      # a0 = n * 2
        jr      t5              # go back to the address in t5

# Returns n * 4.
four_times:
        jal     t5, double_it   # a0 = n * 2, and ra is not changed
        jal     t5, double_it   # a0 = n * 4
        ret                     # ra still holds the tests' address
```

A helper written this way must always be called with `jal t5`, never with `call`.
It must not call anything itself, because it has nowhere to keep `t5`.
A function that calls nothing is called a **leaf** function.

## Only a shortcut, for now

Keeping `ra` and other values in temporary registers is a shortcut.
It works only for small helpers that you wrote yourself, so that you know which registers they change.
It does not work if the helper is someone else's code, or if it calls other functions in turn.

The standard way is to save `ra` and other values in memory, on the **stack**, and to use the **saved registers** `s0` to `s11`, which a callee must put back as it found them.
That is the next concept.
