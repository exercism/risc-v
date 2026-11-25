# Design

## Learning objectives

- Know that an array of strings is an array of 4-byte pointers, each the address of a string stored elsewhere.
- Read C types such as `const char **` and `const char *const *`.
- Load an element of a pointer array with `lw`, then follow it with `lbu`: two levels of indirection.
- Loop over a NULL-terminated pointer array, stopping when the loaded pointer is 0.
- Loop over a pointer array with a count, with an inner loop over each string's characters.
- Compare two strings' text byte by byte, and know that `beq` on pointers compares addresses.
- Find the index of the longest string.
- Read a grid of characters stored as an array of row strings: `rows[r][c]`.
- Store pointers into an output array with `sw`, sharing strings instead of copying them.
- Relate an array of pointers to a Python list of strings, and pointer equality to Python's `is`.

## Out of scope

- Ordering strings, as C's `strcmp` does (covered in `about.md` only).
- Strings and pointer tables in the data section, `.word` and `la` (the `data-section` concept).
- Calling a helper function such as a string comparison or length function (the `functions` concept).
- The stack and saved registers.
- Grids stored as one block, `char grid[ROWS][COLS]`.
- Sorting by swapping pointers.
- Vector instructions.
- Floating point, which this track does not cover.

## Concepts

- `pointer-arrays`

## Prerequisites

- `strings`: walking a string with `lbu` to its zero terminator, and character literals.
- `arrays`: element offsets of `4 * i`, walking a pointer, and testing at the top so that an empty array touches nothing.

## Analyzer

This exercise could benefit from the following rules in the analyzer:

- `essential`: loading an element of a pointer array with `lbu` or `lb` instead of `lw`.
- `essential`: `find_name` comparing the pointers with `beq` instead of comparing the text.
- `essential`: `find_name` stopping at the end of one string without checking that the other ended too, so that `"Grace"` matches `"Grace Tovua"`.
- `essential`: `collect_short` copying characters into `out` instead of storing pointers.
- `actionable`: `grid_char` adding `row` without shifting it by 2.
- `actionable`: `longest_record` replacing the best record when the new one is equally long, so a tie returns the later record.
- `informative`: `record_count` can find the count from the distance between the start and the null pointer, shifted right by 2.
- `informative`: `count_starting_with` needs only the first byte of each record, with no inner loop.
