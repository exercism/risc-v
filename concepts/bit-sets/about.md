# About

A **bit set** stores a set of small whole numbers in the bits of a single integer.
Bit `i` is 1 exactly when `i` is a **member** of the set.
On RV32, one register holds a set of numbers from 0 to 31.
A larger set can be stored as an array of words, where member `i` is bit `i % 32` of word `i / 32`.

Bit sets are small and fast.
A set operation that would loop over a list or a hash table in Python takes a single instruction.
They are used for permissions and option flags, for the free blocks of a disk, for the letters of a word, and for the registers a compiler has in use.

## Types

Use `uint32_t` for a bit set.
Bit 31 is an ordinary member, but signed instructions read it as a minus sign:
`sra` copies it into the bits below, and `blt`, `bltz` and `sgtz` treat a set that contains 31 as negative.
Use `srl`, `snez`, `seqz`, `beqz` and `bnez`, which do not care about the sign.

## Operations on one member

With the member `i` in `a1`, first make the single-bit mask `1 << i`:

```asm
li      t0, 1
sll     t0, t0, a1      # t0 = 1 << i
```

`sll` uses only the lowest 5 bits of `a1`, so a member number of 32 or more silently becomes a different member.
Check the range first if the number might be too large.

| Operation | Instructions                              | Python                     |
| --------- | ----------------------------------------- | -------------------------- |
| add       | `or a0, a0, t0`                           | `s.add(i)`                 |
| remove    | `not t0, t0` then `and a0, a0, t0`        | `s.discard(i)`             |
| toggle    | `xor a0, a0, t0`                          | add if absent, else remove |
| test      | `srl t1, a0, a1` then `andi t1, t1, 1`    | `i in s`                   |
| test      | `and t1, a0, t0` then `snez t1, t1`       | `i in s`                   |

When the member is a constant from 0 to 10, the mask fits in an immediate, so `ori`, `xori` and `andi` work directly.
To remove a constant member from 0 to 10, use `andi` with `~(1 << i)`, which is a negative immediate that fits in 12 bits.

## Operations on two sets

| Operation              | Instructions                       | Python    |
| ---------------------- | ---------------------------------- | --------- |
| union                  | `or a0, a0, a1`                    | `s \| t`  |
| intersection           | `and a0, a0, a1`                   | `s & t`   |
| difference             | `not t0, a1` then `and a0, a0, t0` | `s - t`   |
| symmetric difference   | `xor a0, a0, a1`                   | `s ^ t`   |
| complement             | `not a0, a0`                       | all 32 members not in `s` |
| is `s` empty?          | `seqz a0, a0`                      | `not s`   |
| is `s` a subset of `t`?| difference, then `seqz`            | `s <= t`  |
| are they disjoint?     | intersection, then `seqz`          | `s.isdisjoint(t)` |
| are they equal?        | `xor`, then `seqz`                 | `s == t`  |

The Zbb extension adds `andn a0, a0, a1`, which does the difference in one instruction.
A later concept covers Zbb.

## Lowest-bit tricks

Two's complement arithmetic gives some useful identities for the lowest set bit of `x`:

| Expression     | Result                                     | Example: `x = 0b101100` |
| -------------- | ------------------------------------------ | ----------------------- |
| `x & (x - 1)`  | `x` with its lowest set bit cleared        | `0b101000`              |
| `x & -x`       | only the lowest set bit of `x`             | `0b000100`              |
| `x \| (x - 1)` | `x` with all the bits below the lowest set bit set | `0b101111`      |
| `x ^ (x - 1)`  | the lowest set bit and all the bits below it | `0b000111`            |

`x & (x - 1)` is 0 exactly when `x` has at most one member, which is a quick test for a power of two.

## Counting members

The number of members is called the **population count**, or **popcount**.
Clearing the lowest set bit until nothing is left takes one iteration per member:

```asm
        li      t0, 0                   # count
.Lloop:
        beqz    a0, .Ldone
        addi    t1, a0, -1
        and     a0, a0, t1              # remove the smallest member
        addi    t0, t0, 1
        j       .Lloop
.Ldone:
        mv      a0, t0
```

Checking all 32 bits one at a time would always take 32 iterations.
Zbb's `cpop` instruction counts the bits in one step.

## Smallest and largest member

The smallest member is the number of **trailing zeros**: the zero bits below the lowest set bit.
The largest member is 31 minus the number of **leading zeros**: the zero bits above the highest set bit.
Without Zbb, find them with a loop that shifts the set one place at a time and tests the end bit.
Zbb's `ctz` and `clz` find them in one step, and give 32 for the empty set.

## Sets of letters

Mapping `'a'` to `'z'` onto bits 0 to 25 gives a set of letters in one register.
Classic uses are:

- a **pangram** uses every letter, so its set equals `(1 << 26) - 1`, which is `0x3FFFFFF`;
- an **isogram** uses no letter twice, so no letter is already in the set when you add it;
- two words are **anagrams** only if their sets are equal, although equal sets do not prove it, because a set does not count repeated letters.

Fold uppercase letters into lowercase first if case should not matter.
