# Introduction

A **lookup table** is a list of answers, kept in memory, with one answer for each possible input.
Instead of working the answer out, or choosing it with a chain of branches, the function uses the input as an index and reads the answer from the table.

## From branches to a table

A sports day gives points for the first five places in a race: 10, 6, 4, 2 and 1.
Every other place gets 0 points.

With branches, the function checks each place in turn:

```asm
# Returns the points for a finishing place.
# place arrives in a0.
race_points:
        li      t0, 1
        beq     a0, t0, .Lrace_points_first     # place 1?
        li      t0, 2
        beq     a0, t0, .Lrace_points_second    # place 2?
        # ... three more checks, and five more labels
```

That is a lot of code, and it grows with every new place.
With a table, the code stays short, and the answers sit together where you can read them:

```asm
.section .rodata
place_points:                           # the points for places 1 to 5
        .byte   10, 6, 4, 2, 1

.text
# Returns the points for a finishing place.
# place arrives in a0.
race_points:
        addi    t0, a0, -1              # t0 = place - 1, the index into the table
        li      t1, 5                   # the table has 5 entries
        bgeu    t0, t1, .Lrace_points_none  # not 1 to 5: no points
        la      t1, place_points        # t1 = the address of place_points[0]
        add     t1, t1, t0              # t1 = the address of place_points[place - 1]
        lbu     a0, 0(t1)               # a0 = place_points[place - 1]
        ret
.Lrace_points_none:
        li      a0, 0
        ret
```

The same idea in Python is a list, or a dictionary:

```python
PLACE_POINTS = [10, 6, 4, 2, 1]

def race_points(place):
    if 1 <= place <= 5:
        return PLACE_POINTS[place - 1]
    return 0

# or, with a dictionary instead of if/elif:
POINTS = {1: 10, 2: 6, 3: 4, 4: 2, 5: 1}

def race_points(place):
    return POINTS.get(place, 0)
```

Code that chooses with branches is called **code-driven**.
Code that reads its answers from a table is called **data-driven**.
To change a data-driven function, you usually change only the table.

## Check the index first

A table has a fixed number of entries.
If the index is outside the table, the load reads whatever bytes happen to come after it, and the answer is nonsense.
So check the index before you use it, unless the instructions promise that it is always in range.

The check is the one-compare range check from the unsigned-integers concept.
After subtracting the first valid input, the index is valid exactly when it is less than the number of entries, compared as **unsigned**.
A negative index wraps around to a huge unsigned number, so the same comparison catches it.
In the example above, `bgeu` catches both place 0 and place 6.

## Indexing by a character

A character is a number, so it can be an index too.
To index a table by the letters `A` to `Z`, subtract `'A'`, so that `A` becomes index 0 and `Z` becomes index 25.

```asm
.section .rodata
        .balign 4
roman_values:                           # the value of each letter as a Roman numeral, or 0
        .word   0, 0, 100, 500, 0, 0, 0, 0, 1, 0, 0, 50, 1000   # A to M
        .word   0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 10, 0, 0          # N to Z

.text
# Returns the value of a Roman numeral letter.
# letter arrives in a0, and is always a capital letter.
roman_value:
        addi    t0, a0, -'A'            # t0 = 0 for 'A', up to 25 for 'Z'
        slli    t0, t0, 2               # each word is 4 bytes
        la      t1, roman_values
        add     t1, t1, t0              # t1 = the address of the letter's entry
        lw      a0, 0(t1)
        ret
```

A table of words needs the index times 4, as in the arrays concept.
A table of bytes uses the index as it is.

A table does not need an entry for every number, only for the inputs that can happen.
Here, entries that are not Roman numerals hold 0.
A special value that means "no answer", such as 0 or -1, is often called a **sentinel**.

### A shortcut: move the table's address back

For a table of bytes indexed by a letter, you can let the assembler subtract `'A'` from the table's address instead:

```asm
        la      t0, letter_table - 'A'  # t0 = where entry 0 would be, if the table started at character 0
        add     t0, t0, a0              # t0 = the address of the entry for the letter in a0
        lbu     a0, 0(t0)
```

`letter_table - 'A'` is an address 65 bytes before the table.
Adding a letter's code, from 65 for `A`, lands on the right entry.
This saves one instruction, but it skips the range check, so use it only when the input is always valid.

## Tables of addresses

The values after `.word` do not have to be numbers you write yourself.
A label stands for an address, so `.word some_label` stores the address of `some_label`.
A table of string labels is a table of `const char *` values: look up the entry, and you have the address of the string.

## One table leads to another

The value read from one table can be the index into a second table.
In much of the tropical South Pacific, November to April is the wet season, and May to October is the dry season.
This function looks up the season for a month, then the season's name:

```asm
.section .rodata
wet:
        .string "wet"
dry:
        .string "dry"

month_season:                           # 0 for wet, 1 for dry, January first
        .byte   0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0

        .balign 4
season_names:                           # the address of each season's name
        .word   wet, dry

.text
# Returns the name of the season for a month from 1 to 12.
# month arrives in a0.
season_name:
        la      t0, month_season
        add     t0, t0, a0
        lbu     t1, -1(t0)              # t1 = month_season[month - 1], the season
        slli    t1, t1, 2               # each address is 4 bytes
        la      t0, season_names
        add     t0, t0, t1
        lw      a0, 0(t0)               # a0 = season_names[season], an address
        ret
```

```python
MONTH_SEASON = [0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0]
SEASON_NAMES = ["wet", "dry"]

def season_name(month):
    return SEASON_NAMES[MONTH_SEASON[month - 1]]
```

The two seasons' names are stored once, however many months point to them.
