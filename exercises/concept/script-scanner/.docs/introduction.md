# Introduction

In the strings concept, your loops looked at one character at a time.
Vector instructions can look at many characters at once.
This concept shows how, and why strings need one new kind of load.

In Python, these jobs are all built in:

```python
len(line)           # the length
line.count('/')     # how many times '/' appears
line.find('*')      # the index of the first '*', or -1
line.upper()        # a copy with every lowercase letter made uppercase
line.isascii()      # True if every character has a code below 128
```

## Byte elements: `e8`

A character is one byte, so a string is a row of 8-bit elements.
Ask for them with `e8` in `vsetvli`, instead of `e32`:

```asm
        vsetvli t0, a1, e8, m1, ta, ma      # elements are bytes now
        vle8.v  v8, (a0)                    # v8 = the vl bytes starting at a0
        vse8.v  v8, (a1)                    # the vl bytes starting at a1 = v8
```

`vle8.v` and `vse8.v` load and store bytes, just as `vle32.v` and `vse32.v` load and store words.

A vector register holds four times as many bytes as words.
With VLEN = 128 it holds 16 bytes, and with VLEN = 512 it holds 64.
Each element is one byte, so a pointer moves on by `vl` bytes each round, with no shift.

Every vector instruction now works on bytes.
A `.vx` instruction uses only the lowest 8 bits of the ordinary register, so a character in `a1` works as you would expect.
The compares treat each byte as a number:

- `vmsltu`, `vmsleu` and `vmsgtu` see the bytes from 128 to 255 as large numbers, just like `lbu`.
- `vmslt`, `vmsle` and `vmsgt` see them as negative numbers from -128 to -1, just like `lb`.

So, as with `lbu`, use the unsigned compares for characters.

## Asking for as many bytes as possible

A string does not come with a length, so you cannot give `vsetvli` the number of elements left.
Instead, give it `zero`:

```asm
        vsetvli t0, zero, e8, m1, ta, ma    # t0 = vl = as many bytes as fit in a register
```

With `zero` as the count, `vsetvli` gives the largest `vl` it can, 16 bytes with VLEN = 128.
The destination must be a real register such as `t0`, not `zero`.

## The danger: reading past the end

Suppose a string is 3 bytes long, and you load 16 bytes from its start.
You get its 3 characters, its zero byte, and 12 bytes of whatever comes next in memory.

Usually those bytes belong to something else, and you can just ignore them.
But sometimes there is nothing there at all.
The operating system gives a program its memory in blocks called **pages**, usually 4096 bytes each.
If a string ends at the very end of a page, and the next page was never given to the program, then reading even one byte of the next page stops the program with a **segmentation fault**.

```text
 ...  | 'H' | 'i' |  0  ||  ?    ?    ?    ?  ...
      the end of a page  ||  a page the program cannot read
```

Byte by byte, the loops of the strings concept never had this problem: they stopped at the zero byte.
`vle8.v` has the problem, because it reads all `vl` bytes before you can look at any of them.

## Fault-only-first loads: `vle8ff.v`

`vle8ff.v` solves this.
The `ff` means **fault-only-first**:

- If the **first** byte cannot be read, the program stops, as with any load.
- If a **later** byte cannot be read, the load stops just before it, and makes `vl` smaller.
  The bytes it did load are correct, and nothing goes wrong.

The first byte of a string can always be read: even the empty string has its zero byte.
So `vle8ff.v` is always safe for a string.

After `vle8ff.v`, `vl` may be smaller than the number that `vsetvli` gave you.
`vl` is a **CSR**, a **c**ontrol and **s**tatus **r**egister that the processor keeps for itself.
Read it into an ordinary register with `csrr`, "**c**ontrol and **s**tatus **r**egister **r**ead":

```asm
        vsetvli t0, zero, e8, m1, ta, ma    # ask for as many bytes as fit
        vle8ff.v v8, (a0)                   # load as many of them as can safely be read
        csrr    t0, vl                      # t0 = how many were really loaded
```

The processor may also make `vl` smaller for other reasons, even in the middle of readable memory.
So always read `vl` after `vle8ff.v`, and never assume how many bytes it loaded.

## Finding the zero byte

To find the end of the string in a strip, compare every byte with 0, and look for the first match:

```asm
        vmseq.vi v9, v8, 0                  # mask: which byte is the zero byte
        vfirst.m t2, v9                     # t2 = its index in this strip, or -1
```

If `t2` is -1, the string goes on past this strip: move on by `vl` bytes and load again.
If `t2` is 0 or more, the zero byte is at index `t2` of this strip, and the string ends there.

There is no count, so there is no test at the top of the loop.
Instead, the loop ends when a strip holds the zero byte.

## Only the bytes before the zero byte

The strip with the zero byte also holds the bytes after it.
They are not part of the string, so they must not be counted, searched or changed.

The simplest way is to make `vl` smaller, so that later instructions do not see those bytes.
`vsetvli` with a count smaller than the most that fits gives exactly that count:

```asm
        vsetvli t0, t2, e8, m1, ta, ma      # vl = t2: only the bytes before the zero byte
```

`vsetvli` does not change the vector registers, so the bytes in `v8` are still there.
To include the zero byte as well, use `t2 + 1` instead.
If `t2` is 0, `vl` becomes 0, and the instructions after it do nothing.

Here is a function that checks whether every character in a string has a code below 128, like Python's `str.isascii`:

```asm
# Returns 1 if every byte of s is below 128, else 0.
# s arrives in a0.
is_ascii:
        li      t3, 127                     # the largest ASCII code
.Lis_ascii_loop:
        vsetvli t0, zero, e8, m1, ta, ma    # ask for as many bytes as fit
        vle8ff.v v8, (a0)                   # load the bytes that can safely be read
        csrr    t0, vl                      # t0 = how many were loaded
        vmseq.vi v9, v8, 0                  # mask: the zero byte
        vfirst.m t2, v9                     # t2 = its index, or -1
        bltz    t2, .Lis_ascii_check        # no zero byte in this strip
        vsetvli t0, t2, e8, m1, ta, ma      # keep only the bytes before the zero byte
.Lis_ascii_check:
        vmsgtu.vx v10, v8, t3               # mask: bytes above 127
        vfirst.m t1, v10                    # t1 = the first one, or -1
        bgez    t1, .Lis_ascii_no           # found one
        bgez    t2, .Lis_ascii_yes          # this strip held the zero byte: done
        add     a0, a0, t0                  # move on by vl bytes
        j       .Lis_ascii_loop
.Lis_ascii_yes:
        li      a0, 1
        ret
.Lis_ascii_no:
        li      a0, 0
        ret
```

Without the second `vsetvli`, a byte above 127 that came after the zero byte would give the wrong answer.

## Character classes

A **character class** is a group of characters, such as "the digits" or "the lowercase letters".
A range of characters needs two questions: is the byte at least the first one, and at most the last one?
There is no "greater than or equal" compare with an ordinary register, but "greater than the character before the first" asks the same thing:

```asm
        li      t4, '0' - 1
        li      t5, '9'
        vmsgtu.vx v10, v8, t4               # bytes above '0' - 1, so at least '0'
        vmsleu.vx v11, v8, t5               # bytes at most '9'
        vmand.mm v10, v10, v11              # mask: the digits
```

The one-compare range check from the strings concept works too.
Subtract the first character, and compare as unsigned with the number of characters in the range:

```asm
        li      t4, '0'
        vsub.vx v12, v8, t4                 # 0 to 9 for a digit, something else otherwise
        vmsleu.vi v10, v12, 9               # mask: the digits
```

The subtraction wraps around within each byte, so a byte below `'0'` becomes a large number, and fails the test.
This only works with the unsigned compare.

## Changing bytes in place

To change only some bytes of a string, use a masked instruction, and store the strip back with `vse8.v`.
As in the vector-masks concept, the bytes that are not changed must keep their values, so use `mu`:

```asm
        vsetvli t0, zero, e8, m1, ta, mu    # mu: inactive bytes keep their values
        ...
        vadd.vx v8, v8, t6, v0.t            # change only the bytes chosen by v0
        vse8.v  v8, (a0)                    # store vl bytes
```

`vse8.v` stores exactly `vl` bytes.
In the strip that holds the zero byte, cut `vl` down first, so that nothing after the end of the string is written.
Those bytes belong to someone else, and must be left alone.
The `vsetvli` that cuts `vl` down sets the mask policy again, so give it `mu` too.
