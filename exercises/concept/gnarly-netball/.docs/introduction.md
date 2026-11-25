# Introduction

RISC-V has instructions to multiply and divide whole numbers.
They work on registers, just like `add` and `sub`.

## Multiplying

```asm
mul     a0, a1, a2      # a0 = a1 * a2
```

There is no `muli` instruction with an immediate.
To multiply by a fixed number, load it into a register first:

```asm
li      t0, 10          # t0 = 10
mul     a0, a0, t0      # a0 = a0 * 10
```

A register holds only 32 bits.
If the true answer is too big to fit, `mul` keeps the lowest 32 bits and throws the rest away.
The result **wraps around**, with no error, just as `add` does.

## Dividing

Division gives two answers: the **quotient** (how many times one number goes into the other) and the **remainder** (what is left over).
RISC-V has one instruction for each:

```asm
div     a0, a1, a2      # a0 = a1 / a2, the quotient
rem     a0, a1, a2      # a0 = the remainder of a1 / a2
```

For example, 17 divided by 5 is 3, with 2 left over:

```asm
li      t0, 17          # t0 = 17
li      t1, 5           # t1 = 5
div     t2, t0, t1      # t2 = 3
rem     t3, t0, t1      # t3 = 2
```

There is no `divi` or `remi` either, so a fixed divisor also goes in a register first.

### Negative numbers

`div` always gives a whole number.
It throws away any fraction, so the quotient moves **towards zero**.
This is called **truncating**.
`rem` gives a remainder with the same sign as the number being divided (the **dividend**).

Python's `//` and `%` are different.
`//` rounds **down**, towards minus infinity, and `%` gives a result with the same sign as the divisor.
For positive numbers they agree with `div` and `rem`, but for negative numbers they do not:

| Calculation     | RISC-V `div` / `rem` | Python `//` / `%` |
| --------------- | -------------------- | ----------------- |
| 7 divided by 2  | 3, remainder 1       | 3, remainder 1    |
| -7 divided by 2 | -3, remainder -1     | -4, remainder 1   |

In Python, `int(a / b)` truncates towards zero like `div` does:

```python
-7 // 2       # => -4  (rounds down)
int(-7 / 2)   # => -3  (truncates towards zero, like div)
```

With `div` and `rem`, this is always true:

```
dividend = quotient * divisor + remainder
```

### Dividing by zero

In Python, dividing by zero raises a `ZeroDivisionError`.
In RISC-V, the program does not stop and there is no error.
Instead:

- `div` by zero gives -1;
- `rem` by zero gives the dividend, unchanged.

### Unsigned division

`div` and `rem` treat their operands as **signed** numbers, which may be negative.
There are also `divu` and `remu`, which treat their operands as unsigned numbers, which are never negative.
A later concept covers unsigned numbers.
