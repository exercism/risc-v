# About

## Why output parameters exist

The RISC-V calling convention returns a function's result in `a0`.
C lets a function return only one value, so a function with several results needs another way to give them back.
The usual way is an **output parameter**: a pointer argument that the function stores a result through.

```c
void min_max(const int32_t *values, int32_t *min, int32_t *max);

int32_t lo;
int32_t hi;
min_max(readings, &lo, &hi);
```

In Python you would return a tuple instead:

```python
def min_max(values):
    return min(values), max(values)

lo, hi = min_max(readings)
```

Python can also fill in a list the caller passes, which is closer to what C does:

```python
def min_max(values, answers):
    answers[0] = min(values)
    answers[1] = max(values)
```

## Who owns the memory

The **caller** owns the memory that an output parameter points to.
It decides where the answer goes, often a local variable, and passes that variable's address with `&`.
The function only writes to it, and must not keep the address for later.

Because the caller owns the memory, an output parameter costs nothing to set up inside the function.
In assembly it is just an address in `a0`–`a7`, and storing the result is one `sw`.

## Kinds of pointer parameter

| C parameter          | Kind      | What the function does                             |
| -------------------- | --------- | -------------------------------------------------- |
| `const int32_t *p`   | input     | only reads through `p`                             |
| `int32_t *out`       | output    | only writes through `out`; the old value is ignored |
| `int32_t *inout`     | in/out    | reads through it, then writes a new value back      |

C uses the same type, `int32_t *`, for output and in/out parameters.
Only the function's documentation says whether the old value is read.

## Returning a value and writing outputs

A function can both return a value in `a0` and store more results through pointers.
A common pattern returns a status or a count in `a0` and the real data through pointers.
Another returns the old value of an in/out parameter, so the caller can see both the before and after.

Remember that the first argument also arrives in `a0`.
If the function needs that argument after it has worked out its return value, copy it to a temporary register first, or set `a0` last.

## Aliasing

Two pointers **alias** when they hold the same address, or when their values overlap in memory.
The caller may pass an output pointer that aliases an input:

```c
int32_t clues[3] = {4, 5, 6};
case_summary(clues, &clues[0], &clues[2]);
```

Here the function stores its first result over `clues[0]`, which is one of its own inputs.
If it stores before it has loaded every input, it reads the new value instead of the old one, and gets the wrong answer.

The safe order is:

1. load every input value into registers;
2. work out every result in registers;
3. store the results.

The same applies to in/out parameters that might alias each other.
C has a keyword, `restrict`, that lets a programmer promise two pointers never alias, which allows a compiler to reorder loads and stores.
In hand-written assembly, you are the one who decides the order.

## Returning two values in registers

The calling convention does define two return registers, `a0` and `a1`.
On RV32, a function that returns a 64-bit integer such as `int64_t` returns the low 32 bits in `a0` and the high 32 bits in `a1`.
A small struct of up to two words can be returned the same way.
Larger structs are returned through a hidden pointer: the caller makes room, and passes its address in `a0`, which is an output parameter that C writes for you.

C cannot name two separate `int32_t` return values, so for ordinary results this track uses output parameters.
