# About

A number in a register is 32 bits.
Text is a row of character codes.
**Formatting** turns the first into the second, and **parsing** turns the second back into the first.

| Python          | Result    | In assembly                                 |
| --------------- | --------- | ------------------------------------------- |
| `str(305)`      | `'305'`   | divide by 10 again and again, add `'0'`     |
| `str(-15)`      | `'-15'`   | write `'-'`, then format the magnitude      |
| `f"{7:03}"`     | `'007'`   | pad with `'0'` on the left up to the width  |
| `f"{7:3}"`      | `'  7'`   | pad with `' '` on the left up to the width  |
| `hex(255)`      | `'0xff'`  | divide by 16, or shift right by 4           |
| `int("305")`    | `305`     | `n = n * 10 + (c - '0')` for each digit     |

## Digit characters

The digits `'0'` to `'9'` have the ASCII codes 48 to 57, in order.

- digit to character: `addi t1, t0, '0'`
- character to digit: `addi t0, t1, -'0'`

Write `'0'` rather than 48, so that a reader can see what the number means.

## Formatting an unsigned number

`n % 10` is the last decimal digit of `n`, and `n // 10` is the number made by the others.
Repeating these gives every digit, from the last to the first.
Use `remu` and `divu` for a `uint32_t`: `rem` and `div` would read numbers from 2147483648 upwards as negative.

The loop must run at least once, so that 0 gives `"0"`.
Test at the bottom, as a do-while loop, and stop when the quotient is 0.

A `uint32_t` has at most 10 decimal digits (4294967295).
An `int32_t` needs at most 11 characters (`"-2147483648"`).
With the zero terminator, a 12-byte buffer is always enough for either.

### Putting the digits in order

The digits come out in the wrong order.
Three common ways to deal with that are:

1. **Write forwards, then reverse in place.**
   Store each digit at the next byte of the buffer.
   Then swap the first and last characters, the second and second-last, and so on, until the two pointers meet.
2. **Count first, then write backwards.**
   A first loop divides by 10 just to count the digits.
   Then the end of the text is known: store the terminator there, and store each digit one byte before the last.
3. **Write backwards into a scratch buffer, then copy forwards.**
   Start at the end of a buffer that is big enough for any number, such as 12 bytes on the stack.
   Store each digit one byte before the last, then copy the finished text, from wherever it starts, into the caller's buffer.

The C library's `printf` usually uses the third way.
All three give the same text.

### Dividing by 10 without `divu`

Division is one of the slowest instructions on many processors.
Compilers often replace `n / 10` by a multiplication by a "magic" constant, followed by a shift, using the upper half of the 64-bit product from `mulhu`.
For example, for any `uint32_t n`, `n / 10` is the upper 32 bits of `n * 0xCCCCCCCD`, shifted right by 3.
You do not need this on this track, but it explains the odd constants you may see in compiled code.

## Negative numbers

To format a signed number:

1. If it is negative, write `'-'`, and replace the number by its negation with `neg`.
2. Format what is left as an **unsigned** number.

Step 2 must be unsigned because of -2147483648 (`0x80000000`).
Its negation, 2147483648, does not fit in an `int32_t`, so `neg` gives `0x80000000` again.
Read as signed, that is still negative, and `div`/`rem` would give negative "digits".
Read as unsigned, it is exactly 2147483648, so `divu`/`remu` give the right digits.
For every other negative number the magnitude is at most 2147483647, which reads the same either way.

Python has no such problem, because its numbers have no fixed size.

## Padding

A **width** is the smallest number of characters the text should take.
If the number has fewer digits, extra characters are added on the left:

- leading zeros, as in `f"{7:03}"` giving `'007'`, which is common for codes and times such as `"09:05"`;
- leading spaces, as in `f"{7:3}"` giving `'  7'`, which lines numbers up in a column.

If the number already has at least `width` digits, nothing is added and nothing is cut off.

When you count the digits first and write backwards, zero padding is free.
Write exactly `width` characters with the same `remu`/`divu` loop: once the number has run out of digits, the remainder is 0, and `'0'` is written.
For space padding, write the digits, then fill the remaining places with `' '`.

For a negative number padded with zeros, the `'-'` goes before the zeros: Python's `f"{-7:04}"` is `'-007'`.
With spaces, the `'-'` goes after the spaces: `f"{-7:4}"` is `'  -7'`.

## Other bases

The same method works in any base: divide by the base instead of by 10.

| Base | Name        | Digits                         | Python     |
| ---- | ----------- | ------------------------------ | ---------- |
| 2    | binary      | `0` `1`                        | `bin(n)`   |
| 8    | octal       | `0` to `7`                     | `oct(n)`   |
| 10   | decimal     | `0` to `9`                     | `str(n)`   |
| 16   | hexadecimal | `0` to `9`, then `a` to `f`    | `hex(n)`   |

Hexadecimal digits 10 to 15 are letters, so a digit becomes a character in two different ways:

```asm
        li      t2, 10
        bltu    t0, t2, .Ldecimal_digit # 0 to 9
        addi    t1, t0, 'a' - 10        # 10 becomes 'a', 15 becomes 'f'
        j       .Ldigit_done
.Ldecimal_digit:
        addi    t1, t0, '0'             # 0 becomes '0', 9 becomes '9'
.Ldigit_done:
```

A lookup table of the sixteen characters, such as `"0123456789abcdef"` in the data section, does the same job with a single `lbu`.

When the base is a power of two, you do not need division at all.
A hex digit is 4 bits, so `andi t0, n, 0xF` gives the last digit, and `srli n, n, 4` removes it.
A `uint32_t` is always exactly 8 hex digits, so a hex formatter often writes all 8, with leading zeros, starting from the top: shift right by 28, 24, …, 0 and keep the lowest 4 bits each time.
Then the digits come out in the right order, and no reversing is needed.

## Parsing

Parsing reads the characters from the left:

```python
def parse(text):
    n = 0
    for ch in text:
        if not '0' <= ch <= '9':
            break
        n = n * 10 + (ord(ch) - ord('0'))
    return n
```

In assembly, subtract `'0'` from each character and compare the result with 10 as unsigned.
Anything that is not a digit, including the zero terminator, gives something that is not below 10, so one comparison ends the loop.

For a signed number, check for a leading `'-'` (and perhaps `'+'`), skip it, parse the digits, and `neg` the result at the end if there was a `'-'`.
The digits of `"-2147483648"` give 2147483648, which is `0x80000000`, and `neg` turns that into -2147483648, as wanted.

### Overflow

`mul` and `add` wrap around, so a string with too many digits, such as `"99999999999"`, gives a wrong answer with no error.
Python's `int` never overflows.
C's `strtoul` detects overflow and returns the biggest value instead.
To detect it yourself, check before multiplying that `n` is at most 429496729, and after adding that the sum did not wrap (that it is not less than what you added), or use `mulhu` to check the upper half of the product is 0.

### Other bases

To parse hexadecimal, multiply by 16 (or shift left by 4) instead of 10, and accept `'a'` to `'f'` and `'A'` to `'F'` as the digits 10 to 15.
