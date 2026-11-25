# Hints

## General

- Each `int16_t` and `uint16_t` takes 2 bytes, so element `i` is at address `a0 + 2 * i`.
- Shifting left by 1 multiplies by 2.
- To walk a pointer, work out `end = a0 + 2 * count` before the loop, and move the pointer on by 2 after each element.
- Use `lh` for `int16_t` values, which may be negative, and `lhu` for `uint16_t` values.
- Once a halfword is loaded, the register holds an ordinary 32-bit number, and the usual branches work on it.

## 1. Read one temperature

- Shift the index left by 1 to get the offset in bytes.
- Add the offset to the address in `a0`, then load the halfword there with `lh`.

## 2. Find the coolest reading

- There is always at least one reading, so you can start with the first reading as the coolest so far.
- Then walk through the rest, starting at the second element.
- Use a branch to skip the update when the current reading is not lower.

## 3. Add up the changes

- Keep a running total in a temporary register, starting at 0.
- The total lives in a 32-bit register, so it can grow past 32767 without any problem.

## 4. Find the deepest logger

- Depths from 32768 up have bit 15 set.
- `lh` would copy that bit into the upper bits and make them look negative, so load each depth with `lhu`.
- After `lhu`, every value is between 0 and 65535, so `ble` or `bgt` compare them correctly.

## 5. Calibrate a logger

- For each element: load it with `lh`, add the offset, and store it back to the same address with `sh`.
- Do not use `sw`: it writes 4 bytes, and would change the next reading too.
- Nothing needs to go in `a0` at the end.

## 6. Count the risky readings

- Keep a count in a temporary register, starting at 0.
- The threshold arrives in `a2`, so you can compare each reading with it directly.
- `blt` can skip the `addi` when the current reading is below the threshold.
