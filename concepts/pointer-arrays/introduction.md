# Introduction

You have seen arrays of numbers, and strings, which are arrays of characters.
Often a program needs a list of strings, such as a list of names.
In Python that is just a list:

```python
names = ["Ruth", "Ben", "Lusi"]
print(names[1])     # Ben
print(names[1][0])  # B
```

In assembly, a list of strings is an **array of pointers**.

## An array of pointers

A pointer is an address, and an address is a 32-bit number.
So a pointer takes 4 bytes, exactly like an `int32_t`.
An array of pointers is an array of words, and each word is the address of the first character of a string.

The strings themselves are somewhere else in memory.
They can have different lengths, and they do not have to be next to each other.

```
names ──► ┌────────────┐
          │ address 1  │ ──► "Ruth"
          ├────────────┤
          │ address 2  │ ──► "Ben"
          ├────────────┤
          │ address 3  │ ──► "Lusi"
          └────────────┘
```

In the C declarations in the test file, an array of pointers to strings looks like this:

```c
extern int32_t total_length(const char *const *names, int32_t count);
```

Read the type `const char *const *` from right to left:

- the last `*` says that `names` is a pointer, the address of the array;
- the `const` before it says the function only reads the pointers in the array;
- `const char *` says each element is the address of a string that the function only reads.

You will also see `const char **`, for an array whose pointers the function may change.
Two `*` mean two steps: from the array to a pointer, and from that pointer to a character.

## Two steps to a character

To reach a character, you follow two addresses.
This is called **two levels of indirection**.

1. Load the element with `lw`, because the element is a 4-byte pointer.
2. Load a character of that string with `lbu`, using the pointer you just loaded.

```asm
        lw      t0, 4(a0)       # t0 = names[1], the address of "Ben"
        lbu     t1, 0(t0)       # t1 = names[1][0], the character 'B'
```

Be careful to use `lw` for the first step.
`lbu` would load only one byte of the address, which is not a pointer to anything.

Element `i` of an array of pointers is at offset `4 * i`, as in any array of words.
After you have a string's pointer, character `c` of that string is at offset `c`, because characters are 1 byte each.

## A grid of characters

A grid, such as a map or a game board, can be stored as an array of rows, where each row is a string.
Row `r`, column `c` is then `rows[r][c]`: first load the row's pointer, then load the character in that row.

```asm
# rows arrives in a0, and a column number c arrives in a1.
        lw      t0, 8(a0)       # t0 = rows[2], the address of row 2
        add     t0, t0, a1      # t0 = the address of rows[2][c]
        lbu     t1, 0(t0)       # t1 = rows[2][c]
```

When the row number is in a register, work out the offset `4 * r` with a shift, as you did for arrays.

## Where does the list end?

A list of strings can tell you its length in two ways.

The first way is the one you know from arrays: a pointer and a count, passed as two arguments.

The second way is like the zero terminator of a string.
The last element is a pointer with the value 0, called a **null pointer**.
In C it is written `NULL`.
No string can be at address 0, so a null pointer cannot be confused with a real one.
An array that ends like this is **NULL-terminated**.

This function counts the empty strings in a NULL-terminated list:

```asm
# Returns how many of the strings in names are empty.
# names arrives in a0, and ends with a null pointer.
count_empty:
        li      t0, 0                   # t0 = count = 0
.Lcount_empty_loop:
        lw      t1, 0(a0)               # t1 = the current pointer
        beqz    t1, .Lcount_empty_done  # a null pointer: the end of the list
        lbu     t2, 0(t1)               # t2 = the first character of the string
        bnez    t2, .Lcount_empty_next  # not empty
        addi    t0, t0, 1               # one more empty string
.Lcount_empty_next:
        addi    a0, a0, 4               # move on to the next pointer
        j       .Lcount_empty_loop
.Lcount_empty_done:
        mv      a0, t0                  # return count
        ret
```

```python
def count_empty(names):
    count = 0
    for name in names:
        if name == "":
            count = count + 1
    return count
```

The pointer into the array moves by 4, because each element is a word.
A pointer into a string moves by 1.

## A loop inside a loop

Often you need to look at every character of every string.
Then there are two loops: an **outer loop** over the array, and an **inner loop** over the characters of the current string.
Keep each pointer in its own register, so that the inner loop does not lose your place in the array.

```asm
# Returns the total number of characters in all the strings.
# names arrives in a0, count in a1.
total_length:
        slli    t0, a1, 2               # t0 = 4 * count
        add     t0, a0, t0              # t0 = end of the array
        li      t1, 0                   # t1 = total = 0
.Ltotal_length_outer:
        beq     a0, t0, .Ltotal_length_done
        lw      t2, 0(a0)               # t2 = pointer into the current string
.Ltotal_length_inner:
        lbu     t3, 0(t2)               # t3 = the current character
        beqz    t3, .Ltotal_length_next # end of this string
        addi    t1, t1, 1               # count one character
        addi    t2, t2, 1               # next character
        j       .Ltotal_length_inner
.Ltotal_length_next:
        addi    a0, a0, 4               # next string
        j       .Ltotal_length_outer
.Ltotal_length_done:
        mv      a0, t1                  # return total
        ret
```

## Equal pointers and equal text

`beq` on two pointers asks whether they are the **same address**.
Two different strings can hold the same text at different addresses, and then `beq` says they differ.

To ask whether two strings hold the **same text**, compare them one character at a time:

- if the two characters differ, the strings differ;
- if they are the same and both are zero, you have reached the end of both strings, so they are equal;
- otherwise, move both pointers on by 1 and compare again.

```asm
# Returns 1 if the strings a and b hold the same text, else 0.
# a arrives in a0, b in a1.
same_text:
.Lsame_text_loop:
        lbu     t0, 0(a0)               # t0 = the current character of a
        lbu     t1, 0(a1)               # t1 = the current character of b
        bne     t0, t1, .Lsame_text_no  # they differ
        beqz    t0, .Lsame_text_yes     # both strings ended together
        addi    a0, a0, 1
        addi    a1, a1, 1
        j       .Lsame_text_loop
.Lsame_text_yes:
        li      a0, 1
        ret
.Lsame_text_no:
        li      a0, 0
        ret
```

```python
def same_text(a, b):
    return a == b
```

You only need to test one of the characters for zero.
If the other one were zero and this one were not, they would differ, and the `bne` would already have left the loop.

Python's `==` on strings compares the text, like `same_text`.
Python's `is` asks whether two values are the same object, like `beq` on two pointers.

## Storing a pointer

A pointer is a word, so you copy it from one array to another with `lw` and `sw`.
This copies the address, not the text: both arrays then point to the same string.

```asm
# out arrives in a0, names in a1.
        lw      t0, 8(a1)       # t0 = names[2]
        sw      t0, 0(a0)       # out[0] = names[2]
```

```python
out[0] = names[2]   # the same string, not a copy
```
