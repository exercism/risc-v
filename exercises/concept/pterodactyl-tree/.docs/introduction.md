# Introduction

A **recursive** function is a function that calls itself.
In assembly, it is just a function that does a `call` to its own label.
Everything you learnt about the stack makes this work.

## Base case and recursive case

Here is the factorial of `n`, written recursively in Python.
The factorial of 4 is `4 * 3 * 2 * 1`, which is 24.

```python
def factorial(n):
    if n <= 1:
        return 1                    # base case
    return n * factorial(n - 1)     # recursive case
```

Every recursive function has two parts:

- the **base case**, a small input whose answer is known straight away, with no call;
- the **recursive case**, which calls the same function on a **smaller** input, and then uses that answer.

Each call makes the input smaller, so the calls must reach the base case in the end.
If they never do, the function calls itself for ever.

Here is the same function in assembly:

```asm
# Returns n! (n factorial).
factorial:
        li      t0, 1                   # t0 = 1
        bgt     a0, t0, .Lfactorial_recurse  # if n > 1, go to the recursive case
        li      a0, 1                   # base case: the answer is 1
        ret                             # no frame was made, so just return

.Lfactorial_recurse:
        addi    sp, sp, -16             # make this call's frame
        sw      ra, 12(sp)              # save this call's return address
        sw      s0, 8(sp)               # save the caller's s0

        mv      s0, a0                  # keep n: it must survive the call
        addi    a0, a0, -1              # a0 = n - 1
        call    factorial               # a0 = factorial(n - 1)
        mul     a0, a0, s0              # a0 = factorial(n - 1) * n

        lw      s0, 8(sp)               # restore the caller's s0
        lw      ra, 12(sp)              # restore this call's return address
        addi    sp, sp, 16              # give the frame back
        ret
```

The base case calls nothing, so it needs no frame.
The recursive case makes a call, so it saves `ra` and `s0` just like any other function from the stack concept.

## Every call has its own frame

When `factorial(3)` runs, it calls `factorial(2)`, which calls `factorial(1)`.
At that moment, three copies of `factorial` are running at once, and each one has its own stack frame:

| Call           | Its frame                   | Its saved `ra` goes back to | It keeps `n` in `s0` |
| -------------- | --------------------------- | --------------------------- | -------------------- |
| `factorial(3)` | highest in memory           | the tests                   | 3                    |
| `factorial(2)` | just below `factorial(3)`'s | inside `factorial(3)`       | 2                    |
| `factorial(1)` | none: it is the base case   | –                           | –                    |

The code is the same for every call, but the registers are not.
`factorial(2)` puts 2 in `s0`, so it first saves the 3 that `factorial(3)` left there.
When `factorial(2)` returns, it restores `s0`, and `factorial(3)` finds its own 3 again.
This is why a recursive function must follow the calling convention exactly: its caller is very often itself.

A value kept in a `t` or `a` register does **not** survive the recursive call, because the inner call uses the same registers for its own work.

## Recursion uses stack space

Each call that has not yet returned keeps its frame on the stack.
`factorial(1000)` would have about 1000 frames at once, and with 16 bytes each, that is 16 000 bytes of stack.
The stack is large, but not endless.

Python stops deep recursion for you.
By default, it allows about 1000 calls at once, and then raises a `RecursionError`:

```python
def forever(n):
    return forever(n + 1)    # no base case

forever(0)
# RecursionError: maximum recursion depth exceeded
```

Assembly has no such check.
A recursive function with no base case, or one that never reaches it, keeps moving `sp` down until it runs past the end of the stack.
The program then crashes with a **stack overflow**.

## Loops and recursion

Any loop can be written as recursion, and many recursive functions can be written as loops.
Here is a loop that adds up the numbers from `n` down to 1, and the same idea written recursively:

```python
def total_loop(n):
    total = 0
    while n > 0:
        total = total + n
        n = n - 1
    return total

def total_recursive(n):
    if n <= 0:
        return 0                            # the loop's exit test is the base case
    return n + total_recursive(n - 1)       # one iteration, then "the rest of the loop"
```

The loop's exit test becomes the base case.
The loop body does one step, and the recursive call does all the steps that are left.

A loop is usually shorter and faster in assembly, because it needs no frames.
Recursion is the natural choice when a problem splits into smaller problems of the same shape, as with trees.

## A function that calls itself twice

A recursive case may make more than one call.
Then the first answer must survive the second call, so it goes in an `s` register too:

```python
def tiles(n):
    if n <= 2:
        return n
    return tiles(n - 1) + tiles(n - 2) * 2
```

The calls branch out like a tree: `tiles(5)` calls `tiles(4)` and `tiles(3)`, and each of those calls two more.
The number of calls grows very quickly as `n` grows, but the stack only ever holds one path of calls at a time, from the first call down to the one that is running.
So the stack depth is about `n`, even when there are thousands of calls.

## Linked data in memory

A block of words in memory can hold the address of another block.
A **linked list** is a chain of such blocks.
Here each block is two words: a value at offset 0, and the address of the next block at offset 4.

```c
struct link {
    int32_t value;              // offset 0
    const struct link *next;    // offset 4
};
```

The last block has the address 0 in its `next` word.
Address 0 is never a real block, so it is used to mean "nothing here".
It is called the **null pointer**, or `NULL`, and it plays the part of Python's `None`.

Recursion follows the links naturally.
This function adds up every value in a list:

```python
def list_total(link):
    if link is None:
        return 0
    return link.value + list_total(link.next)
```

```asm
# Returns the sum of the values in the list that starts at a0.
list_total:
        beqz    a0, .Llist_total_empty  # an empty list: the base case

        addi    sp, sp, -16             # make this call's frame
        sw      ra, 12(sp)              # save our return address
        sw      s0, 8(sp)               # save the caller's s0

        lw      s0, 0(a0)               # s0 = this block's value
        lw      a0, 4(a0)               # a0 = the address of the next block
        call    list_total              # a0 = the total of the rest of the list
        add     a0, a0, s0              # add this block's value

        lw      s0, 8(sp)               # restore the caller's s0
        lw      ra, 12(sp)              # restore our return address
        addi    sp, sp, 16              # give the frame back
        ret

.Llist_total_empty:
        li      a0, 0                   # an empty list adds up to 0
        ret
```

## Trees

In a **tree**, each block may point to **two** (or more) other blocks, called its **children**.
The block at the top, which no other block points to, is the **root**.
A child that is `NULL` means there is no child there.

```c
struct node {
    int32_t value;              // offset 0
    const struct node *left;    // offset 4
    const struct node *right;   // offset 8
};
```

A tree is made of smaller trees: each child is the root of its own smaller tree.
So a function over a tree usually has the empty tree (`NULL`) as its base case, and calls itself once for each child.
