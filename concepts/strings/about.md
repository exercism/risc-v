# About

## Bytes and characters

A C string is a sequence of bytes ending with a zero byte.
Each byte is one `char`, and on RISC-V `char` is an **unsigned** 8-bit type, holding 0 to 255.
The C declarations on this track use `char *` for a string the function may change, and `const char *` for one it only reads.

The codes from 0 to 127 are **ASCII**:

| Range    | Characters                                        |
| -------- | ------------------------------------------------- |
| 0        | the zero terminator                               |
| 1–31     | control characters, such as tab (9) and newline (10) |
| 32       | space                                             |
| 33–47    | punctuation, such as `!` (33), `,` (44), `.` (46) and `/` (47) |
| 48–57    | `0` to `9`                                        |
| 65–90    | `A` to `Z`                                        |
| 97–122   | `a` to `z`                                        |
| 127      | delete, a control character                       |

The other gaps (58–64, 91–96 and 123–126) hold more punctuation, such as `:`, `?`, `@`, `[`, `_` and `~`.

Codes from 128 to 255 are not ASCII.
Modern text uses **UTF-8**, which writes every ASCII character as one byte, and every other character (such as `é` or the Samoan `ʻ`) as two to four bytes from 128 to 255.
A function that counts bytes counts those characters more than once, which is what C's `strlen` does too.

## Byte loads and stores

| Instruction           | Effect                                                          |
| --------------------- | --------------------------------------------------------------- |
| `lbu rd, off(rs1)`    | `rd` = the byte at `rs1 + off`, zero-extended (0 to 255)         |
| `lb  rd, off(rs1)`    | `rd` = the byte at `rs1 + off`, sign-extended (-128 to 127)      |
| `sb  rs2, off(rs1)`   | the byte at `rs1 + off` = the lowest 8 bits of `rs2`            |

There is no `sbu`, because storing a byte throws away the upper 24 bits, so signed and unsigned give the same result.

`lb` copies bit 7 of the byte into bits 8 to 31 of the register.
For an ASCII character, bit 7 is 0, so `lb` and `lbu` agree.
For a byte from 128 to 255 they differ: `lb` turns `0xE9` (233) into `0xFFFFFFE9` (-23).
Comparing that with a `char` argument, which arrives zero-extended as 233, then finds no match.
Use `lbu` for characters, and keep `lb` for arrays of `int8_t`.

## Character literals

The assembler turns `'c'` into the code of `c`, and allows arithmetic such as `'a' - 'A'` or `-'0'`.
The escapes `'\n'` (10), `'\t'` (9), `'\\'` (92) and `'\''` (39) also work.

The clang assembler, which this track uses, reads `'\0'` as `'0'` (48), not 0.
GNU `as` reads it as 0.
To be safe, always write the zero byte as `zero`, `0`, or a branch such as `beqz`.

## Strings and arrays

A string and an array of `int32_t` differ in two ways:

| Array of `int32_t`                    | String                                  |
| ------------------------------------- | --------------------------------------- |
| elements are 4 bytes, `lw` / `sw`     | characters are 1 byte, `lbu` / `sb`     |
| the length is passed as an argument   | the end is marked by a zero byte        |

Because the length is not stored, finding it means reading every character.
C's `strlen` does exactly this.
A loop that walks a string should test for the zero byte at the top, so that the empty string `""` is handled.

A common way to find the length is to walk a pointer to the terminator and subtract the start address:

```asm
        mv      t0, a0          # keep the start address in a0
        # ... walk t0 until it points at the zero byte ...
        sub     a0, t0, a0      # length = end - start
```

## Writing strings

A function that produces a string usually writes into a **buffer** the caller provides, as `strcpy` does in C.
The function must:

- write each character with `sb`;
- write the zero terminator at the end, including when the result is empty;
- never write past the end of the buffer.

The last rule is the caller's promise: the buffer is big enough.
C has no check for this, and writing too far damages whatever is stored next in memory.
This is called a **buffer overflow**, and it is a common cause of bugs and security holes.

A copy loop can store each byte before testing it.
Then the terminator is copied along with the characters, and no extra `sb zero` is needed.

## Changing case

Uppercase and lowercase letters differ only in bit 5 (value 32):

| Letter | Code | Bits        |
| ------ | ---- | ----------- |
| `A`    | 65   | `0100 0001` |
| `a`    | 97   | `0110 0001` |

So, once you know a character is a letter:

- `addi c, c, 'a' - 'A'` or `ori c, c, 32` makes it lowercase;
- `addi c, c, 'A' - 'a'` or `andi c, c, ~32` makes it uppercase;
- `xori c, c, 32` swaps the case.

Check that it is a letter first.
These change other characters too: `ori` would turn `@` (64) into a backtick (96).

## Python comparison

| Python                   | Assembly                                          |
| ------------------------ | ------------------------------------------------- |
| `for ch in s:`           | `lbu` a byte, stop on zero, `addi p, p, 1`        |
| `len(s)`                 | walk to the zero byte, then subtract the start    |
| `ord(ch)`                | nothing: the byte already is the code             |
| `chr(n)`                 | nothing: store the number with `sb`               |
| `'a' <= ch <= 'z'`       | `addi t, ch, -'a'` then `sltiu t, t, 26`          |
| `ch.lower()`             | a range check, then add `'a' - 'A'`               |

Python strings cannot be changed in place.
C strings can, if the function receives a `char *` rather than a `const char *`.

## Later concepts

- The `data-section` concept shows how to store fixed strings in your program with `.string`.
- The `number-formatting` concept writes numbers into a buffer as decimal digits.
- The `vector-strings` concept processes many characters at once.
