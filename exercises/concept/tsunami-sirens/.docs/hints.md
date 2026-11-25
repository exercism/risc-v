# Hints

## General

- A branch jumps to a label when its comparison is true, and falls through to the next instruction when it is false.
- Branches compare two registers. To compare with a number, put the number in a temporary register such as `t0` with `li` first.
- Give each branch target a local label that starts with `.L` and the function's name, so labels in different functions never clash.
- A function may have more than one `ret`.

## 1. Pick the larger reading

- If `a0` is already the larger reading, there is nothing to do.
- Otherwise, copy `a1` into `a0` with `mv`.

## 2. Measure the change in the tide

- Start by subtracting the earlier reading from the later one.
- If the difference is negative, turn it round. Subtracting a number from `zero` changes its sign.
- `bgez` or `bltz` compares a register with zero.

## 3. Choose an alert level

- This is an `if`-`elif`-`else` chain with three tests.
- Test the smallest threshold first: if the height is less than 30, the answer is 0.
- `blt` jumps when the first register is less than the second.

## 4. Decide whether to walk to high ground

- `bnez` jumps when a register is not zero.
- An `or` condition becomes two branches that both jump to the same label.
- Remember to return exactly 1 or 0, not the value of an argument.

## 5. Sound the siren

- `beq` or `bne` compares the level with each possible value in turn.
- `beqz` is a quick way to test for level 0.
- Once you have handled levels 0, 1 and 2, the only level left is 3.
