# Introduction

A register holds 32 bits, so the biggest unsigned number it can hold is 4294967295.
Some numbers need more room than that.
For example, a clock that counts **microseconds** (millionths of a second) reaches 4294967295 after only 71 minutes and 35 seconds.

C has 64-bit types for numbers like these: `uint64_t` (unsigned) and `int64_t` (signed).
An unsigned 64-bit number can be anything from 0 to 2⁶⁴ - 1, which is 18446744073709551615.
On RV32, the 32-bit RISC-V used on this track, such a number does not fit in one register, so it is kept in **two**.

## High and low halves

A 64-bit number is split into two 32-bit halves:

- the **low half** holds the lowest 32 bits;
- the **high half** holds the highest 32 bits.

The number is `high * 2**32 + low`.
In hexadecimal the split is easy to see, because each half is 8 hexadecimal digits:

| 64-bit number          | High half    | Low half     |
| ---------------------- | ------------ | ------------ |
| `0x0000000000000005`   | `0x00000000` | `0x00000005` |
| `0x00000001FFFFFFFF`   | `0x00000001` | `0xFFFFFFFF` |
| `0x0006610A409C4000`   | `0x0006610A` | `0x409C4000` |

```python
x = 0x00000001FFFFFFFF
low = x % 2**32     # 0xFFFFFFFF
high = x // 2**32   # 0x00000001
```

## Passing 64-bit values to a function

A 64-bit argument uses **two** argument registers, one after the other.
The low half goes in the first one, and the high half in the second.
A 32-bit argument still takes one register, the next one free.

| C declaration                                  | Registers                                   |
| ---------------------------------------------- | ------------------------------------------- |
| `f(uint64_t x)`                                | `x`: low in `a0`, high in `a1`              |
| `f(uint64_t x, uint64_t y)`                    | `x` in `a0` and `a1`, `y` in `a2` and `a3`  |
| `f(uint64_t x, uint32_t n)`                    | `x` in `a0` and `a1`, `n` in `a2`           |
| `f(uint32_t n, uint64_t x)`                    | `n` in `a0`, `x` in `a1` and `a2`           |

A 64-bit result is returned the same way: the low half in `a0` and the high half in `a1`.
So a function that returns a `uint64_t` must set **both** `a0` and `a1` before it reaches `ret`.

## Adding with a carry

When you add two numbers on paper, a column that adds up to more than 9 carries 1 into the next column.
Adding 64-bit numbers works the same way, with 32-bit halves instead of digits.
Add the two low halves, and if that wraps around, add 1 more to the high half.

How can you tell that the low addition wrapped around?
An unsigned sum that does not wrap is never smaller than what you added.
A sum that wraps is always smaller.
So `sltu` gives the **carry**: 1 if the sum wrapped, and 0 if it did not.

Here `t0`/`t1` holds one 64-bit number (low, high), `t2`/`t3` holds another, and the sum goes in `t4`/`t5`:

```asm
        add     t4, t0, t2              # low half of the sum (it may wrap)
        sltu    t6, t4, t2              # carry = 1 if the low sum wrapped, else 0
        add     t5, t1, t3              # high half of the sum
        add     t5, t5, t6              # add the carry to the high half
```

The same check works when you add a 32-bit number to a 64-bit one.
The 32-bit number has a high half of 0, so there is nothing to add to the high half except the carry.

## Subtracting with a borrow

Subtracting works the same way, with a **borrow** instead of a carry.
If the low half you subtract is bigger than the low half you subtract it from, the low result wraps around, and you must take 1 more from the high half.
Work out the borrow **before** you overwrite either low half.

```asm
        sltu    t6, t0, t2              # borrow = 1 if low of t0 < low of t2
        sub     t4, t0, t2              # low half of the difference (it may wrap)
        sub     t5, t1, t3              # high half of the difference
        sub     t5, t5, t6              # take away the borrow
```

## Comparing 64-bit values

To compare two 64-bit numbers, compare the high halves first.
If they are different, they decide the answer, whatever the low halves are.
Only when the high halves are equal do you need to compare the low halves.

The low halves are always compared as **unsigned**, even for `int64_t`, because the sign lives in the high half.
The high halves are compared as unsigned for `uint64_t`, and as signed for `int64_t`.

Here is a check whether one `int64_t` is greater than another, with the first in `a0`/`a1` and the second in `a2`/`a3`:

```asm
is_greater:
        bne     a1, a3, .Lis_greater_high   # if the high halves differ, they decide
        sltu    a0, a2, a0                  # equal high halves: compare low halves, unsigned
        ret
.Lis_greater_high:
        slt     a0, a3, a1                  # compare high halves, signed for int64_t
        ret
```

Two 64-bit numbers are equal only when both halves are equal.

## Multiplying to a 64-bit result

Multiplying two 32-bit numbers can give an answer up to 64 bits long.
You have seen that `mul` keeps only the low 32 bits.
Another instruction gives you the high 32 bits of the same product:

| Instruction          | Gives                                                      |
| -------------------- | ---------------------------------------------------------- |
| `mul    d, a, b`     | the low half of `a * b` (the same for signed and unsigned) |
| `mulhu  d, a, b`     | the high half of `a * b`, with both read as unsigned       |
| `mulh   d, a, b`     | the high half of `a * b`, with both read as signed         |
| `mulhsu d, a, b`     | the high half of `a * b`, with `a` signed and `b` unsigned |

Together, `mul` and the right `mulh` instruction give the whole 64-bit product:

```asm
        mulhu   t3, t0, t1              # t3 = high half of t0 * t1, unsigned
        mul     t2, t0, t1              # t2 = low half of t0 * t1
```

Make sure the first instruction does not overwrite `t0` or `t1` before the second one reads them.

## Comparing with Python

Python's integers never overflow, so Python gives the true answer.
A `uint64_t` wraps around at 2⁶⁴, just as a `uint32_t` wraps at 2³².
To get the same answer as `uint64_t` in Python, use `% 2**64`:

```python
(0xFFFFFFFFFFFFFFFF + 1) % 2**64    # 0
(0 - 1) % 2**64                     # 18446744073709551615
```
