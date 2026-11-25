# Introduction

A **string** is a piece of text, such as a name or a line from a play.
In Python a string is a special kind of value.
In assembly, a string is just an array of bytes in memory.

## Characters are numbers

Each character is stored as one byte: a number from 0 to 255.
The number for each character is given by **ASCII**, a table that every computer agrees on.
Here are some of its codes:

| Characters     | Codes      |
| -------------- | ---------- |
| space          | 32         |
| `!`            | 33         |
| `/`            | 47         |
| `0` to `9`     | 48 to 57   |
| `A` to `Z`     | 65 to 90   |
| `a` to `z`     | 97 to 122  |

The letters are in order, so `B` is 66, `C` is 67, and so on.

Python's `ord` gives the code of a character, and `chr` gives the character for a code:

```python
ord('A')    # 65
ord('a')    # 97
chr(66)     # 'B'
```

## Character literals

You do not need to remember the table.
The assembler understands a character in single quotes, and turns it into its code.
This is called a **character literal**.
You can use one anywhere a number would go, and even do arithmetic with it:

```asm
        li      t0, 'A'         # t0 = 65
        li      t1, '/'         # t1 = 47
        addi    t2, a0, -'0'    # t2 = a0 - 48
        li      t3, 'Z' - 'A'   # t3 = 25
```

Do not write `'\0'` for the zero byte.
This assembler reads it as the character `'0'`, which is 48.
Use `zero`, `0`, or an instruction such as `beqz` instead.

## Loading and storing bytes

You met `lw` and `sw`, which load and store 4-byte words.
For single bytes, use these instead:

```asm
        lbu     t0, 0(a0)       # t0 = the byte at address a0, from 0 to 255
        lbu     t1, 3(a0)       # t1 = the byte at address a0 + 3
        sb      t0, 1(a1)       # the byte at address a1 + 1 = the lowest 8 bits of t0
```

- `lbu` means **l**oad **b**yte **u**nsigned.
  It fills the other 24 bits of the register with zeros.
- `sb` means **s**tore **b**yte.
  It writes only the lowest 8 bits of the register, and leaves the bytes next to it alone.

Each character takes 1 byte, so the next character is 1 byte further on.
A pointer walks through a string with `addi p, p, 1`, not 4.
Bytes do not need to be aligned, so any address is fine.

### Why not `lb`?

There is also `lb`, **l**oad **b**yte, without the `u`.
It reads the byte as a **signed** number from -128 to 127, and copies its top bit into the other 24 bits.
This is sign extension, which you met with immediates.
For codes from 128 to 255, `lb` gives a negative number: the byte 233 becomes -23.
Characters are unsigned bytes from 0 to 255, so always use `lbu` for them.

## Strings end with a zero byte

In C, `const char *` is the address of the first character of a string.
Unlike the arrays you have seen, a string does not come with a length.
Instead, it ends with a byte that holds 0, called the **zero terminator**.
No character uses the code 0, so it cannot be confused with the text.

The string `"Hi!"` takes 4 bytes:

| Address  | `s`  | `s + 1` | `s + 2` | `s + 3` |
| -------- | ---- | ------- | ------- | ------- |
| Byte     | 72   | 105     | 33      | 0       |
| Meaning  | `H`  | `i`     | `!`     | end     |

The **length** of a string is the number of characters before the zero, so `"Hi!"` has length 3.
The **empty string** `""` has length 0: its first byte is already the terminator.

To visit every character, load a byte, stop if it is zero, and otherwise move on to the next one.
Like Python's `for ch in s:`, this gives you each character in turn.
This function adds up the codes of all the characters:

```asm
# Returns the sum of the codes of the characters in s.
# s arrives in a0.
code_sum:
        li      t0, 0                   # t0 = total = 0
.Lcode_sum_loop:
        lbu     t1, 0(a0)               # t1 = the current character
        beqz    t1, .Lcode_sum_done     # the zero terminator: stop
        add     t0, t0, t1              # total = total + its code
        addi    a0, a0, 1               # move on to the next character
        j       .Lcode_sum_loop         # go back to the test
.Lcode_sum_done:
        mv      a0, t0                  # return total
        ret
```

```python
def code_sum(s):
    total = 0
    for ch in s:
        total = total + ord(ch)
    return total
```

The test is at the top, so the empty string is handled correctly: the loop body never runs.

## Is it a digit?

To check whether a character is in a range, such as `'0'` to `'9'`, use the one-compare range check from the unsigned-integers concept.
Subtract the first character of the range, then compare, as unsigned, with the number of characters in the range:

```asm
        addi    t0, t1, -'0'    # t0 = 0 to 9 for a digit, and something else otherwise
        sltiu   t0, t0, 10      # t0 = 1 if t1 is a digit, else 0
```

```python
is_digit = '0' <= ch <= '9'
```

There are 26 letters, so the same idea works for `'A'` to `'Z'`, and for `'a'` to `'z'`.

## Changing case

Each lowercase letter's code is 32 more than its uppercase letter's code.
`'a'` is 97 and `'A'` is 65, and `'z'` is 122 and `'Z'` is 90.
You can let the assembler work out the 32 as `'a' - 'A'`:

```asm
        addi    t0, t0, 'A' - 'a'       # 'q' (113) becomes 'Q' (81)
```

Only do this to a character that you have checked is a letter.
Taking 32 from a space would give 0, which would end the string.

## Writing a string

To write a string, store its characters one at a time with `sb`, then store the zero terminator after them.
The **caller**, the code that calls your function, gives you the address of a **buffer**: a piece of memory that is big enough to hold the result.
This function writes `count` dashes into the buffer at `dest`:

```asm
# Writes count '-' characters into dest, followed by a zero.
# dest arrives in a0, count in a1.
dashes:
        li      t0, '-'                 # t0 = the character to write
.Ldashes_loop:
        beqz    a1, .Ldashes_done       # no more to write
        sb      t0, 0(a0)               # write one dash
        addi    a0, a0, 1               # move to the next byte
        addi    a1, a1, -1              # one fewer to go
        j       .Ldashes_loop
.Ldashes_done:
        sb      zero, 0(a0)             # end the string
        ret
```

If you forget the terminator, the string does not end where you meant it to.
Whoever reads it carries on into whatever bytes happen to come next.
