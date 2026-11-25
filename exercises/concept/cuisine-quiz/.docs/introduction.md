# Introduction

A register holds a number as 32 bits.
A person wants to read that number as text, such as `"42"`.
Turning a number into text is called **formatting** it, and turning text back into a number is called **parsing** it.

Python does both for you:

```python
str(42)         # => '42'
int("42")       # => 42
f"{7:03}"       # => '007'
```

In assembly, you do it yourself, one digit at a time.

## Digits are characters

The text `"42"` is two bytes, `'4'` and `'2'`, followed by the zero terminator.
The characters `'0'` to `'9'` have the codes 48 to 57, in order.
So a digit from 0 to 9 becomes its character when you add `'0'`, and a character becomes its digit when you subtract `'0'`:

```asm
        addi    t1, t0, '0'     # t1 = the character for the digit in t0 (7 becomes '7')
        addi    t2, t1, -'0'    # t2 = the digit for the character in t1 ('7' becomes 7)
```

## Getting the digits out

Dividing by 10 splits a number into its last digit and the rest:

```python
n = 4096
n % 10      # => 6, the last digit
n // 10     # => 409, the other digits
```

Repeat this until nothing is left, and you get every digit, starting from the **last** one: 6, 9, 0, 4.
In assembly, use `remu` and `divu`, which treat the number as unsigned:

```asm
# Returns the sum of the decimal digits of n.
# n arrives in a0.
digit_sum:
        li      t0, 10                  # t0 = 10, the divisor
        li      t1, 0                   # t1 = total = 0
.Ldigit_sum_loop:
        remu    t2, a0, t0              # t2 = n % 10, the last digit
        add     t1, t1, t2              # total = total + digit
        divu    a0, a0, t0              # n = n // 10, the other digits
        bnez    a0, .Ldigit_sum_loop    # while n != 0, go round again
        mv      a0, t1                  # return total
        ret
```

This is a do-while loop, with its test at the bottom.
That matters when formatting: the number 0 has one digit, `"0"`, so the loop body must run at least once.
A loop with its test at the top would write nothing at all for 0.

## Putting the digits in order

The digits come out last-first, but the text must be first-last.
There are two common ways to fix this.

**Write, then reverse.**
Store each digit's character with `sb` as it comes out, moving forwards through the buffer.
The text is then backwards, such as `"6904"`, so reverse it in place.
To reverse, keep one pointer at the first character and one at the last.
Swap the two bytes, move the pointers towards each other, and stop when they meet:

```asm
# Reverses the characters from first to last, in place.
# first arrives in a0, last in a1.
reverse:
.Lreverse_loop:
        bgeu    a0, a1, .Lreverse_done  # stop when the pointers meet or cross
        lbu     t0, 0(a0)               # t0 = the byte at the front
        lbu     t1, 0(a1)               # t1 = the byte at the back
        sb      t1, 0(a0)               # put the back byte at the front
        sb      t0, 0(a1)               # put the front byte at the back
        addi    a0, a0, 1               # move the front pointer forwards
        addi    a1, a1, -1              # move the back pointer backwards
        j       .Lreverse_loop
.Lreverse_done:
        ret
```

**Count first, then write backwards.**
Count the digits first, with the same divide-by-10 loop.
Then you know where the text ends, so you can store the zero terminator there, and store each digit just before the previous one, moving backwards.

## Negative numbers

For a negative number, write a `'-'` first, then format its **magnitude**: the number without its sign.
`neg` gives the magnitude of a negative number, with one surprise.

The smallest `int32_t` is -2147483648, but the biggest is only 2147483647.
So `neg` of -2147483648 does not fit as a signed number, and gives -2147483648 again.
Its bits are `0x80000000`, which, read as **unsigned**, is 2147483648: the right magnitude.
So after `neg`, treat the magnitude as unsigned, and use `divu` and `remu`, never `div` and `rem`.

```asm
        li      t0, -2147483648         # t0 = 0x80000000
        neg     t0, t0                  # t0 = 0x80000000 again
        li      t1, 10                  # t1 = 10
        div     t2, t0, t1              # t2 = -214748364: wrong, div reads t0 as negative
        divu    t3, t0, t1              # t3 = 214748364: right, divu reads t0 as 2147483648
```

For every other negative number, `neg` gives a magnitude below 2147483648, and `divu` reads that correctly too.
So `neg` followed by `divu` and `remu` works for every negative number.

## Padding

Sometimes text must have a fixed **width**: a fixed number of characters.
Python's `f"{7:03}"` gives `'007'`: the number 7, **padded** to width 3 with leading zeros.
`f"{7:3}"` pads with spaces instead, giving `'  7'`.

If the number already has at least that many digits, nothing is added, and nothing is cut off:

```python
f"{1234:03}"    # => '1234'
```

## Parsing

Parsing goes the other way.
Read the characters from the left, and for each digit, multiply what you have so far by 10 and add the new digit:

```python
n = 0
for ch in "305":
    n = n * 10 + (ord(ch) - ord('0'))
# n goes 0, 3, 30, 305
```

One step of this, in assembly, is:

```asm
        addi    t1, t1, -'0'    # t1 = the digit for the character in t1
        mul     a0, a0, t0      # n = n * 10 (t0 holds 10)
        add     a0, a0, t1      # n = n + digit
```

Use the one-compare range check from the strings concept to tell whether a character is a digit at all.
