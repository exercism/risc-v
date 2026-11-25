# Introduction

A branch lets your code jump to a different place depending on a comparison.
Often you do not want to jump at all.
You only want the answer to a question: is this smaller than that?
RISC-V has instructions that write the answer into a register.

## Booleans are 0 and 1

A **boolean** is a value that is either true or false.
Assembly has no special boolean type.
Instead, a register holds **1** for true and **0** for false.
C does the same thing, and so does Python underneath:

```python
print(int(3 < 5))   # 1
print(int(5 < 3))   # 0
print(True == 1)    # True
```

The functions in this exercise return an `int32_t` that must be exactly 0 or 1.

## Set if less than

`slt` means "**s**et if **l**ess **t**han".
It compares two registers, and writes 1 if the first is smaller than the second, or 0 if it is not.
`slti` does the same with an immediate.

```asm
slt     t0, a0, a1      # t0 = 1 if a0 < a1, else 0
slti    t1, a0, 10      # t1 = 1 if a0 < 10, else 0
```

```python
t0 = int(a0 < a1)
t1 = int(a0 < 10)
```

There is also `sltu` ("set if less than, unsigned").
It compares the registers as **unsigned** numbers, which a later concept explains.
For now, you only need it through the pseudo-instructions below.

## Pseudo-instructions for comparisons

The assembler gives you some easier names.
Each one becomes a single `slt`, `sltu` or `sltiu` instruction.

```asm
seqz    t0, a0          # t0 = 1 if a0 == 0, else 0   (set if equal to zero)
snez    t0, a0          # t0 = 1 if a0 != 0, else 0   (set if not equal to zero)
sltz    t0, a0          # t0 = 1 if a0 < 0, else 0    (set if less than zero)
sgtz    t0, a0          # t0 = 1 if a0 > 0, else 0    (set if greater than zero)
sgt     t0, a0, a1      # t0 = 1 if a0 > a1, else 0   (set if greater than)
```

There is no instruction that compares two registers for equality.
Instead, subtract one from the other, and check whether the difference is zero.
To check whether `a0` equals 7:

```asm
addi    t0, a0, -7      # t0 = a0 - 7, which is 0 only when a0 == 7
seqz    t0, t0          # t0 = 1 if a0 == 7, else 0
```

## Combining booleans

When two registers each hold 0 or 1, these instructions combine them:

```asm
and     t2, t0, t1      # t2 = 1 if t0 and t1 are both 1, else 0
or      t2, t0, t1      # t2 = 1 if t0 or t1 (or both) is 1, else 0
xori    t2, t0, 1       # t2 = 1 if t0 is 0, and 0 if t0 is 1 ("not")
```

```python
t2 = t0 and t1
t2 = t0 or t1
t2 = not t0
```

These instructions work on every bit of a register, which a later concept explains.
For booleans that are 0 or 1, they behave exactly like the table above.
They only give 0 or 1 if their inputs are 0 or 1, so turn any other number into a boolean first, for example with `snez`.

To check "is `a0` at least 10?", ask the opposite question and then flip the answer:

```asm
slti    t0, a0, 10      # t0 = 1 if a0 < 10
xori    t0, t0, 1       # t0 = 1 if a0 >= 10
```

You could also answer these questions with branches.
Comparison instructions are often shorter, and they never jump.
