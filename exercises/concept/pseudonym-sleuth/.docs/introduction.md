# Introduction

A function returns its result in `a0`.
That is only one register, so a function can only return one value.
But sometimes a job has two or three answers.

## One answer is not always enough

In Python, a function can return several values at once as a **tuple**:

```python
def first_and_last(scores):
    return scores[0], scores[2]

first, last = first_and_last([4, 9, 6])
```

A C function can return only one value.
To give back more, it uses **output parameters**.

## Output parameters

An **output parameter** is a pointer argument that the function writes its answer through.
The function that makes the call is the **caller**.
The caller makes a place for each answer, and passes its address.
The function stores an answer at each address.

In the C declarations in the test file, an output parameter is a pointer without `const`:

```c
extern void copy_and_next(int32_t n, int32_t *copy, int32_t *next);

int32_t copy;
int32_t next;
copy_and_next(41, &copy, &next);
// copy => 41
// next => 42
```

The memory belongs to the caller.
The function only fills it in.

Here is the same idea in Python, with a list that the caller makes and passes in:

```python
def copy_and_next(n, answers):
    answers[0] = n
    answers[1] = n + 1

answers = [0, 0]
copy_and_next(41, answers)
print(answers)  # [41, 42]
```

In assembly, an output parameter arrives in a register as an address, like any pointer.
You store the answer there with `sw`:

```asm
# extern void copy_and_next(int32_t n, int32_t *copy, int32_t *next);
copy_and_next:
        sw      a0, 0(a1)       # the word at address a1 = n
        addi    t0, a0, 1       # t0 = n + 1
        sw      t0, 0(a2)       # the word at address a2 = n + 1
        ret
```

This function is `void`: it returns nothing, and all of its answers go through pointers.

## Returning a value as well

A function can also return one value in `a0` and give back others through pointers.

```c
extern int32_t next_and_copy(int32_t n, int32_t *copy);
```

```asm
next_and_copy:
        sw      a0, 0(a1)       # store n through the output pointer
        addi    a0, a0, 1       # return n + 1 in a0
        ret
```

Look at the order.
`n` arrives in `a0`, and the return value leaves in `a0`.
Use `n` for everything that needs it before you overwrite `a0` with the return value.

## In/out parameters

Sometimes a pointer is both an input and an output.
The function reads the value at the address, works out a new value, and stores it back in the same place.
This is called an **in/out parameter**.

```asm
# extern int32_t add_ten(int32_t *count);
# Adds 10 to the count, and returns the new count.
add_ten:
        lw      t0, 0(a0)       # read the count
        addi    t0, t0, 10      # work out the new count
        sw      t0, 0(a0)       # write it back
        mv      a0, t0          # return the new count
        ret
```

When a function reads through some pointers and writes through others, load every input it needs before it stores anything.
A store can change a value you have not read yet.

## Two return registers

The RISC-V calling convention does allow a function to return a second value in `a1`.
C uses this for values too big for one register, such as 64-bit numbers.
The tests in this exercise use output parameters instead.
