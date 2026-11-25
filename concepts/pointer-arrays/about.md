# About

## Arrays of pointers

An array of pointers is an array of 32-bit words, where each word is an address.
On RV32 every pointer is 4 bytes, whatever it points to, so element `i` is always at offset `4 * i`.
Load an element with `lw` and store one with `sw`.

The things the pointers point to live elsewhere in memory.
For an array of strings, this means each string can have its own length, and two elements may even point to the same string.

| C type                  | Meaning                                                            |
| ----------------------- | ------------------------------------------------------------------ |
| `const char *`          | a string the function only reads                                   |
| `const char **`         | an array of such strings; the function may change the pointers     |
| `const char *const *`   | an array of such strings; the function changes nothing             |
| `char **`               | an array of strings whose characters the function may change       |

In a function's parameter list, `const char *names[]` means exactly the same as `const char **names`.
The array is not copied: only its address is passed.

## Two levels of indirection

`names[i][c]` takes two loads:

```asm
        slli    t0, a1, 2       # t0 = 4 * i
        add     t0, a0, t0      # t0 = the address of names[i]
        lw      t0, 0(t0)       # t0 = names[i], the address of a string
        add     t0, t0, a2      # t0 = the address of names[i][c]
        lbu     t0, 0(t0)       # t0 = names[i][c]
```

The first load must be `lw`.
Loading a pointer with `lbu` gives only its lowest byte, and following that "address" usually crashes the program.

The number of `*` in a type is the number of loads needed to reach the base value.
A `char ***`, for example a list of grids, would need three.

## Ending the list

| Style            | How the end is found                   | C example                          |
| ---------------- | -------------------------------------- | ---------------------------------- |
| pointer + count  | a separate `count` argument            | `f(names, 3)`                      |
| NULL-terminated  | the element after the last one is 0    | `char *argv[]`, `char **environ`   |

A **null pointer** has the value 0.
On this track's systems, address 0 never holds valid data, so a null pointer can safely mark "no string".
Test it with `beqz` straight after the `lw`, before you follow it.
Following a null pointer, for example with `lbu t0, 0(t1)` when `t1` is 0, crashes the program.

The `argv` argument of a C program's `main` uses both styles at once: `argc` gives the count, and `argv[argc]` is also `NULL`.

## Strings in a grid

There are two common ways to store a grid of characters in C:

- as an array of row pointers, `const char *rows[]`, where each row is a separate string;
- as one block of memory, `char grid[ROWS][COLS]`, where row `r` starts at `grid + r * COLS`.

With row pointers, `rows[r][c]` is a `lw` followed by a `lbu`.
In one block, it is a single `lbu` at `grid + r * COLS + c`, with no pointer to follow.
Row pointers let the rows have different lengths, and let you swap two rows by swapping two pointers.

## Comparing strings

Comparing two pointers with `beq` or `bne` compares the addresses.
That answers "is this the same string?", like Python's `is`.

Comparing text, like Python's `==`, needs a loop over both strings.
C's `strcmp(a, b)` does this, and returns 0 when the strings are equal.
It also orders strings: it stops at the first position where the bytes differ, and returns a negative number if `a`'s byte is smaller, or a positive number if it is bigger.
Because one string's zero terminator is smaller than any character, a string comes before any longer string that starts with it, so `"Ben"` comes before `"Benson"`.

The bytes are compared as unsigned numbers, which is another reason to use `lbu`.

## Pointers as results

A function can fill an output array with pointers.
Each `sw` copies an address, so the output shares its strings with the input: no characters are copied.
This is how sorting and filtering are often done in C: the pointers are moved around, and the strings stay where they are.

```python
short = [name for name in names if len(name) <= 4]
# the strings in short are the same objects as in names
```
