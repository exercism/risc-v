# Hints

## General

- Each function receives its arguments in `a0`, `a1`, `a2`, in the order they appear in the C declaration at the top of the test file.
- Whatever is in `a0` when the function reaches `ret` is the value it returns.

## 1. Know the size of the hold

- `li` loads a number into a register.

## 2. Order some spares

- `addi` adds a number written in the instruction to a register.

## 3. Work out the space left

- `sub` subtracts one register from another, and the order of the operands matters.
- You will need the hold's size in a register of its own first. Any of `t0`–`t6` will do.

## 4. Keep a running count

- The three arguments arrive in `a0`, `a1` and `a2`.
- You can do this in two instructions.

## 5. Trust the crane

- The crane's count arrives in `a1`, but the result has to be in `a0`.
- `mv` copies one register into another.
