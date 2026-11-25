# Introduction

An **array** is a list of values of the same type, stored one after another in memory.
It is the assembly version of a Python list.

## An array as an argument

A C function cannot receive a whole array in its registers.
Instead, it receives two arguments: the address of the first value, and how many values there are.

```c
extern int32_t sum_of_squares(const int32_t *values, int32_t count);
```

- `values` arrives in `a0`: it is the address of the first value.
- `count` arrives in `a1`: it is the **length** of the array.

Each value in an array is called an **element**.
The element's position is its **index**, and the first element has index 0, just as in Python.

The array does not know its own length.
Nothing in memory marks where it ends, so your code must use `count` to know when to stop.
A count can be 0: then the array is **empty**, and your code must not read any element at all.

## Where is element `i`?

You saw in the memory concept that each `int32_t` takes 4 bytes.
So element `i` starts `4 * i` bytes after element 0:

| Element     | Address        |
| ----------- | -------------- |
| `values[0]` | `values`       |
| `values[1]` | `values + 4`   |
| `values[2]` | `values + 8`   |
| `values[i]` | `values + 4*i` |

The number of bytes in each element is called the **element size**.
It is 4 for `int32_t`.
Later exercises use arrays with smaller elements.

## Indexing

When the index is a number you know in advance, you can write the offset straight into `lw` or `sw`, as in the memory concept.
When the index is in a register, you must work out the address yourself.
Multiplying by 4 is the same as shifting left by 2, so this takes two instructions:

```asm
# Sets values[i] to 0.
# values arrives in a0, i arrives in a1.
clear_entry:
        slli    t0, a1, 2       # t0 = 4 * i, the offset in bytes
        add     t0, a0, t0      # t0 = values + 4 * i, the address of values[i]
        sw      zero, 0(t0)     # values[i] = 0
        ret
```

```python
def clear_entry(values, i):
    values[i] = 0
```

Remember to shift.
Adding `i` itself gives an address in the middle of an element, which is wrong.

## A loop with an index

To visit every element, you can loop with an index from 0 to `count - 1`, and work out each address as above.
This function stores the same value into every element:

```asm
# Sets every element of values to value.
# values arrives in a0, count in a1, value in a2.
fill:
        li      t0, 0           # t0 = i = 0
fill_loop:
        bge     t0, a1, fill_done  # if i >= count, leave the loop
        slli    t1, t0, 2       # t1 = 4 * i
        add     t1, a0, t1      # t1 = the address of values[i]
        sw      a2, 0(t1)       # values[i] = value
        addi    t0, t0, 1       # i = i + 1
        j       fill_loop       # go back to the test
fill_done:
        ret
```

```python
def fill(values, value):
    for i in range(len(values)):
        values[i] = value
```

## Walking a pointer

There is a shorter way.
Keep a register that holds the address of the current element.
After each element, add 4 to it, so it points at the next one.
This is called **walking a pointer** through the array.

Before the loop, work out the address just **past** the last element.
This is the **end pointer**: `end = values + 4 * count`.
The loop stops when the pointer reaches the end.

```asm
# Sets every element of values to value.
# values arrives in a0, count in a1, value in a2.
fill:
        slli    t0, a1, 2       # t0 = 4 * count
        add     t0, a0, t0      # t0 = end = values + 4 * count
fill_loop:
        beq     a0, t0, fill_done  # if the pointer has reached the end, stop
        sw      a2, 0(a0)       # store value in the current element
        addi    a0, a0, 4       # move the pointer to the next element
        j       fill_loop       # go back to the test
fill_done:
        ret
```

The end pointer is not the address of an element.
Never load from it or store to it: it only marks where to stop.

This version needs no index and no shift inside the loop.
It is like Python's `for x in values:`, which gives you each element in turn without an index.

Reading works the same way, with `lw` instead of `sw`:

```asm
# Returns the sum of the squares of the elements of values.
# values arrives in a0, count in a1.
sum_of_squares:
        slli    t0, a1, 2       # t0 = 4 * count
        add     t0, a0, t0      # t0 = end
        li      t1, 0           # t1 = total = 0
sum_of_squares_loop:
        beq     a0, t0, sum_of_squares_done  # if the pointer has reached the end, stop
        lw      t2, 0(a0)       # t2 = x, the current element
        mul     t2, t2, t2      # t2 = x * x
        add     t1, t1, t2      # total = total + x * x
        addi    a0, a0, 4       # move the pointer to the next element
        j       sum_of_squares_loop  # go back to the test
sum_of_squares_done:
        mv      a0, t1          # return total
        ret
```

```python
def sum_of_squares(values):
    total = 0
    for x in values:
        total = total + x * x
    return total
```

## Empty arrays

Both loops above test at the top.
When `count` is 0, the end pointer equals the start, so the loop body never runs and nothing in memory is touched.
A do-while loop, with the test at the bottom, would read or write one element even for an empty array.
Use a do-while loop only when you know the array has at least one element.

## From a pointer back to an index

Sometimes you walk a pointer but need to know the index of the current element.
Keep a copy of the start address before the loop moves the pointer.
Then subtract the start address from the pointer to get the offset in bytes, and shift right by 2 to divide by 4:

```asm
        sub     t3, a0, t4      # t3 = pointer - start (start was saved in t4)
        srli    t3, t3, 2       # t3 = offset / 4, the index
```

You can also keep an index in another register, and add 1 to it each time you move the pointer.
