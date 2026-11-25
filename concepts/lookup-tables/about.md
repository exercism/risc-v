# About

A lookup table replaces a calculation, or a chain of branches, with a load from memory.
The table holds one answer for each possible input, and the input, after a little arithmetic, is the index.

## The pattern

Almost every table lookup has the same four steps:

1. turn the input into an index from 0, usually by subtracting the first valid input;
2. check the index with one unsigned comparison against the number of entries;
3. scale the index by the size of an entry: no shift for `.byte`, `slli 2` for `.word`;
4. add it to the table's address from `la`, and load with `lbu` or `lw`.

```asm
        addi    t0, a0, -FIRST          # step 1
        li      t1, COUNT
        bgeu    t0, t1, .Lnot_found     # step 2
        slli    t0, t0, 2               # step 3, for a table of words
        la      t1, table
        add     t1, t1, t0              # step 4
        lw      a0, 0(t1)
```

Step 2 can be left out only when the caller promises the input is always valid.
Without it, a bad input reads memory outside the table: a wrong answer at best, and a crash if the address is not mapped.

## Choosing the entry size

Use the smallest entry that holds every answer:

| Directive | Load  | Range of values                        |
| --------- | ----- | -------------------------------------- |
| `.byte`   | `lbu` | 0 to 255                               |
| `.byte`   | `lb`  | -128 to 127                            |
| `.half`   | `lhu` | 0 to 65535                             |
| `.half`   | `lh`  | -32768 to 32767                        |
| `.word`   | `lw`  | any 32-bit value, including addresses  |

A `.byte` table that needs a sentinel of -1 can store it as `-1` (the byte 255) and load it with `lb`, which turns 255 back into -1.
Put `.balign 2` or `.balign 4` before a table of halfwords or words.

## A biased base

`la` accepts an expression, so the assembler can move a table's address before the program runs:

```asm
        la      t0, byte_table - 'A'            # bytes: one entry per character
        la      t1, word_table - 4 * 'A'        # words: four bytes per character
```

The address in the register is not the address of any entry.
It is the address where entry 0 would be if the table started at character code 0.
Adding the input (scaled, for words) then lands on the right entry, without the `addi` of step 1.
This is only safe when the input is known to be valid, because the range check needs the index from 0.

## Tables indexed by any byte

A table with 256 entries can be indexed by any byte from 0 to 255 with no range check at all.
`.zero` fills the gaps between the entries you care about:

```asm
char_class:                             # 1 for a digit, 2 for a capital letter, else 0
        .zero   '0'                     # codes 0 to 47
        .byte   1, 1, 1, 1, 1, 1, 1, 1, 1, 1
        .zero   'A' - '9' - 1           # codes 58 to 64
        .byte   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
        .byte   2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
        .zero   256 - 'Z' - 1           # codes 91 to 255
```

Tables like this are how C's `isdigit` and `isalpha` are often written.
They cost 256 bytes of memory, which is a good trade when the lookup happens millions of times.

## Tables of addresses

`.word label` stores the address of `label`.
The linker fills in the final address when it builds the program, so the table works wherever the string ends up.
A table of string addresses lets a function return a different string for each input without a branch.

The pointer-arrays concept goes further with tables of `char *`, including ones that a caller passes in.

## Offsets into one long string

Instead of a table of 4-byte addresses, a program can keep all its strings in one block and store each string's offset from the start of the block in a `.byte` or `.half` table.
The offsets are smaller than addresses, and the assembler can work them out from labels:

```asm
names:
red:    .string "red"
blue:   .string "blue"
offsets:
        .byte   red - names, blue - names       # 0 and 4
```

The function loads the offset, then adds it to the address of `names`.

## Two-level tables

When many inputs share a few answers, store the answers once, and give each input a small number that picks one.
The first table maps each input to that number, and the second maps the number to the answer.
This is how fonts map characters to shapes, and how a compiler might map a token to its kind and then the kind to its name.

The same idea handles a bit mask as an index: a mask of 5 bits has only 32 possible values, so a 32-entry table can give any property of the mask, such as how many bits are set.

## Data-driven or code-driven

| Code-driven: branches                 | Data-driven: a table                          |
| ------------------------------------- | --------------------------------------------- |
| grows by several instructions per case | grows by one entry per case                   |
| each case can do different work       | every case does the same work                 |
| fine for two or three cases           | better for many cases, or cases with gaps     |
| takes time proportional to the case   | takes the same time for every input           |

In Python, the same choice is between `if`/`elif` and a list or `dict`.
A list is like a table indexed from 0, and `dict.get(key, default)` is like a table with a range check and a sentinel.

A table of addresses of code, rather than of data, is a **jump table**: the jump-tables concept uses one to choose which code to run.
