# Introduction

A **loop** runs the same instructions again and again.
RISC-V has no special loop instruction.
Instead, you build a loop from two things you already know: a label, and a branch that jumps **backward** to it.

## A counting loop

In Python, you might multiply a number by itself several times like this:

```python
def power(base, exponent):
    result = 1
    for i in range(exponent):
        result = result * base
    return result
```

In assembly, you keep the loop counter `i` in a temporary register, and add 1 to it each time round:

```asm
# Returns base raised to the power exponent.
# base arrives in a0, exponent arrives in a1.
power:
        li      t0, 1           # t0 = result = 1
        li      t1, 0           # t1 = i = 0
power_loop:
        bge     t1, a1, power_done  # if i >= exponent, leave the loop
        mul     t0, t0, a0      # result = result * base
        addi    t1, t1, 1       # i = i + 1
        j       power_loop      # go back to the test
power_done:
        mv      a0, t0          # return result
        ret
```

Each time the processor goes round the loop is called an **iteration**.
The register `t0` is an **accumulator**: it starts with a value before the loop, and each iteration updates it.
The register `t1` is the **loop counter**.
Temporary registers `t0` to `t6` are a good place for both.

Each loop needs its own label names, and no two labels in a file may have the same name.
Starting each label with the function's name, as in `power_loop`, keeps them apart.

## Test at the top: a while loop

The loop above tests its condition **before** each iteration, like a Python `while` loop.
If the condition is false at the start, the body never runs at all.
For example, `power(5, 0)` skips the loop and returns 1.

Here is another loop with the test at the top.
It takes 7 away from a number until what is left is less than 7:

```python
while n >= 7:
    n = n - 7
```

```asm
        li      t0, 7           # t0 = 7
sevens_loop:
        blt     a0, t0, sevens_done  # if n < 7, leave the loop
        addi    a0, a0, -7      # n = n - 7
        j       sevens_loop     # go back to the test
sevens_done:
```

The shape is always the same:

1. a label at the top;
2. a branch that **leaves** the loop when the condition is false;
3. the body;
4. a `j` back to the label at the top.

## Test at the bottom: a do-while loop

Sometimes the body must run at least once.
Then you can put the test at the **bottom** of the loop, and branch backward while the condition is still true.
This is called a **do-while** loop.
It is shorter, because it needs only one branch and no `j`.

Python has no do-while loop, but you can write one with `while True` and `break`.
This finds the first multiple of 5 that is bigger than `n`:

```python
m = 0
while True:
    m = m + 5
    if not (m <= n):
        break
```

```asm
        li      t0, 0           # t0 = m = 0
fives_loop:
        addi    t0, t0, 5       # m = m + 5
        ble     t0, a0, fives_loop  # while m <= n, go round again
        mv      a0, t0          # return m
```

## Beware of infinite loops

If the condition that ends a loop never becomes true, the loop runs forever.
This is called an **infinite loop**.
Your function never reaches `ret`, so the tests never finish, and the test runner eventually stops them with a timeout.

If that happens, check that:

- the loop counter or the value being tested changes on every iteration;
- it changes in the right direction, towards the exit condition;
- the branch compares the registers you meant it to compare.
