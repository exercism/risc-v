# About

## One set of bits, two meanings

A 32-bit register holds a bit pattern.
Whether that pattern means a signed or an unsigned number depends only on the instruction that uses it.

| Type       | Smallest      | Largest              |
| ---------- | ------------- | -------------------- |
| `uint32_t` | 0             | 4294967295 (`0xFFFFFFFF`) |
| `int32_t`  | -2147483648 (`0x80000000`) | 2147483647 (`0x7FFFFFFF`) |

In two's complement, the top bit has the value -2³¹ instead of +2³¹.
That is why patterns from `0x80000000` upwards are negative when read as signed: the signed value is the unsigned value minus 2³².
To negate a two's complement number, flip every bit and add 1.

In Python, integers never overflow, so you have to wrap them yourself:

```python
def to_unsigned(x):
    return x % 2**32

def to_signed(x):
    x = x % 2**32
    return x - 2**32 if x >= 2**31 else x
```

## Instructions that do not care

These instructions give the same bits for signed and unsigned operands, so there is only one version of each:

- `add`, `addi`, `sub`, `mul` (the low 32 bits of the product);
- `beq`, `bne`, `seqz`, `snez`;
- the bitwise instructions `and`, `or`, `xor`.

Wrap-around in `add` and `sub` is **modular arithmetic**: every result is the true answer modulo 2³².
This means `b - a` is the correct distance from `a` to `b` whenever that distance is less than 2³², even if a counter wrapped past 0 in between.

RISC-V never traps on overflow.
Detecting it, when you need to, is your job.
For unsigned addition, the sum wrapped exactly when it is smaller than either operand:

```asm
        add     t0, a0, a1              # t0 = a0 + a1, wrapped
        sltu    t1, t0, a0              # t1 = 1 if the addition wrapped (the carry)
```

This trick is how 64-bit addition is built from 32-bit registers.

## Instructions that do care

| Signed           | Unsigned          |
| ---------------- | ----------------- |
| `blt`, `bge`     | `bltu`, `bgeu`    |
| `bgt`, `ble` (pseudo) | `bgtu`, `bleu` (pseudo) |
| `bltz`, `bgez`, `blez`, `bgtz` (pseudo) | none needed: an unsigned number is never below 0 |
| `slt`, `slti`    | `sltu`, `sltiu`   |
| `sgt` (pseudo)   | `sgtu` (pseudo)   |
| `div`, `rem`     | `divu`, `remu`    |
| `mulh`           | `mulhu` (and `mulhsu`, signed times unsigned) |

`bgtu a, b, label` is assembled as `bltu b, a, label`, and `bleu a, b, label` as `bgeu b, a, label`.

The immediate in `sltiu` is sign-extended to 32 bits first, and then compared as unsigned.
So `sltiu t0, a0, -1` compares with `0xFFFFFFFF`, and is 1 for every value except `0xFFFFFFFF`.

## The range check idiom

`low <= x <= high` needs two signed comparisons, but only one unsigned comparison:

```asm
        addi    t0, a0, -low            # t0 = x - low
        sltiu   t0, t0, high - low + 1  # t0 = 1 if low <= x <= high
```

If `x < low`, the subtraction wraps to a huge unsigned number, which fails the test just as `x > high` does.
Compilers use this all the time, for example to check that a character is a digit, or that an index is inside an array.
A related check, `0 <= i < n` for a signed index `i`, is just `bltu i, n, label`: a negative `i` looks like a huge unsigned number.

When `high - low + 1` is bigger than 2047, load it into a register with `li` and use `sltu` or `bltu`.

## Division by zero and overflow

| Operation                    | Result        |
| ---------------------------- | ------------- |
| `divu x, 0`                  | `0xFFFFFFFF`  |
| `remu x, 0`                  | `x`           |
| `div x, 0`                   | -1            |
| `rem x, 0`                   | `x`           |
| `div -2147483648, -1`        | -2147483648   |
| `rem -2147483648, -1`        | 0             |

None of these stops the program.
Unsigned division can never overflow.

## C and unsigned types

In C, `unsigned int` and `uint32_t` arithmetic is defined to wrap modulo 2³².
Signed overflow, on the other hand, is _undefined behaviour_ in C, even though the processor itself just wraps.
Mixing signed and unsigned values in a C comparison converts the signed one to unsigned, so `-1 < 1u` is false in C.
