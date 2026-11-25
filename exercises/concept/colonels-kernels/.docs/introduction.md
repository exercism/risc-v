# Introduction

An array keeps many values of the **same** type together.
A **struct** keeps a few values of **different** types together, as one record.

## Structs in C

Here is a C struct for one reading from a weather station:

```c
struct reading {
    int32_t time;
    int16_t rain;
    int16_t wind;
};
```

Each value inside a struct is called a **field**.
This struct has three fields: `time`, `rain` and `wind`.

In Python, the nearest thing is a dataclass, or a dictionary that always has the same keys:

```python
from dataclasses import dataclass

@dataclass
class Reading:
    time: int
    rain: int
    wind: int

r = Reading(time=600, rain=12, wind=35)
print(r.rain)  # 12
```

A Python object can gain new attributes at any time.
A C struct cannot: its fields, and their types, are fixed when the program is written.

## Fields in memory

In memory, a struct is a block of bytes.
The fields are stored **in the order they are written**, one after another.
Each field starts at a fixed distance from the start of the struct.
That distance, in bytes, is the field's **offset**.

| Field  | Type      | Size | Offset |
| ------ | --------- | ---- | ------ |
| `time` | `int32_t` | 4    | 0      |
| `rain` | `int16_t` | 2    | 4      |
| `wind` | `int16_t` | 2    | 6      |

The whole struct takes 8 bytes.
In C, `sizeof(struct reading)` is 8.

## A pointer to a struct

A function cannot receive a whole struct in its registers.
Instead, it usually receives a **pointer to the struct**: the address of its first byte.

```c
extern int32_t windy(const struct reading *r);
```

The address arrives in `a0`.
To read a field, load from `a0` plus that field's offset.
Choose the load that matches the field's type: `lw` for `int32_t`, `lh` for `int16_t`, and `lhu` for `uint16_t`.

```asm
        lw      t0, 0(a0)       # t0 = r->time, an int32_t at offset 0
        lh      t1, 6(a0)       # t1 = r->wind, an int16_t at offset 6
```

In C, `r->wind` means "the `wind` field of the struct that `r` points to".

The assembler knows nothing about structs or field names.
It only sees numbers, so a comment that names the field makes your code much easier to read.

## Changing a field

To change a field, load it, change it in a register, and store it back at the same offset.
Use the store that matches the field's size: `sw` for a 4-byte field, `sh` for a 2-byte field.

```asm
# Adds 1 to the rain field.
# r arrives in a0.
more_rain:
        lh      t0, 4(a0)       # t0 = r->rain, at offset 4
        addi    t0, t0, 1       # t0 = t0 + 1
        sh      t0, 4(a0)       # r->rain = t0
        ret
```

```python
def more_rain(r):
    r.rain = r.rain + 1
```

Storing with the wrong size damages the struct.
Here, `sw` would write 4 bytes, and change `wind` as well as `rain`.

## Alignment and padding

You saw earlier that a word's address must be a multiple of 4, and a halfword's address a multiple of 2.
This is called **alignment**.
Fields inside a struct must be aligned too.

So C does not always put a field straight after the one before.
If the next free offset is not a multiple of the field's alignment, C skips some bytes first.
The skipped bytes are called **padding**.
They hold no field, and your code should never read or write them.

```c
struct score {
    int16_t round;
    int32_t points;
};
```

| Field     | Type      | Size | Offset |
| --------- | --------- | ---- | ------ |
| `round`   | `int16_t` | 2    | 0      |
| (padding) |           | 2    | 2      |
| `points`  | `int32_t` | 4    | 4      |

`round` ends at offset 2.
2 is not a multiple of 4, so `points` cannot start there.
C adds 2 bytes of padding, and `points` starts at offset 4.

There is one more rule.
The size of the whole struct is rounded **up** to a multiple of its largest alignment.
That way, when several structs sit side by side in an array, every field of every struct is still aligned.

```c
struct sample {
    int32_t depth;
    int16_t temp;
    int16_t salt;
    int16_t light;
};
```

| Field     | Type      | Size | Offset |
| --------- | --------- | ---- | ------ |
| `depth`   | `int32_t` | 4    | 0      |
| `temp`    | `int16_t` | 2    | 4      |
| `salt`    | `int16_t` | 2    | 6      |
| `light`   | `int16_t` | 2    | 8      |
| (padding) |           | 2    | 10     |

The fields end at offset 10.
The largest field is an `int32_t`, with alignment 4, so the size is rounded up from 10 to 12.
`sizeof(struct sample)` is 12, not 10.

To work out a struct's layout by hand:

1. Start at offset 0.
2. For each field, in order, round the offset up to a multiple of the field's alignment, then give the field that offset.
3. Add the field's size to get the next free offset.
4. At the end, round the offset up to a multiple of the largest alignment. That is the struct's size.

For `int16_t` and `uint16_t` the alignment is 2, and for `int32_t`, `uint32_t` and pointers it is 4.

## Arrays of structs

An array of structs works just like any other array.
The element size is the size of the struct, **including** its padding.
So element `i` starts at `base + i * size`.

For an array of `struct sample`, the size is 12.
Element 0 is at `base`, element 1 at `base + 12`, element 2 at `base + 24`, and so on.

12 is not a power of 2, so you cannot multiply by it with a single shift.
Use `mul` instead:

```asm
        li      t0, 12          # t0 = sizeof(struct sample)
        mul     t0, a1, t0      # t0 = i * 12, the offset of samples[i]
        add     t0, a0, t0      # t0 = the address of samples[i]
        lh      t1, 4(t0)       # t1 = samples[i].temp
```

The field's offset goes in the load, as usual.
The address in `t0` points at the start of the element, and `4(t0)` reaches its `temp` field.

To walk a pointer through the array, work out the end pointer as `base + count * size`, and move the pointer on by the size after each element.
This function sets the `light` field of every sample to 0:

```asm
# Clears the light field of every sample.
# samples arrives in a0, count in a1.
clear_light:
        li      t0, 12          # t0 = sizeof(struct sample)
        mul     t0, a1, t0      # t0 = count * 12
        add     t0, a0, t0      # t0 = end
clear_light_loop:
        beq     a0, t0, clear_light_done  # if the pointer has reached the end, stop
        sh      zero, 8(a0)     # the light field of this sample = 0
        addi    a0, a0, 12      # move the pointer to the next sample
        j       clear_light_loop  # go back to the test
clear_light_done:
        ret
```

```python
def clear_light(samples):
    for s in samples:
        s.light = 0
```

Notice the two different numbers.
The **offset** (8) picks a field inside one struct.
The **size** (12) steps from one struct to the next.

If you need the index of the current element as well, keep a counter in another register, and add 1 to it each time you move the pointer.
