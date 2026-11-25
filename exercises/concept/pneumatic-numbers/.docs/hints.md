# Hints

## General

- Check that a code is in range before you use it as an index.
  One `bgeu` against the number of codes catches both negative codes and codes that are too big.
- A table of addresses is a table of words, so entry `i` is at offset `4 * i`, and you read it with `lw`.
- Put each table in `.section .rodata`, after a `.balign 4`.

## 1. Show the symbol

- The symbols can be a table of bytes, one per code, such as `.ascii "+-*/"`.
- Read a byte table with `lbu`, with no shift.
- A character literal such as `'?'` can go in a `li`.

## 2. Work out the answer

- Each entry of the table is the label of the code for one operation, written after `.word`.
- After the range check, use `slli`, `add`, `lw` and then `jr` to jump to the code for the operation.
- Each block of code can end with its own `ret`: the jump did not change `ra`.
- Check `b` for zero before you use `div`.

## 3. Run a paper tape

- Keep the wheel's number in a temporary register, and copy it to `a0` before you return.
- Load the table's address once, before the loop starts.
- Each block of code ends with `j` back to the loop, instead of `ret`.
- A code that is out of range should skip straight to the next step.

## 4. Try a new operation

- `fn` arrives in `a0`, but `fn` expects `a` in `a0` and `b` in `a1`.
- Move `fn` out of `a0` before you move the numbers in.
- If you jump to `fn` with `jr`, it returns straight to the tests, with its answer in `a0`.

## 5. Hand over the operation

- A function pointer is just the address of the function's first instruction.
- Each operation needs its own small function, which takes `a` in `a0` and `b` in `a1` and ends with `ret`.
- A table of `.word` entries can hold the addresses of those functions.
- Your answer to task 2 can use the same table, jumping to the function with `jr`.
