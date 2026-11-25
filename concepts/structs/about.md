# About

## What a struct is

A C `struct` groups a fixed set of named **fields**, possibly of different types, into one block of memory.
The assembler has no idea that structs exist.
To assembly code, a struct is just an address, and each field is a number of bytes from that address.

The nearest thing in Python is a dataclass, a `namedtuple`, or a dictionary that always has the same keys.
The difference is that a C struct's fields, their types and their order are fixed when the program is compiled.
That is what makes it possible to know every field's offset in advance.

## Sizes and alignments on RV32

The RISC-V calling convention for RV32 (the `ilp32` ABI) gives each C type a size and an alignment:

| C type                         | Size | Alignment |
| ------------------------------ | ---- | --------- |
| `int8_t`, `uint8_t`, `char`    | 1    | 1         |
| `int16_t`, `uint16_t`, `short` | 2    | 2         |
| `int32_t`, `uint32_t`, `int`   | 4    | 4         |
| pointers, `long`               | 4    | 4         |
| `int64_t`, `long long`         | 8    | 8         |

## Layout rules

C lays out a struct like this:

1. The fields are placed in the order they are declared.
   C never reorders them.
2. Each field starts at the next offset that is a multiple of its alignment.
   Any bytes skipped to get there are **padding**.
3. The struct's alignment is the largest alignment of any of its fields.
4. The struct's size is rounded up to a multiple of its alignment, which may add padding at the end.

Rule 4 is what makes arrays of structs work.
If `sizeof` were not a multiple of the alignment, the second element of an array would start at a misaligned address.

The first field is always at offset 0, so a pointer to a struct is also a pointer to its first field.

C programs can check a layout with `sizeof` and `offsetof`, which comes from `<stddef.h>`:

```c
_Static_assert(offsetof(struct score, points) == 4, "points is at offset 4");
_Static_assert(sizeof(struct score) == 8, "struct score takes 8 bytes");
```

The test files on this track do exactly this, so that the layout you work out by hand is the layout the tests use.

## Ordering fields to save space

Because C keeps the declared order, the same fields can take different amounts of memory:

```c
struct wasteful {   // size 12
    int16_t a;      // offset 0, then 2 bytes of padding
    int32_t b;      // offset 4
    int16_t c;      // offset 8, then 2 bytes of padding
};

struct compact {    // size 8
    int32_t b;      // offset 0
    int16_t a;      // offset 4
    int16_t c;      // offset 6
};
```

Putting the fields with the largest alignment first usually removes most of the padding.
When the struct is shared with C code, though, the assembly must follow whatever order C uses.

## The contents of padding

Padding bytes have no defined value.
A struct written by C code may have anything in its padding, so code must never depend on it.
For example, loading a word at offset 0 of the `wasteful` struct gives `a` in the low 16 bits and padding in the high 16 bits.

## Naming offsets

Numbers like `8(a0)` are easy to get wrong and hard to read.
A comment naming the field helps.
Once you know about the `.equ` directive, you can also give each offset and size a name:

```asm
.equ CRATE_ITEMS,  0
.equ CRATE_WEIGHT, 4
.equ CRATE_AISLE,  8
.equ CRATE_SIZE,   12

        lw      t0, CRATE_WEIGHT(a0)
        addi    a0, a0, CRATE_SIZE
```

If the struct ever changes, only the `.equ` lines need to change.

## Indexing an array of structs

Element `i` of an array of structs is at `base + i * size`.
When the size is a power of 2, a shift does the multiplication.
Otherwise, use `mul`, or build the product from shifts and adds:

```asm
        slli    t0, a1, 3       # t0 = 8 * i
        slli    t1, a1, 2       # t1 = 4 * i
        add     t0, t0, t1      # t0 = 12 * i
```

When every element is visited in order, walking a pointer by `size` avoids the multiplication inside the loop.
Only the end pointer, `base + count * size`, needs one multiplication, before the loop.

The offset of a field still goes in the load or store, so one address register reaches every field of the current element.

## Structs passed by value

Most functions take a pointer to a struct.
C can also pass a struct **by value**.
In the RISC-V calling convention, a struct of up to 8 bytes (on RV32) is passed in one or two argument registers, as if its bytes had been loaded into them as words.
A larger struct is copied to memory by the caller, and the function receives a pointer to the copy.
Small structs are returned in `a0` and `a1`; larger ones are written to memory whose address the caller passes in `a0`.

## Nested structs and unions

A struct can contain another struct.
The inner struct is placed like any other field, at a multiple of its own alignment, and its fields are found by adding the two offsets together.

A `union` is like a struct whose fields all start at offset 0, so they share the same bytes.
Its size is the size of its largest field, rounded up to its alignment.
