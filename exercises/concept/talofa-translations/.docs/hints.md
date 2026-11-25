# Hints

## General

- Put strings and tables after `.section .rodata`, and the visitor count after `.data`.
  Write `.text` again before your functions.
- Give each string and table its own label, and load its address with `la`.
- `.string` adds the zero terminator for you.
- Write `.balign 4` before a table of words.

## 1. Greet the visitor

- Put the greeting in your read-only data with `.string`, under a label.
- The function only needs to load the address of that label into `a0`.

## 2. Say thank you

- Give each of the four phrases its own label.
- A chain of branches, as in the conditionals concept, chooses which address to load into `a0`.
- You can type `ʻ`, `ā` and `ō` straight into a `.string`.

## 3. Count the days in a month

- Write the twelve numbers after one label, with `.byte 31, 28, ...`.
- January is the first byte of the table, at index 0, so the index is `month - 1`.
- A byte table needs no shift: add the index to the table's address, then use `lbu`.
- `lbu` takes an offset, and the offset may be negative.

## 4. Number the phrase of the day

- The table starts `.word 0, 31, 59, ...`: no days come before January, 31 come before February, and 59 before March.
- Each word takes 4 bytes, so shift the index left by 2 before adding it to the table's address.
- The answer is the table entry plus the day of the month.

## 5. Count the visitors

- A `.word 0` in `.data` gives you a count that starts at 0.
- Load the count, add 1, store it back, and return it.
- Storing into `.rodata` crashes the program, so make sure the count is in `.data`.
