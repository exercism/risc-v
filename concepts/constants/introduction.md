# Introduction

Look at this line of assembly:

```asm
        addi    a0, a0, 7
```

What is the `7`?
A week of days?
The number of players in a team?
A number like this, with no name to explain it, is called a **magic number**.
Magic numbers make code hard to read, and hard to change.

## Naming a number with `.equ`

The `.equ` directive gives a number a name:

```asm
.equ    CREW_SIZE, 7            # CREW_SIZE now means 7
```

After this line, you can write `CREW_SIZE` anywhere you could write `7`.
This named number is called a **constant**.

In Python you would write the same idea as a variable in UPPER_CASE at the top of a file:

```python
CREW_SIZE = 7
```

In Python that is only a habit, and the program could still change `CREW_SIZE` later.
In assembly a constant really is fixed.

Constant names are written in UPPER_SNAKE_CASE: capital letters, with `_` between the words.
This makes them easy to tell apart from registers and labels.
Put your constants near the top of the file, before the code that uses them.

## Using a constant

A constant can go wherever an immediate can go.

```asm
.equ    CREW_SIZE, 7
.equ    SPARE_PADDLES, 2

        li      t0, CREW_SIZE           # t0 = 7
        addi    a0, a0, SPARE_PADDLES   # a0 = a0 + 2
        blt     a0, t0, .Lshort         # if a0 < 7, jump to .Lshort
```

A branch still needs two registers, so a constant used in a comparison must be loaded with `li` first, just like any other number.

## Working out a constant from other constants

The value of a constant can be an **expression**: a small calculation using numbers, other constants, `+`, `-`, `*`, `/` and brackets.

```asm
.equ    CANOES, 3
.equ    CREW_SIZE, 7
.equ    PADDLERS, CANOES * CREW_SIZE    # 21
.equ    LAST_SEAT, CREW_SIZE - 1        # 6
```

You can also write an expression straight into an instruction:

```asm
        addi    a0, a0, CREW_SIZE - 1   # a0 = a0 + 6
```

The **assembler** works out these expressions, while it turns your text into machine code.
The processor never sees `CANOES * CREW_SIZE`.
It only sees the answer, `21`, inside the instruction.
So the `*` here does not need a multiply instruction.

## Constants take no space

A constant is only a name that the assembler remembers while it reads your file.
It is not stored in memory and it does not have an address.
It is not a register, either.
Writing `li t0, CREW_SIZE` gives exactly the same machine code as writing `li t0, 7`.

## Codes for choices

Programs often use small numbers as **codes** that stand for a choice.
A set of related codes is called an **enumeration**.
In Python you might write:

```python
from enum import IntEnum

class Weather(IntEnum):
    SUNNY = 1
    CLOUDY = 2
    RAINY = 3
```

In assembly, give each code its own constant, with a shared start to the names:

```asm
.equ    WEATHER_SUNNY, 1
.equ    WEATHER_CLOUDY, 2
.equ    WEATHER_RAINY, 3
```

Then a test reads like the idea behind it:

```asm
        li      t0, WEATHER_RAINY       # t0 = 3
        beq     a0, t0, .Lumbrella      # if the weather is rainy, jump
```

## Agreeing with C

The tests for this track are written in C.
C has its own way to name numbers, with `#define` or `enum`:

```c
#define CREW_SIZE 7

enum weather { WEATHER_SUNNY = 1, WEATHER_CLOUDY = 2, WEATHER_RAINY = 3 };
```

The C file and the assembly file are turned into machine code separately, and they cannot see each other's names.
So the numbers must agree: if the tests say `WEATHER_RAINY` is 3, your `.equ WEATHER_RAINY` must also be 3.
