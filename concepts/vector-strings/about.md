# About

## Byte vectors

With `e8` in `vsetvli`, every element is one byte.
The most bytes one register can hold, VLMAX, is VLEN / 8: 16 with VLEN = 128, 64 with VLEN = 512.
With LMUL `m8`, a group of eight registers works as one, and VLMAX becomes VLEN, so a single instruction can handle 128 or 512 bytes.

| Instruction           | Meaning                                                  |
| --------------------- | -------------------------------------------------------- |
| `vle8.v vd, (rs1)`    | load `vl` bytes from `rs1`                               |
| `vse8.v vs3, (rs1)`   | store `vl` bytes to `rs1`                                |
| `vle8ff.v vd, (rs1)`  | load up to `vl` bytes, stopping early instead of faulting |
| `vle16.v`, `vse16.v`, `vle16ff.v` | the same for halfwords (`e16`)               |

There is no alignment rule for byte loads and stores: any address works.
A pointer advances by `vl` bytes, not `vl * 4`.

When SEW is 8, a `.vx` operand uses the low 8 bits of the scalar register, and a `.vi` immediate (-16 to 15) is truncated to 8 bits.
Arithmetic wraps within each byte: `'A' - 'a'` is -32, which wraps around to 224.

Bytes from 128 to 255 are negative for the signed compares (`vmslt`, `vmsle`, `vmsgt`), and large for the unsigned ones (`vmsltu`, `vmsleu`, `vmsgtu`).
Characters are unsigned, so use the unsigned compares, as you use `lbu` and not `lb`.

## `vsetvli` with `zero`

The AVL (application vector length) operand of `vsetvli` has two special cases:

| `rd`     | `rs1`    | Result                                              |
| -------- | -------- | --------------------------------------------------- |
| not `x0` | `x0`     | `vl` = VLMAX, and `rd` = VLMAX                      |
| `x0`     | `x0`     | `vl` is kept, only the type (SEW, LMUL, policies) changes |
| any      | not `x0` | AVL = `rs1`: `vl` = AVL if AVL is at most VLMAX, and never more than VLMAX |

So `vsetvli t0, zero, e8, m1, ta, ma` asks for as many bytes as fit, while `vsetvli zero, zero, e8, m1, ta, ma` does something different.

When AVL is at most VLMAX, `vl` is exactly AVL.
That is what makes `vsetvli t0, t2, e8, m1, ta, ma` a reliable way to cut `vl` down to the index of the zero byte.
`vsetvli` never changes the contents of the vector registers.

## Fault-only-first loads

`vle8ff.v` (and `vle16ff.v`, `vle32ff.v`, `vle64ff.v`) behave like ordinary unit-stride loads, with one difference:

- If element 0 raises an exception, such as a page fault, the trap is taken as usual and `vl` is unchanged.
- If an element *i* > 0 would raise an exception, no trap is taken, and `vl` is reduced to *i*.
- Even with no exception, the processor may load fewer elements and reduce `vl`, as long as it loads at least one.

The elements past the new `vl` are tail elements: their values may be anything, and they may even be overwritten with values that are not in memory.
So read `vl` back with `csrr rd, vl` after every fault-only-first load, and use only the first `vl` elements.

Memory is protected one **page** at a time (4096 bytes on most RISC-V Linux systems).
Within a page, either every byte can be read or none can.
So the only place a string read can fault is where it crosses into the next page, and a fault-only-first load just stops there.
The next round then starts at the new page, and its element 0 is a byte of the string, which can always be read.

The specification also warns that fault-only-first loads can be used to probe which addresses are mapped, which is a security concern for privileged code.
A user program only learns about the memory next to a string it was given.

`vl` is a read-only CSR (control and status register).
The other vector CSRs include `vtype`, the current SEW, LMUL and policies, and `vlenb`, VLEN in bytes:

```asm
        csrr    t0, vl          # the current vector length
        csrr    t1, vlenb       # VLEN / 8, the bytes in one vector register
```

## The strlen pattern

The specification's own example of `strlen` uses this loop:

```asm
strlen:
        mv      a3, a0                          # save the start
.Lloop:
        vsetvli a1, zero, e8, m8, ta, ma        # as many bytes as possible
        vle8ff.v v8, (a3)                       # load what can be read
        csrr    a1, vl                          # how many were read
        vmseq.vi v0, v8, 0                      # where are the zero bytes?
        vfirst.m a2, v0                         # the first one, or -1
        add     a3, a3, a1                      # move on
        bltz    a2, .Lloop                      # not found yet
        add     a0, a0, a1                      # start + this strip's vl
        add     a3, a3, a2                      # the address after this strip + the index
        sub     a0, a3, a0                      # the length
        ret
```

It moves the pointer on before the test, and corrects for that afterwards.
It uses `m8`, so each round handles up to eight registers' worth of bytes.

## Stopping at the terminator

In the strip that holds the zero byte, the bytes after it belong to other data.
There are two common ways to ignore them.

**Cut `vl` down.**
`vsetvli t0, t2, ...` with `t2` the index of the zero byte (or `t2 + 1` to include it) makes every later instruction see only the string.
This also makes `vse8.v` write only the string, which matters when changing a string in place or copying it.

**Build a mask that stops at the terminator.**
The mask instructions `vmsbf.m`, `vmsif.m` and `vmsof.m` work on the first set bit of a mask:

| Instruction        | Bit `i` of the result is 1 when                   |
| ------------------ | ------------------------------------------------- |
| `vmsbf.m vd, vs2`  | `i` is **before** the first set bit of `vs2`      |
| `vmsif.m vd, vs2`  | `i` is before or **including** the first set bit  |
| `vmsof.m vd, vs2`  | `i` is **only** the first set bit                 |

If `vs2` has no set bit, `vmsbf.m` and `vmsif.m` set every bit.
So `vmsbf.m` of the zero-byte mask selects exactly the characters of the string in this strip, and `vmand.mm` with it restricts any other mask to the string.
A masked store, `vse8.v v8, (a0), v0.t`, then writes only those bytes.

## Character classes

| Class        | Two compares                                    | One compare after subtracting |
| ------------ | ----------------------------------------------- | ----------------------------- |
| digit        | `vmsgtu.vx` with `'0' - 1`, `vmsleu.vx` with `'9'` | subtract `'0'`, `vmsleu.vi` 9 |
| uppercase    | `vmsgtu.vx` with `'A' - 1`, `vmsleu.vx` with `'Z'` | subtract `'A'`, `vmsltu.vx` 26 |
| lowercase    | `vmsgtu.vx` with `'a' - 1`, `vmsleu.vx` with `'z'` | subtract `'a'`, `vmsltu.vx` 26 |

`vmandn.mm`, `vmor.mm` and `vmnot.m` build larger classes, such as "letter" (`upper | lower`) or "not a space".
The ASCII trick `c | 0x20` (with `vor.vx`) turns every uppercase letter into lowercase and leaves lowercase letters alone, so one range check on the result finds every letter.

## Case conversion

Lowercase and uppercase letters differ by 32 (`'a' - 'A'`).
A masked `vsub.vx` or `vadd.vx` changes only the letters, and needs `mu` so the other bytes keep their values when the strip is stored.
Without a mask, `vmerge.vvm` can choose between the original bytes and the converted ones.

Bit 5 (value 32) is the difference between the cases, so `vxor.vx` with 32 swaps case, `vand.vx` with `~32` makes uppercase, and `vor.vx` with 32 makes lowercase, but only for bytes that are letters.

## Copying

`strcpy` is the same loop with a store:
load with `vle8ff.v`, find the zero byte, cut `vl` to its index plus 1, store with `vse8.v`, and stop.
Only the source needs a fault-only-first load.
The destination is a buffer of known size, and the store never goes past the zero byte.

When the length is known, as in `memcpy`, an ordinary strip-mining loop with `vle8.v` and a count works, and no fault-only-first load is needed.

## Comparing with Python

| Python                        | RVV                                                       |
| ----------------------------- | --------------------------------------------------------- |
| `len(s)`                      | `vle8ff.v`, `vmseq.vi` 0, `vfirst.m`                      |
| `s.count(c)`                  | `vmseq.vx`, `vcpop.m`, stopping at the zero byte          |
| `s.find(c)`                   | `vmseq.vx`, `vfirst.m`, stopping at the zero byte         |
| `sum(ch.isupper() for ch in s)` | two compares and `vmand.mm`, then `vcpop.m`             |
| `s.upper()`                   | a lowercase mask and a masked `vsub.vx` with `mu`         |
| `s.isascii()`                 | `vmsgtu.vx` with 127 and `vfirst.m`                       |

## UTF-8

Text outside ASCII, such as `é`, is usually stored as UTF-8: one character takes two to four bytes, each from 128 to 255.
Byte-by-byte functions count bytes, not characters, and vector string functions do the same.
A range check on bytes never matches part of a UTF-8 character as an ASCII letter, because every such byte is at least 128.
