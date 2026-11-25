# Hints

## General

- Bit `i` of a log stands for boat `i`, from boat 0 in bit 0 to boat 31 in bit 31.
- A boat number arrives in `a1`, and `li t0, 1` then `sll t0, t0, a1` makes a mask with only that boat's bit set.
- Boat 31 is the top bit, which signed instructions read as a minus sign.
  Use `beqz`, `bnez` or `snez` rather than signed tests such as `bltz` or `sgtz`.

## 1. Log a sighting

- Make the boat's single-bit mask, then set that bit in the log with `or`.

## 2. Check for a boat

- `srl` moves the boat's bit down to bit 0, and `andi` with 1 keeps only that bit.
- Or keep only the boat's bit with `and` and a mask, then turn the result into 0 or 1 with `snez`.

## 3. Find the missing boats

- A boat is missing if it is in the first set and **not** in the second.
- `not` gives the set of every boat that was not seen.

## 4. Count the boats

- `x & (x - 1)` removes the smallest boat from `x`.
- Keep removing boats, and counting, until the log is 0.
- Test at the top of the loop, so that an empty log gives 0.

## 5. Name the first boat

- Check for the empty log first, and return -1 for it.
- Otherwise, look at bit 0, then shift the log right by one place and add 1 to the boat number, until bit 0 is set.

## 6. Read the flag message

- Walk the string one byte at a time with `lbu`, and stop at the zero byte.
- Subtracting `'a'` from a character gives its boat number, from 0 to 25, if it is a lowercase letter.
- The one-compare range check, with `sltiu` or `bgeu`, tells you whether that number is a boat at all.
- Then add the boat to the log, just as in task 1.
