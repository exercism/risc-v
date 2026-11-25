# Introduction

A **set** is a collection of things in which each thing appears at most once, and the order does not matter.
Python has a `set` type for this.
In assembly, when the things are small numbers from 0 to 31, a single 32-bit register can hold the whole set.

## A number as a set

Bit `i` of the register says whether the number `i` is in the set:

- bit `i` is set (1) means `i` is in the set;
- bit `i` is clear (0) means `i` is not in the set.

Each number in the set is called a **member** of the set.
For example, the set {0, 3, 4} is stored as `0b11001`, which is `0x19`:

| Bit   | 4 | 3 | 2 | 1 | 0 |
| ----- | - | - | - | - | - |
| Value | 1 | 1 | 0 | 0 | 1 |

The number 0 is the **empty set**, with no members, and `0xFFFFFFFF` is the set of all 32 numbers from 0 to 31.
A set stored like this is called a **bit set**.
In C, a bit set is usually a `uint32_t`, because bit 31 is a member like any other, not a sign bit.

Python can do the same thing with an `int`:

```python
s = {0, 3, 4}                           # a Python set
bits = (1 << 0) | (1 << 3) | (1 << 4)   # the same set as a bit set: 0b11001
```

## A mask with one bit

Most set operations start with a **single-bit mask**: a number with only bit `i` set.
That number is `1 << i`.
When `i` is in a register, load 1 and shift it with `sll`:

```asm
        li      t0, 1           # t0 = 1, which is bit 0
        sll     t0, t0, a1      # t0 = 1 << a1, a mask with only bit a1 set
```

When `a1` is 31, the mask is `0x80000000`, the top bit.

## Changing one member

You met these uses of a mask in the bitwise-operations concept.
With a single-bit mask in `t0`, each one changes one member of the set in `a0`:

```asm
        or      a0, a0, t0      # add the member: set its bit
        xor     a0, a0, t0      # toggle the member: add it if it is missing, remove it if it is there
```

To **remove** a member, clear its bit.
`and` keeps the bits under the 1s of a mask, so flip the mask with `not` first, and then the 1s are everywhere except the member's bit:

```asm
        not     t1, t0          # t1 = every bit except the member's
        and     a0, a0, t1      # remove the member: clear its bit
```

```python
bits = bits | (1 << i)      # s.add(i)
bits = bits ^ (1 << i)      # add i if missing, remove it if present
bits = bits & ~(1 << i)     # s.discard(i)
```

## Is it a member?

To test whether `i` is a member, you want a boolean: 1 if it is, 0 if it is not.
There are two ways.

Shift the set right by `i`, so that bit `i` moves down to bit 0, and then keep only bit 0:

```asm
        srl     t1, a0, a1      # bit a1 of the set moves down to bit 0
        andi    t1, t1, 1       # t1 = 1 if a1 is a member, else 0
```

Or keep only bit `i` with `and` and a single-bit mask, and then turn the result into a boolean with `snez`:

```asm
        and     t1, a0, t0      # t1 = the mask if a1 is a member, else 0
        snez    t1, t1          # t1 = 1 if a1 is a member, else 0
```

Use `snez`, not a signed test such as `sgtz`.
When `i` is 31, `and` leaves `0x80000000`, and signed instructions read that as a negative number.

```python
is_member = (bits >> i) & 1     # like i in s, but 1 or 0
```

## Combining two sets

A bitwise instruction works on all 32 bits at once, so it works on all 32 members at once.
With two sets in `a0` and `a1`:

```asm
        or      t0, a0, a1      # union: members of either set, or both
        and     t1, a0, a1      # intersection: members of both sets
```

```python
s | t       # union of Python sets
s & t       # intersection of Python sets
s - t       # difference: members of s that are not in t
```

The **difference** `s - t` keeps the members of `s` and removes every member of `t`.
That is the same as removing one member, with a whole set as the mask instead of a single bit.

## Counting the members

Python's `len(s)` gives the number of members of a set.
For a bit set, that is the number of set bits.

There is a neat trick for this.
Subtracting 1 from a number changes its lowest set bit to 0, and changes all the 0s below it to 1s.
The bits above it stay the same.
So `x & (x - 1)` is `x` with its lowest set bit cleared:

| `x`      | `x - 1`  | `x & (x - 1)` |
| -------- | -------- | ------------- |
| `101100` | `101011` | `101000`      |
| `101000` | `100111` | `100000`      |
| `100000` | `011111` | `000000`      |

Each time you do this, one member is removed.
Count how many times you can do it before the set becomes 0, and you have counted the members.
A loop that tests at the top handles the empty set: its count is 0.

A later concept introduces `cpop`, an instruction from the Zbb extension that counts the set bits in one step.

## The smallest member

The smallest member of a set is the position of its lowest set bit.
You can find it by checking bit 0, then bit 1, and so on, until you find a set bit.

The empty set has no smallest member.
Check for it before you start looking, or a loop that looks for a set bit never finds one.

## Building a set from text

Bit sets are a quick way to answer questions about the letters in a word.
Does it use every letter of the alphabet?
Does any letter appear twice?

Give each lowercase letter a number: `'a'` is 0, `'b'` is 1, and so on up to `'z'`, which is 25.
That number is the character's code minus `'a'`.
For any other character, the same subtraction gives a number outside 0 to 25, so the one-compare range check from the strings concept tells you whether a character is a lowercase letter.

This Python function uses a bit set to find out whether a word repeats a lowercase letter:

```python
def repeats_a_letter(word):
    seen = 0
    for ch in word:
        if 'a' <= ch <= 'z':
            bit = 1 << (ord(ch) - ord('a'))
            if seen & bit:
                return True     # this letter is already in the set
            seen = seen | bit
    return False
```

To check that a word uses every letter, build its set of letters and compare it with `0x3FFFFFF`, the set with bits 0 to 25 all set.
