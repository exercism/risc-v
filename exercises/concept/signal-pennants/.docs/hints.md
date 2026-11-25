# Hints

## General

- Put your tables after `.section .rodata`, and write `.text` again before your functions.
- Give each table a label, and load its address with `la`.
- Subtract `'A'` from a letter to get an index from 0 to 25.
- A table of bytes uses the index as it is; a table of words needs the index times 4.
- Write `.balign 4` before a table of words that comes after bytes or strings.

## 1. Size the word cards

- A `.byte` table of 26 lengths, from Alfa's 4 to Zulu's 4, holds every answer.
- Check the index before you use it: after subtracting `'A'`, a capital letter gives 0 to 25.
- One unsigned comparison, `bgeu` or `bltu`, catches characters both before `A` and after `Z`.

## 2. Say the letter

- Give each of the 26 words its own label and `.string`.
- A table of `.word` entries can hold labels: `.word alfa, bravo, ...` stores the address of each string.
- Load the entry with `lw`, and return it: it is already the address of the word.

## 3. Know the colours

- A `.byte` table of 26 sets holds every answer, because the biggest set is 31.
- The assembler can add for you: `.byte 1 + 2` is the same as `.byte 3`, and is easier to check against the table of colours.
- The letter is always a capital letter, so you can skip the range check.
  You may like to try the shortcut `la t0, your_table - 'A'`.

## 4. Count the colours

- Load the letter's set of colours from your table from task 3, but do not return it yet.
- The set is a number from 0 to 31: use it as the index into a second `.byte` table of 32 entries.
- Entry `n` of the second table is the number of 1 bits in `n`: `0, 1, 1, 2, 1, 2, 2, 3, ...`.

## 5. Time the radio call

- Walk through the message one byte at a time with `lbu`, and stop at the zero byte.
- For each character, do the same range check and lookup as in task 1.
- If the character is not a capital letter, go straight on to the next one, rather than adding -1.
- You can load the table's address and the number 26 into registers once, before the loop.
