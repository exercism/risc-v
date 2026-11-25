# About

RISC-V has no loop instructions.
A loop is a label followed by some instructions, with a branch or jump at the end that goes **backward** to the label.
Every kind of loop you know from Python or JavaScript can be built this way.

## While loops: test at the top

```python
while condition:
    body
```

becomes

```asm
loop:
        # branch to done if the condition is FALSE
        # body
        j       loop
done:
```

The branch at the top uses the **opposite** of the Python condition, because it decides when to leave.
For `while i < n`, the branch is `bge i, n, done`.
For `while n != 0`, the branch is `beqz n, done`.

If the condition is false the first time, the body runs zero times.

## Do-while loops: test at the bottom

```asm
loop:
        # body
        # branch to loop if the condition is TRUE
```

The body always runs at least once.
Each iteration runs one branch and no `j`, so this shape is a little faster.

A common trick is to **rotate** a while loop into a do-while loop, guarded by one test before it starts:

```asm
        # branch to done if the condition is FALSE
loop:
        # body
        # branch to loop if the condition is TRUE
done:
```

Compilers often produce this shape.

## Counting loops

Python's `for i in range(n)` counts `i` up from 0 to `n - 1`:

```asm
        li      t0, 0           # i = 0
loop:
        bge     t0, a0, done    # stop when i >= n
        # body, which may use i
        addi    t0, t0, 1       # i = i + 1
        j       loop
done:
```

When the body does not need `i`, counting **down** to zero saves a register, because `beqz` and `bnez` compare with `zero` directly:

```asm
        mv      t0, a0          # t0 = n
loop:
        beqz    t0, done        # stop when no iterations are left
        # body
        addi    t0, t0, -1      # one fewer left
        j       loop
done:
```

## Accumulators

An **accumulator** is a register that starts with a value before the loop and is updated on every iteration.
A sum starts at 0, a product starts at 1, and a count starts at 0.

## Working through the digits of a number

`rem` by 10 gives the last decimal digit of a non-negative number, and `div` by 10 removes it.
Repeating these until the number reaches zero visits every digit, from the last to the first.
Both instructions need 10 in a register, so load it once, before the loop, rather than on every iteration.

## Choosing registers

Loop counters, accumulators and constants such as 10 normally live in the temporary registers `t0`–`t6`, or in argument registers whose original value is no longer needed.
A function that calls no other functions may change all of these freely.

## Label names

Labels are names for places in the file, and each one must be unique within the file.
Prefixing loop labels with the function's name, such as `power_loop` and `power_done`, avoids clashes.

The assembler also accepts **numeric local labels**, such as `1:`, which may be repeated.
A branch refers to the nearest one before it with `1b` (backward) or after it with `1f` (forward):

```asm
1:
        addi    t0, t0, -1
        bnez    t0, 1b          # back to the nearest "1:" above
```

Labels that start with `.L`, such as `.Lloop`, are kept out of the object file's symbol table, so they do not appear in debuggers.

## Infinite loops

A loop whose exit condition never becomes true runs forever.
Common causes are:

- forgetting to update the loop counter;
- updating it in the wrong direction, such as `addi t0, t0, 1` when counting down;
- branching back to a label that is before the instructions that set the counter up, so the counter is reset on every iteration;
- comparing the wrong registers.

Numbers in registers wrap around at 32 bits, so a loop such as "double until bigger than `n`" can also run forever, or give a wrong answer, when `n` is close to the largest 32-bit value.
