# About

## Sections

An object file is made of **sections**.
The linker gathers the sections of the same name from every file, and the operating system loads each kind into memory with its own permissions.

| Section   | Directive                         | Holds                                  | Permissions         |
| --------- | --------------------------------- | -------------------------------------- | ------------------- |
| `.text`   | `.text`                           | instructions                           | read, execute       |
| `.rodata` | `.section .rodata`                | constant data: strings, tables         | read                |
| `.data`   | `.data`                           | variables with a starting value        | read, write         |
| `.bss`    | `.bss`                            | variables that start as zero           | read, write         |

`.bss` takes no space in the program file.
The file only records how big it is, and the loader fills that much memory with zeros.
Only `.zero`, `.space` and alignment belong in `.bss`, because there is nowhere to keep any other value.

The general form `.section name` works for any section, so `.section .data` and `.section .text` are also valid.
`.text`, `.data` and `.bss` are short ways to write the common ones.
There is no short `.rodata` directive.

A store into `.rodata` or `.text` is stopped by the processor's memory protection.
On Linux the program is killed with the signal `SIGSEGV`, a **segmentation fault**.
This is a feature: a bug that tries to change a constant is caught at once, rather than quietly changing a value that other code relies on.

## Data directives

| Directive                 | Size of each value | Notes                                          |
| ------------------------- | ------------------ | ---------------------------------------------- |
| `.byte`                   | 1 byte             |                                                |
| `.half`, `.2byte`, `.short` | 2 bytes          |                                                |
| `.word`, `.4byte`, `.long` | 4 bytes           | the size of a register on RV32                 |
| `.dword`, `.8byte`, `.quad` | 8 bytes          |                                                |
| `.ascii "s"`              | 1 byte per byte    | no terminator                                  |
| `.string "s"`, `.asciz "s"` | 1 byte per byte  | adds a zero terminator                         |
| `.zero n`, `.space n`     | `n` bytes          | all zero; `.space n, v` fills with `v`         |

Each directive takes a list of values separated by commas.
A value may be an expression the assembler can work out, such as `'A' + 1`, `60 * 60` or the address of another label.

Inside a string, the usual escapes work: `\n` is a newline, `\t` a tab, `\"` a double quote and `\\` a backslash.
Several strings may follow one `.ascii` or `.string`, and with `.string` each one gets its own terminator.

RISC-V is **little-endian**: the lowest byte of a `.half` or `.word` comes first in memory.
`.word 0x11223344` stores the bytes `44 33 22 11`.

## Alignment

`.balign n` pads with zero bytes until the address is a multiple of `n`.
`.p2align k` does the same for a multiple of 2 to the power `k`, so `.p2align 2` is `.balign 4`.
`.align` means `.p2align` on RISC-V, but `.balign` on some other processors, so `.balign` and `.p2align` are clearer.

The data directives do not align themselves.
Put `.balign 2` before halfwords, and `.balign 4` before words, whenever something of a different size comes before them.
Each section starts at a well-aligned address, so the first item in a section in a file is usually aligned already, but writing `.balign` anyway costs nothing.

## Addresses

`la rd, symbol` loads the address of a symbol.
In the programs on this track it becomes:

```asm
.Lpcrel:
        auipc   rd, %pcrel_hi(symbol)           # rd = this pc + the upper 20 bits of the distance
        addi    rd, rd, %pcrel_lo(.Lpcrel)      # add the lower 12 bits
```

`auipc` (add upper immediate to `pc`) adds a 20-bit immediate, shifted left by 12, to the address of the `auipc` itself.
Together with `addi`, this reaches any address within 2 GiB of the code, and it works wherever the program is loaded.

The assembler can also load or store straight through a label, using the same pair of instructions:

```asm
        lw      t0, counter             # t0 = the word at counter
        sw      t0, counter, t1         # the word at counter = t0, using t1 for the address
```

`sw` needs a spare register to hold the upper part of the address.

`lui` with `%hi(symbol)` and `addi` with `%lo(symbol)` build an absolute address instead.
That only works when the program is always loaded at the same address.

## Global data

`.global name` exports a data label in the same way as a function label, so that C code can declare it:

```c
extern int32_t visits;                // a .word in .data
extern const char motto[];            // a .string in .rodata
```

The C names refer to the data itself, not to a pointer, so `motto` here is the address of the first character.

## Variables that last

Data in `.data` and `.bss` lives for the whole run of the program.
In C it is called a **static** or **global** variable, and in Python it is a module-level variable.

Every caller shares the same copy.
Two calls that change it at the same time, from two threads, can lose an update.
The programs on this track run one thread, so that does not happen here.
