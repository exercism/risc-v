# About

## Arrays in memory

An array is a block of memory holding elements of the same type, one straight after another, with no gaps.
In C, an array passed to a function **decays** to a pointer to its first element.
The function never sees the array itself, only that address.

So a C function that works on an array almost always takes a pointer and a length:

```c
int32_t total(const int32_t *values, int32_t count);
void scale(int32_t *values, int32_t count, int32_t factor);
```

Under the calling convention, the pointer and the length are ordinary arguments in `a0`–`a7`.
Nothing in memory records the length, so a function that reads past `values + 4 * count` reads whatever happens to be there.
C does not check array bounds, and neither does the processor.

## Element addresses

For an array whose elements are `size` bytes each, element `i` is at:

```text
address of values[i] = values + size * i
```

| C type                 | Element size | Shift for `size * i` | Load / store    |
| ---------------------- | ------------ | -------------------- | --------------- |
| `int32_t`, `uint32_t`  | 4            | `slli t0, i, 2`      | `lw` / `sw`     |
| `int16_t`, `uint16_t`  | 2            | `slli t0, i, 1`      | `lh`, `lhu` / `sh` |
| `int8_t`, `uint8_t`    | 1            | none                 | `lb`, `lbu` / `sb` |
| `int64_t`              | 8            | `slli t0, i, 3`      | two `lw` / `sw` on RV32 |

Element sizes are powers of two, so the multiplication is always a shift.
For other sizes, such as an array of 12-byte structs, use `mul` or a mix of shifts and adds.

In C, `values[i]` and `*(values + i)` mean the same thing.
C pointer arithmetic counts in elements, so the compiler multiplies `i` by the element size for you.
In assembly, addresses count in bytes, so you must do it yourself.

## Constant indices

When the index is a constant, fold it into the load or store offset:

```asm
        lw      t0, 12(a0)      # values[3]
```

The offset is a 12-bit signed immediate, so this works for indices from -512 to 511 of an `int32_t` array.

## Indexing versus pointer walking

There are two common shapes for a loop over an array.

**Indexing** keeps `i` and recomputes the address each time:

```asm
        li      t0, 0                   # i
loop:
        bge     t0, a1, done
        slli    t1, t0, 2
        add     t1, a0, t1
        lw      t2, 0(t1)               # values[i]
        # ...
        addi    t0, t0, 1
        j       loop
done:
```

**Pointer walking** keeps a pointer to the current element and stops at an end pointer:

```asm
        slli    t0, a1, 2
        add     t0, a0, t0              # end = values + 4 * count
loop:
        beq     a0, t0, done
        lw      t2, 0(a0)               # *p
        # ...
        addi    a0, a0, 4
        j       loop
done:
```

Pointer walking uses fewer instructions per element and fewer registers.
Compilers usually turn an indexed loop into a pointer walk for this reason.
Use indexing when you need the index itself, or when you jump to elements out of order.

The end pointer points one element **past** the end of the array.
C allows this address to be computed and compared, but not to be read or written.

## Ending the loop

`beq p, end, done` (or `bne p, end, loop` at the bottom) is the safest test, because the pointer moves in steps of 4 and lands exactly on `end`.
Signed tests such as `bge p, end, done` also work for the addresses in these exercises, but addresses are really unsigned numbers.
The unsigned branches `bltu` and `bgeu` are the right choice when you compare addresses for order; a later concept covers them.

A test at the top of the loop handles an empty array (`count == 0`) for free.
A test at the bottom needs a check before the loop, or a guarantee that `count >= 1`.

## Counting down

Another common shape counts the length down to zero, moving the pointer at the same time:

```asm
loop:
        beqz    a1, done
        lw      t2, 0(a0)
        # ...
        addi    a0, a0, 4
        addi    a1, a1, -1
        j       loop
done:
```

This needs no end pointer, but it updates two registers each time round.

## Turning a pointer into an index

If `start` holds the address of element 0 and `p` points at some element, then `(p - start) / 4` is its index.
Because `p - start` is a multiple of 4 and never negative, `srli` by 2 does the division exactly.

## Comparing with Python

A Python list holds references to objects, and knows its own length.
A C array of `int32_t` holds the numbers themselves, packed 4 bytes apart, and the length travels separately.

| Python                   | Assembly                                       |
| ------------------------ | ---------------------------------------------- |
| `values[i]`              | `slli t0, i, 2`, `add t0, a0, t0`, `lw x, 0(t0)` |
| `len(values)`            | the separate `count` argument                  |
| `for x in values:`       | walk a pointer from `values` to `end`          |
| `for i in range(len(values)):` | an index loop from 0 to `count - 1`      |
| `values[i] = x`          | work out the address, then `sw x, 0(t0)`       |
| `IndexError`             | nothing: an out-of-range index reads other memory |
