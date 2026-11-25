# About

A **recursive** function calls itself, directly or through other functions.
RISC-V has no special support for recursion, and needs none.
A recursive call is an ordinary `call`, and the stack keeps each call's values apart.

## The shape of a recursive function

Every recursive function needs:

- one or more **base cases**, answered without a recursive call;
- a **recursive case**, which calls the function on a smaller input and combines the answer.

"Smaller" must mean that the calls always reach a base case: a smaller number, a shorter list, or a smaller part of a tree.

In assembly, a good layout is to test for the base case first, and return at once.
Only the recursive case pays for a prologue and an epilogue:

```asm
# Returns 1 + 2 + ... + n.
triangle:
        blez    a0, .Ltriangle_zero     # base case: n <= 0
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)
        mv      s0, a0                  # n survives the call
        addi    a0, a0, -1
        call    triangle                # a0 = triangle(n - 1)
        add     a0, a0, s0
        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret
.Ltriangle_zero:
        li      a0, 0
        ret
```

## One frame per active call

Each call that has started but not yet returned is **active**, and has its own frame on the stack.
The frames hold the return addresses that lead back up through the chain of calls, and the callers' `s` registers.

Two rules from the stack concept matter even more here:

- A value needed after the recursive call must be in an `s` register, or stored in the frame.
  The recursive call runs the same code, so it uses the same `t` and `a` registers for its own values.
- Every `s` register the function changes must be saved and restored.
  The caller that relies on it is usually another copy of the same function.

## Stack depth

The **depth** of a recursion is the largest number of calls active at once.
The stack space used is the depth times the frame size.

| Function                       | Depth for input `n`      | Total number of calls    |
| ------------------------------ | ------------------------ | ------------------------ |
| `triangle(n)`                  | about `n`                | about `n`                |
| one call per digit of `n`      | number of digits         | number of digits         |
| two calls on `n - 1`, `n - 2`  | about `n`                | grows roughly like 1.6ⁿ  |
| a tree of `n` nodes            | the height of the tree   | about `2n`               |

A program's stack is typically a few megabytes.
With 16-byte frames, a depth of a million already needs 16 MB.
When `sp` moves past the end of the stack, the program crashes with a **stack overflow**, usually reported as a segmentation fault.

Python protects itself with a limit, by default 1000 active calls, and raises `RecursionError` when it is reached:

```python
import sys
sys.getrecursionlimit()   # => 1000
```

Assembly has no such limit.
An input that is too deep, or a missing base case, simply runs off the end of the stack.

## Recursion and loops

Any loop can be written as a recursive function:

- the loop's variables become the function's arguments;
- the exit test becomes the base case;
- one iteration of the body happens, and the recursive call does the rest.

Going the other way, a recursive function that makes only one call can usually be turned into a loop, often with an accumulator.
A function that makes two or more calls, like a walk over a tree, needs a stack of its own to become a loop, so the recursive version is usually clearer.

## Tail calls

A call is a **tail call** when it is the very last thing a function does: its result is returned unchanged.

```python
def triangle_acc(n, total):
    if n <= 0:
        return total
    return triangle_acc(n - 1, total + n)   # a tail call
```

After a tail call, the caller has nothing left to do with its frame.
So it can restore `ra` and its `s` registers, give its frame back, and then **jump** to the callee with `tail` instead of `call`.
`tail` is a pseudo-instruction that jumps to a label like `j`, but can reach a label at any distance.
The callee then returns straight to the original caller.

```asm
# Returns total + n + (n - 1) + ... + 1.
triangle_acc:
        blez    a0, .Ltriangle_acc_done
        add     a1, a1, a0              # total = total + n
        addi    a0, a0, -1              # n = n - 1
        tail    triangle_acc            # jump: no new frame, ra unchanged
.Ltriangle_acc_done:
        mv      a0, a1
        ret
```

A tail call to the function itself is really a loop: the stack does not grow at all.
Compilers such as GCC and Clang make this change when they optimise.
Python never does, so even a tail-recursive Python function hits the `RecursionError` limit.

## Recursion over data in memory

A list or tree in memory is made of blocks that hold the addresses of other blocks.
The address 0, `NULL`, means "no block", and is the natural base case.

```c
struct node {
    int32_t value;              // offset 0
    const struct node *left;    // offset 4
    const struct node *right;   // offset 8
};
```

A function over a tree keeps the node's address in an `s` register, so that it can still reach the second child after the call for the first child.
It loads each field with `lw` at a fixed offset from that address.

## Common mistakes

- Forgetting the base case, or a recursive case that does not move towards it.
- Keeping a value in a `t` or `a` register across the recursive call.
- Returning from the base case after the prologue without running the epilogue, or from inside the recursive case before it.
- Loading a field through a `NULL` address, before testing for it.
