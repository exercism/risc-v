# About

On RV32 every register holds 32 bits, but C's `int64_t` and `uint64_t` hold 64.
The processor has no 64-bit instructions, so a 64-bit number lives in a **pair** of registers, and each 64-bit operation is built from a few 32-bit ones.
Compilers for RV32 do exactly this when they compile C code that uses `long long`, `int64_t` or `uint64_t`.

## Register pairs in the calling convention

The RISC-V calling convention for RV32 (the `ilp32` ABI) passes a 64-bit scalar argument in two consecutive argument registers.
The low 32 bits go in the lower-numbered register.

| C declaration                                   | Registers                                     |
| ----------------------------------------------- | --------------------------------------------- |
| `uint64_t f(uint64_t x)`                        | `x` low in `a0`, high in `a1`                 |
| `uint64_t f(uint64_t x, uint64_t y)`            | `x` in `a0`/`a1`, `y` in `a2`/`a3`            |
| `uint64_t f(uint64_t x, uint32_t n)`            | `x` in `a0`/`a1`, `n` in `a2`                 |
| `int64_t f(int32_t n, int64_t x)`               | `n` in `a0`, `x` in `a1`/`a2`                 |
| `int64_t f(int32_t m, int32_t n, int64_t x)`    | `m` in `a0`, `n` in `a1`, `x` in `a2`/`a3`    |

A 64-bit argument does **not** have to start at an even-numbered register for ordinary functions: `f(int32_t n, int64_t x)` puts `x` in `a1`/`a2`.
(Variadic functions such as `printf` are different, and do align the pair to an even register.)
If only one argument register is left, the low half goes in `a7` and the high half on the stack.

A 64-bit return value comes back in `a0` (low) and `a1` (high).

## Building 64-bit operations

Here `x` is in `a0`/`a1` and `y` is in `a2`/`a3`.

### Addition

```asm
        add     a0, a0, a2              # low half
        sltu    t0, a0, a2              # carry: the low sum wrapped if it is smaller than y's low half
        add     a1, a1, a3              # high half
        add     a1, a1, t0              # plus the carry
```

The check `sltu carry, sum, y_low` works because an unsigned sum that wraps around is always smaller than either of the numbers added.
You can compare with either addend, but it must be one you have not overwritten.

### Subtraction

```asm
        sltu    t0, a0, a2              # borrow: x_low < y_low
        sub     a0, a0, a2              # low half
        sub     a1, a1, a3              # high half
        sub     a1, a1, t0              # minus the borrow
```

### Negation

Negating is subtracting from zero:

```asm
        snez    t0, a0                  # borrow: 1 unless the low half is 0
        neg     a0, a0                  # low half
        neg     a1, a1                  # high half
        sub     a1, a1, t0              # minus the borrow
```

### Comparison

| Comparison          | High halves                      | Low halves, if the high halves are equal |
| ------------------- | -------------------------------- | ---------------------------------------- |
| `uint64_t` `x < y`  | `bltu` / `sltu`                  | `bltu` / `sltu`                          |
| `int64_t` `x < y`   | `blt` / `slt`                    | `bltu` / `sltu`                          |
| `x == y`            | `beq`                            | `beq`                                    |

The low half of an `int64_t` carries no sign, so it is always compared as unsigned.

### Multiplication

| Instruction | Result                                                    |
| ----------- | --------------------------------------------------------- |
| `mul`       | low 32 bits of the product                                |
| `mulhu`     | high 32 bits, both operands unsigned                      |
| `mulh`      | high 32 bits, both operands signed                        |
| `mulhsu`    | high 32 bits, first operand signed, second unsigned       |

`mul` with `mulhu` multiplies two `uint32_t` values to a `uint64_t`, and `mul` with `mulh` multiplies two `int32_t` values to an `int64_t`.
The RISC-V specification recommends writing the high-half instruction first, then `mul` with the same source registers, so that a processor can fuse the two into one multiplication.
That only works when the first instruction does not overwrite one of the sources.

Multiplying two full 64-bit numbers needs more steps.
For the low 64 bits of `x * y`:

- the low half is `mul` of the low halves;
- the high half is `mulhu` of the low halves, plus `mul` of `x_low` and `y_high`, plus `mul` of `x_high` and `y_low`.

The parts that would land above bit 63 are thrown away, so the result wraps at 2⁶⁴ just like `uint64_t` in C.

### Division

RV32 has no 64-bit division instruction.
Compilers call a library function such as `__udivdi3` instead.

## Overflow and wrap-around

`uint64_t` arithmetic wraps around at 2⁶⁴.
Python's integers never overflow, so to check an answer in Python, use `% 2**64`:

```python
(2**64 - 1 + 1) % 2**64     # 0
```

The carry out of the high half, found with the same `sltu` check, tells you whether a 64-bit unsigned addition overflowed.

## RV64

On RV64, the 64-bit RISC-V, every register holds 64 bits, so a `uint64_t` fits in one register and `add`, `sub` and `mul` work on all 64 bits at once.
RV64 also has `addw`, `subw` and `mulw`, which work on the low 32 bits.
