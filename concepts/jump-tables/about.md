# About

A **jump table** is an array of code addresses.
A program picks an entry by number and jumps to the address stored there.
It is the usual way to build a `switch` statement with many cases close together, and the same idea, a **function pointer**, lets one piece of code call another without knowing its name in advance.

## Code addresses

Every instruction is stored in memory, so every label in `.text` has an address, exactly like a label in `.rodata` or `.data`.
`la rd, label` loads that address, and `.word label` stores it in memory.
When the program is linked, the linker writes the real address into each `.word`.

On RV32, a code address is 32 bits, the same size as any other pointer.
RISC-V instructions are 4 bytes, or 2 bytes with the compressed (C) extension, so a code address is always even.

## Indirect jumps

`jalr rd, offset(rs)` jumps to the address `rs + offset` (with the lowest bit cleared), and writes the address of the next instruction to `rd`.

| You write      | The assembler produces  | Meaning                                    |
| -------------- | ----------------------- | ------------------------------------------ |
| `jr rs`        | `jalr zero, 0(rs)`      | jump to the address in `rs`                |
| `jalr rs`      | `jalr ra, 0(rs)`        | call the function whose address is in `rs` |
| `ret`          | `jalr zero, 0(ra)`      | return                                     |

The offset is a 12-bit immediate, so `jalr zero, 8(t0)` jumps 8 bytes past the address in `t0`.

## Building a switch

A compiler turns a dense `switch` into a jump table in four steps:

```asm
        li      t0, COUNT
        bgeu    a0, t0, .Ldefault       # unsigned: catches negative values too
        slli    t1, a0, 2               # 4 bytes per entry
        la      t2, table
        add     t2, t2, t1
        lw      t2, 0(t2)               # the code address for this case
        jr      t2
```

- If the smallest case is not 0, subtract it first, as in the one-comparison range check.
- Missing values in the middle of the range point at the default code.
- If the cases are far apart, such as 1, 100 and 5000, a table would be mostly empty, and a compiler uses a chain of branches or a binary search instead.

A table entry can point at a block inside the same function, which ends with `ret` or with `j` back to a loop, or at a separate function.

## Function pointers

C writes the type of a pointer to a function that takes two `int32_t` and returns one as `int32_t (*)(int32_t, int32_t)`.
A `typedef` often makes this easier to read:

```c
typedef int32_t (*binary_op)(int32_t, int32_t);
extern int32_t apply(binary_op fn, int32_t a, int32_t b);
```

A function pointer is passed and returned in a register like any other pointer.
A null function pointer is 0, and jumping to it crashes the program.

## Calling through a pointer

When the call is the very last thing a function does, it can be a **tail call**: set up the arguments, then `jr` to the pointer.
The callee's `ret` returns straight to the original caller, because `ra` was never changed.

Otherwise, the caller uses `jalr` and must keep everything it needs after the call somewhere the callee will not touch.
A function reached through a pointer may be anyone's code, so it may change every caller-saved register: `ra`, `a0`–`a7` and `t0`–`t6`.
The values must therefore be kept on the stack or in the saved registers `s0`–`s11`:

```asm
        addi    sp, sp, -16
        sw      ra, 12(sp)
        sw      s0, 8(sp)
        mv      s0, a2                  # keep b
        jalr    a0                      # call fn: ra = return address
        add     a0, a0, s0              # fn's result + b
        lw      s0, 8(sp)
        lw      ra, 12(sp)
        addi    sp, sp, 16
        ret
```

## Uses

- `switch` statements and interpreters: a virtual machine reads an instruction code and jumps to the code that carries it out.
- Callbacks: C's `qsort` takes a pointer to a comparison function.
- Tables of handlers, such as the interrupt vector table of a microcontroller, or a table of system calls.
- Virtual methods in C++ and other object-oriented languages are called through tables of function pointers.

## Security

Because the jump goes wherever the table entry says, a jump table must always be range-checked.
An unchecked index lets bad input pick any word in memory as a code address.
Attackers look for exactly this, which is why some processors add checks that an indirect jump lands on an allowed instruction.
RISC-V's Zicfilp extension does this with a special landing-pad instruction.
