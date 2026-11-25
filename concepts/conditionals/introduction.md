# Introduction

So far, the processor has run every instruction in order, from the top of the function to `ret`.
To make a decision, like Python's `if`, the processor must be able to skip some instructions.

## Branches

A **branch** instruction compares two registers.
If the comparison is true, the processor jumps to a label and carries on from there.
If it is false, the processor carries on with the next instruction, as usual.
This is called **falling through**.

```asm
        beq     a0, a1, .Lsame          # if a0 == a1, jump to .Lsame
        li      a0, 0                   # only runs when a0 != a1
.Lsame:
        ret                             # runs in both cases
```

You met labels in the basics, where a label names the start of a function.
A label can name any place in the code, and a branch can jump to it.

There are four branch instructions:

| Instruction        | Jumps to `label` when |
| ------------------ | --------------------- |
| `beq  a, b, label` | `a == b`              |
| `bne  a, b, label` | `a != b`              |
| `blt  a, b, label` | `a < b`               |
| `bge  a, b, label` | `a >= b`              |

These treat the numbers as signed, so `-5` is less than `3`, just as in Python.

The assembler also understands some **pseudo-instructions**: shorter ways to write a branch, which it turns into one of the four above.

| Pseudo-instruction | Jumps to `label` when |
| ------------------ | --------------------- |
| `bgt  a, b, label` | `a > b`               |
| `ble  a, b, label` | `a <= b`              |
| `beqz a, label`    | `a == 0`              |
| `bnez a, label`    | `a != 0`              |
| `bltz a, label`    | `a < 0`               |
| `bgez a, label`    | `a >= 0`              |
| `blez a, label`    | `a <= 0`              |
| `bgtz a, label`    | `a > 0`               |

A branch can only compare two registers, or a register with zero.
To compare with any other number, load that number into a temporary register first:

```asm
        li      t0, 10                  # t0 = 10
        blt     a0, t0, .Lsmall         # if a0 < 10, jump to .Lsmall
```

## Jumps

`j label` always jumps to the label.
It does not compare anything.
You use it to skip over code that must not run, as in the `if`-`else` example below.

## Local labels

Every label in a file must have a different name.
A label that starts with `.L` is a **local label**: it can only be used inside this file, and the tests cannot see it.
Use local labels for branch targets.
A good habit is to start each one with the function's name, such as `.Lcap_done`, so that two functions never use the same name by mistake.

## If

Here is a Python `if` with no `else`, and the same thing in assembly:

```python
def cap(score):
    if score > 100:
        score = 100
    return score
```

```asm
cap:
        li      t0, 100                 # t0 = 100
        ble     a0, t0, .Lcap_done      # if score <= 100, skip the next line
        mv      a0, t0                  # score = 100
.Lcap_done:
        ret
```

Notice that the branch tests the **opposite** of the Python condition.
The branch jumps _past_ the body of the `if` when the body should not run.

## If-else

```python
def choose(flag, a, b):
    if flag != 0:
        return a
    else:
        return b
```

```asm
choose:
        beqz    a0, .Lchoose_else       # if flag == 0, go to the else part
        mv      a0, a1                  # the if part: result = a
        j       .Lchoose_done           # skip over the else part
.Lchoose_else:
        mv      a0, a2                  # the else part: result = b
.Lchoose_done:
        ret
```

Without the `j`, the processor would fall through into the `else` part as well.

A function may also have more than one `ret`.
Here the `j .Lchoose_done` could be replaced by `ret`.

## If-elif-else

A chain of `elif` tests is a chain of branches.
Each test either jumps to its own answer, or falls through to the next test.

```python
def bus_fare(age):
    if age < 5:
        return 0
    elif age < 18:
        return 2
    else:
        return 4
```

```asm
bus_fare:
        li      t0, 5                   # t0 = 5
        blt     a0, t0, .Lbus_fare_free # if age < 5, jump
        li      t0, 18                  # t0 = 18
        blt     a0, t0, .Lbus_fare_child # if age < 18, jump
        li      a0, 4                   # else: fare is 4
        ret
.Lbus_fare_free:
        li      a0, 0                   # fare is 0
        ret
.Lbus_fare_child:
        li      a0, 2                   # fare is 2
        ret
```
