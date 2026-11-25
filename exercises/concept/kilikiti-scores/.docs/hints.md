# Hints

## General

- The array arrives in `a0` as the address of element 0, and the number of elements arrives in `a1`.
- Each `int32_t` takes 4 bytes, so element `i` is at address `a0 + 4 * i`.
- Shifting left by 2 multiplies by 4.
- To walk a pointer, work out `end = a0 + 4 * count` before the loop, and stop when the pointer equals `end`.
- Test at the top of the loop, so that an empty array (count 0) never reads any element.

## 1. Look up one batter's runs

- Shift the index left by 2 to get the offset in bytes.
- Add the offset to the address in `a0`, then load the word there.

## 2. Add up the runs

- Keep a running total in a temporary register, starting at 0.
- Move the pointer on by 4 after each element.

## 3. Find the highest score

- There is always at least one score, so you can start with the first score as the highest so far.
- Then walk through the rest, starting at the second element.
- Use a branch to skip the update when the current score is not higher.

## 4. Count the ducks

- Keep a count in a temporary register, starting at 0.
- `bnez` can skip the `addi` when the current score is not 0.

## 5. Add the bonus runs

- For each element: load it, add the bonus, and store it back to the same address.
- Nothing needs to go in `a0` at the end.

## 6. Find the first century

- Put 100 in a temporary register before the loop, so that you can compare with it.
- You can keep an index in a register and add 1 to it each time round.
- Or you can keep a copy of the start address, and turn the pointer back into an index with `sub` and `srli`.
- If the loop reaches the end, return -1.
