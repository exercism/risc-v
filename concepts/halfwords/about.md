# About

## Sizes in memory

RISC-V names memory values by their size:

| Name     | Bits | Bytes | Load (signed) | Load (unsigned) | Store |
| -------- | ---- | ----- | ------------- | --------------- | ----- |
| byte     | 8    | 1     | `lb`          | `lbu`           | `sb`  |
| halfword | 16   | 2     | `lh`          | `lhu`           | `sh`  |
| word     | 32   | 4     | `lw`          | none on RV32    | `sw`  |

On RV32 a register is 32 bits, so `lw` fills it completely and needs no unsigned form.
On RV64 there is also `lwu`, and a doubleword (`ld`, `sd`).

The C types that match a halfword are `int16_t` (-32768 to 32767) and `uint16_t` (0 to 65535).
On RISC-V, `short` and `unsigned short` are also 16 bits.

## Sign extension and zero extension

`lh rd, offset(rs1)` loads 16 bits and copies bit 15 into bits 16 to 31 of `rd`.
`lhu rd, offset(rs1)` loads 16 bits and sets bits 16 to 31 of `rd` to zero.

| Bits in memory | `lh` gives             | `lhu` gives            |
| -------------- | ---------------------- | ---------------------- |
| `0x0064`       | `0x00000064` = 100     | `0x00000064` = 100     |
| `0x7FFF`       | `0x00007FFF` = 32767   | `0x00007FFF` = 32767   |
| `0x8000`       | `0xFFFF8000` = -32768  | `0x00008000` = 32768   |
| `0xFF38`       | `0xFFFFFF38` = -200    | `0x0000FF38` = 65336   |
| `0xFFFF`       | `0xFFFFFFFF` = -1      | `0x0000FFFF` = 65535   |

The two loads agree for values from 0 to 32767, so a bug from the wrong choice only shows up for negative `int16_t` values, or `uint16_t` values from 32768 up.
Tests should always include such values.

After either load, the register holds the correct 32-bit value of the halfword.
From then on, signed branches such as `blt` and `bge` compare correctly, because a value from `lhu` is between 0 and 65535 and so is never negative as a 32-bit number.

## Doing the extension yourself

If a register already holds a value whose low 16 bits matter, the extension can be done with shifts:

```asm
        slli    t0, t0, 16      # move bit 15 up to bit 31
        srai    t0, t0, 16      # sign-extend back down (like lh)

        slli    t1, t1, 16
        srli    t1, t1, 16      # zero-extend back down (like lhu)
```

Zero extension can also be done with a mask, but `0xFFFF` does not fit in a 12-bit immediate, so it must be loaded with `li` first.
The Zbb extension adds single instructions for these: `sext.h` and `zext.h`.

## Storing

`sh rs2, offset(rs1)` writes bits 0 to 15 of `rs2` and ignores the rest.
The bytes on either side are not changed.
This is why storing into an `int16_t` array with `sw` is a bug: it overwrites the next element too.

A value outside the 16-bit range is truncated, with no error and no flag.
In C, converting an out-of-range value to `int16_t` does the same on RISC-V: the value wraps around modulo 65536.
For example, 40000 stored as a halfword and loaded back with `lh` gives 40000 - 65536 = -25536.

## Byte order

RISC-V is **little-endian**: the low byte of a halfword is at the lower address.
The halfword `0x1234` at address `a` is stored as `0x34` at `a` and `0x12` at `a + 1`.
This matters only when the same memory is read with different sizes.

## Alignment

A halfword is **naturally aligned** when its address is a multiple of 2.
The base ISA allows a misaligned access to be very slow, or to trap so that software can finish it, or to fail.
Compilers always place `int16_t` values at even addresses, and arrays of them keep that alignment because the element size is 2.

## Arithmetic

RISC-V has no 16-bit arithmetic instructions.
Values are extended to 32 bits when loaded, and every `add`, `sub`, `mul` and branch works on all 32 bits.
This is the same as C, where an `int16_t` is promoted to `int` before arithmetic.

So the sum of many `int16_t` values, kept in a register, does not overflow until it passes the 32-bit range.
A result needs to fit in 16 bits only when it is stored back with `sh`.

## Comparing with Python

Python's integers have no fixed size, so there is no sign extension or truncation in ordinary Python code.
The `array` module and NumPy do have fixed sizes:

```python
from array import array
import numpy as np

array('h', [40000])                          # OverflowError: array refuses it
np.array([40000]).astype(np.int16)           # array([-25536]): truncated, like sh
np.frombuffer(b'\x38\xff', dtype=np.int16)   # array([-200]), like lh
np.frombuffer(b'\x38\xff', dtype=np.uint16)  # array([65336]), like lhu
```
