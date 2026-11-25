# About

A **local array** is an array that lives in a function's own stack frame.
It is the assembly version of a temporary list made inside a Python function, and of an array declared inside a C function:

```c
int32_t count_sizes(const int32_t *values, int32_t n) {
    int32_t counts[8] = {0};    // a local array, in the stack frame
    // ...
}
```

A temporary array like this is often called a **buffer** or **scratch** space.

## Laying out the frame

A local array is room in the frame, made with the same `addi sp, sp, -N` as the room for saved registers.
A common layout puts the array at the bottom and the saved registers at the top:

| Offset from `sp` | Contents                      |
| ---------------- | ----------------------------- |
| `N - 4`          | `ra`                          |
| `N - 8`          | `s0`                          |
| …                | other saved registers, unused |
| `4 * k - 4`      | last element of the array     |
| …                | …                             |
| `0`              | element 0 of the array        |

With the array at offset 0, `sp` itself is the array's address, which keeps the address arithmetic short.

`N` must be a multiple of 16, because the psABI requires `sp` to be a multiple of 16 at every call.
Add up the bytes for the array and for the saved registers, then round up.
An array of 16 words (64 bytes) and `ra` (4 bytes) need 68 bytes, so the frame is 80 bytes.

The immediate in `addi` is 12 bits, so `addi sp, sp, -N` works for frames up to 2048 bytes.
A bigger frame needs the size in a register first: `li t0, 4096` and `sub sp, sp, t0`.

## Garbage

The stack is reused all the time.
The memory of a new frame still holds whatever the last function that used it stored there, such as old return addresses and old loop counters.
Nothing clears it, so a local array starts full of **garbage**.

If your code adds to the elements, or reads them before writing them, store zeros first:

```asm
        mv      t0, sp          # pointer to element 0
        addi    t1, sp, 64      # end of a 16-word array
clear_loop:
        beq     t0, t1, clear_done
        sw      zero, 0(t0)
        addi    t0, t0, 4
        j       clear_loop
clear_done:
```

A program with an uninitialised array may seem to work, because the memory happened to be zero, and then fail later when a different function has run first.

## Passing the address on

The address of a local array is an ordinary pointer.
Put it in an argument register and call:

```asm
        mv      a0, sp          # a0 = &array[0]
        addi    a1, sp, 16      # a1 = &array[4]
        call    compare
```

The callee's frame goes below yours, so the array is untouched by the call itself, although the callee may read or write it through the pointer.
`sp` must still be a multiple of 16 at the `call`.

## Lifetime: never return a pointer to a local array

A local array exists from the moment its function makes room for it until the function gives the room back.
After that, the memory belongs to whichever function is called next.

```asm
# WRONG: returns the address of memory that is about to be reused.
broken_make_buffer:
        addi    sp, sp, -32
        mv      a0, sp          # a0 = address of the local array
        addi    sp, sp, 32      # the array is gone
        ret                     # the caller gets a dangling pointer
```

A pointer to memory that is no longer valid is called a **dangling pointer**.
Reading through it gives garbage, and writing through it corrupts other functions' frames, often far from the real mistake.
If the caller needs an array of results, the caller should pass in the address of its own array, and the callee fills it.

Python never has this problem, because a list stays alive for as long as anything refers to it, and the garbage collector frees it later.

## Variable-size arrays

When the size is known only at run time, compute it in a register and subtract it from `sp`:

```asm
        slli    t0, a1, 2       # bytes = 4 * n
        addi    t0, t0, 15
        andi    t0, t0, -16     # round up to a multiple of 16
        sub     sp, sp, t0
```

`-16` is `0xFFFFFFF0`: every bit set except the lowest four.
`andi` with it clears those four bits, which rounds down to a multiple of 16.
Adding 15 first makes it round up instead, and a size that is already a multiple of 16 stays the same.
When `n` is 0 the size is 0, and `sp` does not move.

After the `sub`, the distance from `sp` to the saved registers is no longer a constant, so they cannot be reached with fixed offsets from `sp`, and `addi sp, sp, N` cannot remove the array.
The usual answer is a **frame pointer**:

```asm
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)
        mv      s0, sp          # s0 (fp) = sp before the array

        # ... compute the size in t0 ...
        sub     sp, sp, t0      # the array is at sp

        # ... use the array; the saved registers are at 12(s0) and 8(s0) ...

        mv      sp, s0          # remove the array, whatever its size
        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret
```

`s0` is callee-saved, so it keeps its value across any calls made while the array exists.
That is why it, and not a `t` register, is used as the frame pointer.

In C this is what `alloca(n)` and variable-length arrays such as `int32_t copy[n];` compile to.
Compilers use `s0` as the frame pointer in exactly this way.

## Stack overflow

The stack has a fixed size.
On Linux the main thread's stack is usually 8 MB, other threads often get less, and small embedded systems may have only a few kilobytes.
A local array that is larger than the space left moves `sp` past the end of the stack.

Operating systems usually place an unmapped **guard page** just past the end of the stack, so the first access there crashes the program with a **stack overflow** (a segmentation fault on Linux).
A very large array can jump right over the guard page and silently overwrite other memory, so large buffers, or buffers whose size comes from outside the program, belong somewhere else: an array passed in by the caller, or the data section.

## Common mistakes

- Forgetting to set a counting array to zero.
- A frame size that is not a multiple of 16, or a variable size that is not rounded up.
- Restoring `sp` with `addi` after a `sub` by a run-time amount.
- Using `s0` as a frame pointer without saving the caller's `s0` first.
- Storing past the end of the local array, which overwrites the saved registers above it, often `ra`, so `ret` jumps somewhere strange.
- Returning, or storing somewhere lasting, the address of a local array.
